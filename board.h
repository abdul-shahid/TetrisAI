#ifndef BOARD
#define BOARD
#include <QBasicTimer>
#include <QFrame>
#include <QPointer>
#include "piece.h"

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE
class Board : public QFrame
{
    Q_OBJECT
public:
    Board(QWidget *parent = nullptr);
    void setNextLabel(QLabel *label);
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
public slots:
    void start();
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
private:
    enum {BoardWidth = 10, BoardHeight = 22};
    int timeoutTime() { return 1000 / (1 + level);}
    int squareWidth() {return contentsRect().width() / BoardWidth;}
    int squareHeight() {return contentsRect().height() / BoardHeight;}
    PieceShape &shapeAt(int x, int y) {return board[(y * BoardWidth) + x];}

    void clearBoard();
    void newPiece();
    void showNextPiece();
    bool tryMove(const Piece &newPiece, int newX, int newY);
    void draw(QPainter &painter, int x, int y, PieceShape shape);
    void oneLineDown();
    void pieceDropped();
    void removeFullLines();
    void hardDrop();
    QBasicTimer timer;
    QPointer<QLabel> nextLabel;
    Piece curPiece;
    Piece nextPiece;
    
    bool isStarted;
    bool isWaitingAfterLine;
    int curX;
    int curY;
    int score;
    int level;
    PieceShape board[BoardWidth * BoardHeight];

    
};
#endif