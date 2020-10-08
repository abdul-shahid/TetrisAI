#include "board.h"
#include <iostream>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>

Board::Board(QWidget *parent) : QFrame(parent), isStarted(false)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus);
    clearBoard();
    nextPiece.setRandomShape();
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
    score = 0;
    numLines = 0;
    clearBoard();
    emit linesRemovedChanged(numLines);
    newPiece();
    timer.start(timeoutTime(), this);
}

void Board::startAI() {
    
}

void Board::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);
    QPainter painter(this);
    QRect rect = contentsRect();

    int boardTop = rect.bottom() - BoardHeight*squareHeight();

    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            PieceShape shape = shapeAt(j, BoardHeight - i - 1);
            if (shape != NoShape)
                draw(painter, rect.left() + j * squareWidth(),
                           boardTop + i * squareHeight(), shape);
        }
    }

    if (curPiece.shape() != NoShape) {
        for (int i = 0; i < 4; ++i) {
            int x = curX + curPiece.x(i);
            int y = curY - curPiece.y(i);
            draw(painter, rect.left() + x * squareWidth(),
                       boardTop + (BoardHeight - y - 1) * squareHeight(),
                       curPiece.shape());
        }
    }
}
void Board::keyPressEvent(QKeyEvent *event) {
    if (!isStarted) {
        QFrame::keyPressEvent(event);
        return;
    }
    switch (event->key()) {
        case Qt::Key_Left:
            tryMove(curPiece, curX - 1, curY);
            break;
        case Qt::Key_Right:
            tryMove(curPiece, curX +1, curY);
            break;
        case Qt::Key_Down:
            tryMove(curPiece.rotateRight(), curX, curY);
            break;
        case Qt::Key_Space:
            hardDrop();
            break;
        case Qt::Key_D:
            oneLineDown();
            break;
        case Qt::Key_Up:
            tryMove(curPiece.rotateLeft(), curX, curY);
            break;
        default:
            QFrame::keyPressEvent(event);
    }
}

void Board::timerEvent(QTimerEvent *event) {
    if (event->timerId() == timer.timerId()) {
        if (isWaitingAfterLine) {
            isWaitingAfterLine = false;
            newPiece();
            timer.start(timeoutTime(), this);
        } else {
        }
    } else QFrame::timerEvent(event);
}

void Board::clearBoard() {
    for (int i = 0; i < BoardHeight * BoardWidth; ++i)
            board[i] = NoShape;
}
void Board::newPiece() {
    curPiece = nextPiece;
    nextPiece.setRandomShape();
    showNextPiece();
    curX = curPiece.maxX() + 1;
    curY = BoardHeight - 2 + curPiece.minY();
    if (!tryMove(curPiece, curX, curY)) {
        curPiece.setShape(NoShape);
        timer.stop();
        isStarted = false;
    }
}
void Board::showNextPiece() {
    if (!nextLabel) return;
    int dx = nextPiece.maxX() - nextPiece.minX() + 1;
    int dy = nextPiece.maxY() - nextPiece.minY() + 1;
    QPixmap pixmap(dx * squareWidth(), dy * squareHeight());
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), nextLabel->palette().window());
    for (int i = 0; i < 4; ++i) {
        int x = nextPiece.x(i) - nextPiece.minX();
        int y = nextPiece.y(i) - nextPiece.minY();
        draw(painter, x * squareWidth(), y * squareHeight(), nextPiece.shape());
    }
    nextLabel->setPixmap(pixmap);
}
bool Board::tryMove(const Piece &newPiece, int newX, int newY) {
    for (int i = 0; i < 4; ++i) {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);
        if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight)
            return false;
        if (shapeAt(x, y) != NoShape)
            return false;
    }
    curPiece = newPiece;
    curX = newX;
    curY = newY;
    update();
    return true;
}

void Board::draw(QPainter &painter, int x, int y, PieceShape shape) {
    static constexpr QRgb colorTable[8] = {
        0x264653, 0x2A9D8F, 0xE9C46A, 0xF4A261,
        0xE76F51, 0x794c74, 0x19d3da, 0x03c4a1
    };

    QColor color = colorTable[int(shape)];
    painter.fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2,
                     color);

    painter.setPen(color.lighter());
    painter.drawLine(x, y + squareHeight() - 1, x, y);
    painter.drawLine(x, y, x + squareWidth() - 1, y);

    painter.setPen(color.darker());
    painter.drawLine(x + 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + squareHeight() - 1);
    painter.drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + 1);
}

void Board::oneLineDown() {
    if (!tryMove(curPiece, curX, curY - 1))
        pieceDropped();
}
void Board::pieceDropped() {
    for (int i = 0; i < 4; ++i) {
        int x = curX + curPiece.x(i);
        int y = curY - curPiece.y(i);
        shapeAt(x, y) = curPiece.shape();
    }
    removeFullLines();

    if (!isWaitingAfterLine)
        newPiece();
}
void Board::removeFullLines() {
    int fullLines = 0;
    for (int i = BoardHeight -1; i >= 0; --i) {
        bool isFull = true;
        for (int j = 0; j < BoardWidth; ++j)
            if (shapeAt(j, i) == NoShape) {
                isFull = false;
                break;
            }
        if (isFull) {
            ++fullLines;
            for (int k = i; k < BoardHeight - 1; ++k) {
                for (int j = 0; j < BoardWidth; ++j) {
                    shapeAt(j, k) = shapeAt(j, k + 1);
                }
            }
            for (int j = 0; j  < BoardWidth; ++j) {
                shapeAt(j, BoardHeight - 1) = NoShape;
            }
        }
    }
    if (fullLines > 0) {
        numLines += fullLines;
        emit linesRemovedChanged(numLines);
        timer.start(500, this);
        isWaitingAfterLine = true;
        curPiece.setShape(NoShape);
        update();
    }
}

void Board::hardDrop() {
    while (tryMove(curPiece, curX, curY - 1)) {}
    pieceDropped();
}