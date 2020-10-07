#include "board.h"
#include "window.h"
#include <QCoreApplication>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
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

    connect(startButton, &QPushButton::clicked, board, &Board::start);
    connect(quitButton, &QPushButton::clicked, board, &QCoreApplication::quit);
    

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createLabel(tr("NEXT")), 0, 0);
    layout->addWidget(nextLabel, 1, 0);
    layout->addWidget(startButton, 4, 0);
    layout->addWidget(board, 0, 1, 6, 1);
    layout->addWidget(quitButton, 5, 2);

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