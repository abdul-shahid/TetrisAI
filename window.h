#ifndef WINDOW
#define WINDOW
#include <QWidget>
QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
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
    QPushButton *startButton;
    QPushButton *quitButton;
};
//! [0]
#endif