#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Frame Stuff */
    gameBoard = new board(this);
    QHBoxLayout *boardLayout = new QHBoxLayout(this);
    ui->boardFrame->setLayout(boardLayout);
    boardLayout->addWidget(gameBoard);

    /* Signals and Slots */
    connect(ui->resetButton, SIGNAL(clicked()),gameBoard, SLOT(resetGame()));
    connect(gameBoard, SIGNAL(updateTurnHeader(QString)), ui->turnHeader, SLOT(setText(QString)));
    connect(ui->computerButton, SIGNAL(toggled(bool)), gameBoard, SLOT(vsComputer(bool)));


}

MainWindow::~MainWindow()
{
    delete ui;
}
