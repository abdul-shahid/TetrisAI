#ifndef PIECE
#define PIECE
#include <list>
enum PieceShape {NoShape, Z, S, LINE, T, SQUARE, L, LL};
class Piece
{
public:
    Piece() {setShape(NoShape);}
    void setRandomShape();
    void setShape(PieceShape shape);
    PieceShape shape() const {return piece;}
    int x(int index) const {return coords[index][0];}
    int y(int index) const {return coords[index][1];}
    int minX() const;
    int minY() const;
    int maxX() const;
    int maxY() const;
    Piece rotateLeft() const;
    Piece rotateRight() const;
private:
    void setX(int index, int x) {coords[index][0] = x;}
    void setY(int index, int y) {coords[index][1] = y;}
    PieceShape piece;
    int coords[4][2];
    std::list<PieceShape> pieces{Z,S,LINE,T,SQUARE,L,LL};
};
#endif