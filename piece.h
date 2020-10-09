#ifndef PIECE
#define PIECE
#include <list>
#include <iostream>
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
    friend std::ostream& operator<<(std::ostream& os, Piece const &tc) {
        switch (tc.shape())
        {
        case NoShape:
            os << "NoShape"; 
            break;
        case Z:
            os << "Z";
            break;
        case S:
            os << "S";
            break;
        case LINE:
            os << "LINE";
            break;
        case T:
            os << "T";
            break;
        case SQUARE:
            os << "SQUARE";
            break;
        case L:
            os << "L";
            break;
        case LL:
            os << "LL";
            break;
        default:
            break;
        }
        return os;
    }
private:
    void setX(int index, int x) {coords[index][0] = x;}
    void setY(int index, int y) {coords[index][1] = y;}
    PieceShape piece;
    int coords[4][2];
    std::list<PieceShape> pieces{Z,S,LINE,T,SQUARE,L,LL};
};
#endif