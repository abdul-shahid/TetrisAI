#ifndef WINDOW
#define WINDOW
#include <QWidget>
QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QLCDNumber;
QT_END_NAMESPACE
class Board;
//! [0]
class Window : public QWidget {
    Q_OBJECT
public:
    Window(QWidget *parent = nullptr);
private:
    QLabel *createLabel(const QString &text);
    Board *board;
    QLabel *nextLabel;
    QLCDNumber *linesLcd;
    QPushButton *startButton;
    QPushButton *quitButton;
    QPushButton *AIButton;
};
//! [0]
#endif