#include "AI.h"
#include "piece.h"
#include "board.h"
#include <vector>
using b = std::vector<std::vector<PieceShape>>;
AI::AI() {}
Piece AI::getBest(b &board, Piece &cur, Piece &next, int &curX, int &curY, int BoardHeight, int BoardWidth) {
    // for each of the 4 rotations
    Piece best = cur;
    double score = -2147483647;
    Piece c(cur);
    for (int i = 0; i < 4; ++i) {
        if (cur.shape() == SQUARE && i > 0) break;
        if (cur.shape() == LINE || cur.shape() == S || cur.shape() == Z)
            if (i > 1) break;
        int newX = c.maxX() + 1;
        // move as left as possible
        newX = moveLeft(board, c, BoardWidth, BoardHeight, curX, curY);
        b bb = board;
        while (move(board, c, newX, curY, BoardWidth, BoardHeight)) {
            int newY = move_down(board, c, newX, curY, BoardWidth, BoardHeight);
            double piece_score = get_score(board, newX, newY, c);
            
            Piece n(next);
            for (int j = 0; j < 4; ++j) {
                if (next.shape() == SQUARE && j > 0) break;
                if (next.shape() == LINE || cur.shape() == S || cur.shape() == Z)
                    if (j > 1) break;
                int cy = BoardHeight - 2 + n.minY();
                int nx = moveLeft(board, n, BoardWidth, BoardHeight, n.maxX() + 1,cy);
                while (move(board, n, nx, cy, BoardWidth, BoardHeight)) {
                    int ny = move_down(board, n, nx, cy, BoardWidth, BoardHeight);
                    b bb2 = board;
                    double next_score = get_score(board, nx, ny, n);
                    board = bb2;
                    if (piece_score + next_score > score) {
                        best = c;
                        curX = newX;
                        score = piece_score + next_score;
                    }
                    ++nx;
                }
                n = n.rotateLeft();
                
            }
            board = bb;
            ++newX;
        }
        c = c.rotateLeft();
    }
    return best;
}
int AI::moveLeft(b &board, Piece &piece, int BoardWidth, int BoardHeight, int curX, int curY) {
     int newX = piece.maxX() + 1;
     while (move(board, piece, newX, curY, BoardWidth, BoardHeight))
            --newX;
    int iter = newX;
    while (!move(board, piece, newX, curY, BoardWidth, BoardHeight)) {
        if (newX - iter > BoardWidth + 1) break;
        ++newX;
    }
    return newX;
}
int AI::move_down(b &board, Piece &c, int newX, int newY, int BoardWidth, int BoardHeight) {
    while(move(board, c, newX, newY-1, BoardWidth, BoardHeight)) --newY;
    return newY;
}
double AI::get_score(b &board, int newX, int newY, Piece &p) {
    for (int i = 0; i < 4; ++i) {
        int x = newX + p.x(i);
        int y = newY - p.y(i);
        board[y][x] = p.shape();
    }
    int fl = 0;
    int bh = board.size(), bw = board[0].size();
    double ret = aggregate_score(board);
    for (int i = bh -1; i >= 0; --i) {
        bool isFull = true;
        for (int j = 0; j < bw; ++j)
            if (board[i][j] == NoShape) {
                isFull = false;
                break;
            }
        if (isFull) {
            ++fl;
            for (int k = i; k < bh - 1; ++k) {
                for (int j = 0; j < bw; ++j) {
                    board[k][j] = board[k+1][j];
                }
            }
            for (int j = 0; j  < bw; ++j) {
                board[bh - 1][j] = NoShape;
            }
        }
    }
    return ret;
}

double AI::aggregate_score(b &board) {
    int n = board.size(), m = board[0].size();
    int holes = 0;
    int max_height = 0;
    int full_lines = 0;
    int wells = 0;
    int blockades = 0;
    std::vector<int> holes_by_col(m, 0);
    std::vector<int> found_by_col(m, 0);
    for (int i = n-1; i > 0; --i) {
        bool isEmpty = true;
        bool isFull = true;
        for (int j = 0; j < m; ++j) {
            if (board[i][j] != NoShape && board[i-1][j] == NoShape) {
                --holes;
                if (holes_by_col[j] > 0) ++blockades;
                if (i > 1 && board[i-2][j] == NoShape){ 
                    ++wells;
                    if (found_by_col[j]) ++blockades;
                    ++holes_by_col[j];
                }
                ++holes_by_col[j];
            }
            
            if (board[i][j] != NoShape) {
                isEmpty = false;
                found_by_col[j] = true;
            }
            if (board[i][j] == NoShape) {
                isFull = false;
            }
        }
        if (isFull) ++full_lines;
        if (!isEmpty) ++max_height;
    }
    int deepest_well = 0;
    int smallest = n;
    int prev = -10;
    int bumpiness = 0;
    for (int j = 0; j < m; ++j) {
        int i = n -1;
        while (i >= 0 && board[i][j] == NoShape) --i;
        smallest = std::min(smallest, i);
        int dep = 0;
        while (i >= 0) {
            if (board[i][j] != NoShape){
                deepest_well = std::max(deepest_well, dep);
                dep = 0;
            }
            else ++dep;
            --i;
        }
        deepest_well = std::max(deepest_well, dep);
    }
    for (int j = 0; j < m; ++j) {
        int i = n-1;
        while (i >=0 && board[i][j] == NoShape) --i;
        if (prev > -10 && i >= 0) {
            bumpiness += std::abs(i - prev);
        }

        prev = i;
    }
    if (full_lines >= 3) full_lines *= 2;
    return 10*holes - 4*max_height + 6*full_lines -3.5 *bumpiness - 5*deepest_well;
}


bool AI::move(b &board, Piece &newPiece, int newX, int newY, int &BoardWidth, int &BoardHeight) {
    for (int i = 0; i < 4; ++i) {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);
        if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight)
            return false;
        if (board[y][x] != NoShape)
            return false;
    }
    return true;
}