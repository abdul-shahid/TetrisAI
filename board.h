#ifndef BOARD
#define BOARD
#include <QBasicTimer>
#include <QFrame>
#include <QThread>
#include <QPointer>
#include "piece.h"
#include "AI.h"
#include <vector>
QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE
class AI;
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
    void startAI();
signals:
    void linesRemovedChanged(int numLines);
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
private:
    enum {BoardWidth = 10, BoardHeight = 22};
    int timeoutTime() { return 1000;}
    int squareWidth() {return contentsRect().width() / BoardWidth;}
    int squareHeight() {return contentsRect().height() / BoardHeight;}
    PieceShape &shapeAt(int x, int y) {return board[y][x];}
    void clearBoard();
    void newPiece();
    void showNextPiece();
    bool tryMove(const Piece &newPiece, int newX, int newY);
    void draw(QPainter &painter, int x, int y, PieceShape shape);
    void oneLineDown();
    void pieceDropped();
    void removeFullLines();
    void hardDrop();
    void startTimer();
    QBasicTimer timer;
    QPointer<QLabel> nextLabel;
    Piece curPiece;
    Piece nextPiece;
    
    bool isStarted;
    bool isAI;
    bool isWaitingAfterLine;
    int curX;
    int curY;
    int score;
    int numLines = 0;
    std::vector<std::vector<PieceShape>> board{BoardHeight, std::vector<PieceShape>{BoardWidth, NoShape}};
    AI ai;
};
#endif