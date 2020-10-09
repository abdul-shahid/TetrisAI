#include "AI.h"
#include "piece.h"
#include "board.h"
#include <vector>
using b = std::vector<std::vector<PieceShape>>;
AI::AI() {}
Piece AI::getBest(b board, Piece &cur, int &curX, int &curY, int BoardHeight, int BoardWidth) {
    // for each of the 4 rotations
    for (int i = 0; i < 4; ++i) {
        if (cur.shape() == SQUARE && i > 0) break;
        if (cur.shape() == LINE || cur.shape() == S || cur.shape() == Z)
            if (i > 1) break;
        Piece c(cur);
        int newX = c.maxX() + 1;
        // move as left as possible
        while (move(board, c, newX, curY, BoardWidth, BoardHeight)) --newX;
        while (!move(board, c, newX, curY, BoardWidth, BoardHeight)) ++newX;
        
        while (move(board, c, newX, curY, BoardWidth, BoardHeight)) {
            // drop and test
            ++newX;
        }
        c = c.rotateLeft();
    }
    return Piece(cur);
}


bool AI::move(b &board, Piece &newPiece, int &newX, int &newY, int &BoardWidth, int &BoardHeight) {
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