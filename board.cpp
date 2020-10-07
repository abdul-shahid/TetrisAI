#include "board.h"

#include <QKeyEvent>
#include <QLabel>
#include <QPainter>

Board::Board(QWidget *parent) : QFrame(parent), isStarted(false)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus);
}
void Board::setNextLabel(QLabel *label) {nextLabel = label;}
QSize Board::sizeHint() const {
    return QSize(BoardWidth *15 + frameWidth() * 2, BoardHeight *15 + frameWidth() * 2);
}
QSize Board::minimumSizeHint() const {
    return QSize(BoardWidth *5 + frameWidth() * 2, BoardHeight *5 + frameWidth() * 2);
}
void Board::start() {
    isStarted = true;
    isWaitingAfterLine = false;
}
void Board::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);
    QPainter painter(this);
    QRect rect = contentsRect();

    int boardTop = rect.bottom() - BoardHeight*squareHeight();
    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            // drawShape
        }
    }
    // draw current piece
}
void Board::keyPressEvent(QKeyEvent *event) {
    if (!isStarted) {
        QFrame::keyPressEvent(event);
        return;
    }
    switch (event->key()) {
        case Qt::Key_Left:
            break;
        case Qt::Key_Right:
            break;
        case Qt::Key_Down:
            break;
        case Qt::Key_Space:
            break;
        default:
        QFrame::keyPressEvent(event);
    }
}

void Board::timerEvent(QTimerEvent *event) {
    QFrame::timerEvent(event);
}