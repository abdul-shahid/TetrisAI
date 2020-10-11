#include "AI.h"
#include "piece.h"
#include "board.h"
#include <vector>
using b = std::vector<std::vector<PieceShape>>;
AI::AI() {}
Piece AI::getBest(b &board, Piece &cur, int &curX, int &curY, int BoardHeight, int BoardWidth) {
    // for each of the 4 rotations
    Piece best = cur;
    int prev = curX;
    double score = -2147483647;
    Piece c(cur);
    for (int i = 0; i < 4; ++i) {
        if (cur.shape() == SQUARE && i > 0) break;
        if (cur.shape() == LINE || cur.shape() == S || cur.shape() == Z)
            if (i > 1) break;
        int newX = c.maxX() + 1;
        // move as left as possible
        while (move(board, c, newX, curY, BoardWidth, BoardHeight))
            --newX;
        while (!move(board, c, newX, curY, BoardWidth, BoardHeight))
            ++newX;
        while (move(board, c, newX, curY, BoardWidth, BoardHeight)) {
            int newY = curY;
            while(move(board, c, newX, newY-1, BoardWidth, BoardHeight))
                --newY;
            double piece_score = get_score(board, newX, newY, c);
            if (piece_score > score) {
                best = c;
                curX = newX;
                score = piece_score;
            }
            ++newX;
        }
        c = c.rotateLeft();
    }
    return best;
}
double AI::get_score(b &board, int newX, int newY, Piece &p) {
    for (int i = 0; i < 4; ++i) {
        int x = newX + p.x(i);
        int y = newY - p.y(i);
        board[y][x] = p.shape();
    }
    int ret = aggregate_score(board);
    for (int i = 0; i < 4; ++i) {
        int x = newX + p.x(i);
        int y = newY - p.y(i);
        board[y][x] = NoShape;
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
    int prev = -1;
    int bumpiness = 0;
    for (int j = 0; j < m; ++j) {
        int i = n-1;
        while (i >=0 && board[i][j] == NoShape) --i;
        if (prev >= 0 && i >= 0) {
            bumpiness += std::abs(i - prev);
        }
        prev = i;
        deepest_well = std::max(deepest_well, max_height -  i);
    }
    return 7.5*holes - max_height + 9*full_lines - 3.5*wells - 1.5 * bumpiness - 4*blockades - 2 *deepest_well;
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