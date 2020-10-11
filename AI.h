#ifndef ARTI
#define ARTI
#include <vector>
#include "piece.h"
using b = std::vector<std::vector<PieceShape>>;
class Board;
class AI {
private:
    double get_score(b &board, int newX, int newY, Piece &p);
    double aggregate_score(b &board);
public:
    AI();
    Piece getBest(b &board, Piece &cur, int &curX, int &curY, int BoardHeight, int BoardWidth);
    bool move(b &board, Piece &newPiece, int newX, int newY, int &BoardWidth, int &BoardHeight);
};
#endif