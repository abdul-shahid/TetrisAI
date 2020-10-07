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
    Shape &shapeAt(int x, int y) {return board[(y * BoardWidth) + x];}
    QBasicTimer timer;
    QPointer<QLabel> nextLabel;
    
    bool isStarted;
    bool isWaitingAfterLine;
    
    Piece curPiece;
    Piece nextPiece;
    int curX;
    int curY;
    int score;
    int level;
    Shape board[BoardWidth * BoardHeight];

    
};
#endif