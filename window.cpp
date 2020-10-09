#include "board.h"
#include "window.h"
#include <QCoreApplication>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLCDNumber>
Window::Window(QWidget *parent) : QWidget(parent), board(new Board)
{
    nextLabel = new QLabel;
    nextLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    nextLabel->setAlignment(Qt::AlignCenter);
    board->setNextLabel(nextLabel);
    startButton = new QPushButton(tr("&Start"));
    startButton->setFocusPolicy(Qt::NoFocus);
    quitButton = new QPushButton(tr("&Quit"));
    quitButton->setFocusPolicy(Qt::NoFocus);
    AIButton = new QPushButton(tr("&AI"));
    AIButton->setFocusPolicy(Qt::NoFocus);
    linesLcd = new QLCDNumber(5);
    linesLcd->setSegmentStyle(QLCDNumber::Filled);
    connect(AIButton, &QPushButton::clicked, board, &Board::startAI);
    connect(startButton, &QPushButton::clicked, board, &Board::start);
    connect(quitButton, &QPushButton::clicked, board, &QCoreApplication::quit);
    connect(board, &Board::linesRemovedChanged, linesLcd, QOverload<int>::of(&QLCDNumber::display));
    

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createLabel(tr("NEXT")), 0, 0);
    layout->addWidget(nextLabel, 1, 0);
    layout->addWidget(startButton, 4, 0);
    layout->addWidget(board, 0, 1, 6, 1);
    layout->addWidget(quitButton, 5, 2);
    layout->addWidget(AIButton, 6, 2);
    layout->addWidget(linesLcd, 3, 2);
    setLayout(layout);

    setWindowTitle(tr("Tetris"));
    resize(550, 370);
}
QLabel *Window::createLabel(const QString &text)
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return label;
}