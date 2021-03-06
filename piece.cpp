#include "piece.h"
#include <QtCore>

void Piece::setRandomShape() {
    if (pieces.size() == 0) {
        std::list<PieceShape> p{Z,S,LINE,T,SQUARE,L,LL};
        pieces = p;
    }
    int random = QRandomGenerator::global()->bounded((int)pieces.size());
    std::list<PieceShape>::iterator it = pieces.begin();
    std::advance(it, random);
    setShape(*it);
    pieces.erase(it);
}

void Piece::setShape(PieceShape shape) {
    static constexpr int coordsTable[8][4][2] = {
        { { 0, 0 },   { 0, 0 },   { 0, 0 },   { 0, 0 } },
        { { 0, -1 },  { 0, 0 },   { -1, 0 },  { -1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 1, 0 },   { 1, 1 } },
        { { 0, -1 },  { 0, 0 },   { 0, 1 },   { 0, 2 } },
        { { -1, 0 },  { 0, 0 },   { 1, 0 },   { 0, 1 } },
        { { 0, 0 },   { 1, 0 },   { 0, 1 },   { 1, 1 } },
        { { -1, -1 }, { 0, -1 },  { 0, 0 },   { 0, 1 } },
        { { 1, -1 },  { 0, -1 },  { 0, 0 },   { 0, 1 } }
    };

    for (int i = 0; i < 4 ; i++) {
        for (int j = 0; j < 2; ++j)
            coords[i][j] = coordsTable[shape][i][j];
    }
    piece = shape;
}

int Piece::minX() const {
    int min = coords[0][0];
    for (int i = 1; i < 4; ++i) min = qMin(min, coords[i][0]);
    return min;
}
int Piece::maxX() const {
    int max = coords[0][0];
    for (int i = 1; i < 4; ++i) max = qMax(max, coords[i][0]);
    return max;
}

int Piece::minY() const {
    int min = coords[0][1];
    for (int i = 1; i < 4; ++i) min = qMin(min, coords[i][1]);
    return min;
}
int Piece::maxY() const {
    int max = coords[0][1];
    for (int i = 1; i < 4; ++i) max = qMax(max, coords[i][1]);
    return max;
}
Piece Piece::rotateLeft() const {
    if (piece == SQUARE)
        return *this;

    Piece result;
    result.piece = piece;
    for (int i = 0; i < 4; ++i) {
        result.setX(i, y(i));
        result.setY(i, -x(i));
    }
//! [7]
    return result;
}
Piece Piece::rotateRight() const {
    if (piece == SQUARE) return *this;
    Piece result;
    result.piece = piece;
    for (int i = 0; i < 4; ++i) {
        result.setX(i, -y(i));
        result.setY(i, x(i));
    }
    return result;
}