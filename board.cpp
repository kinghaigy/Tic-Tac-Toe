#include "board.h"
#include "square.h"
#include <cstdlib>
#include <QPen>
#include <QPainter>
#include <QColor>
#include <iostream>
#include <TicTacToeAI.h>

board::board(QWidget *parent) :
    QWidget(parent),
    m_lastStart(NAUGHT),
    m_winner(NONE),
    m_vsComputer(false),
    m_turn(CROSS),
    m_winType(PLAYING),
    m_turnCounter(0)
{
    squareGrid = new QGridLayout(this);
    squareGrid->setSpacing(0);
    squareGrid->setMargin(0);

    this->setLayout(squareGrid);

    /* MAKE OUR SQUARES*/
    for (int idxY = 0; idxY < 3; idxY++)
    {
        for (int idxX = 0; idxX < 3; idxX++)
        {
            square *tempSquare = new square(idxX, idxY, this);
            m_squares.push_back(tempSquare);
            squareGrid->addWidget(tempSquare, idxY, idxX);
            connect(tempSquare,SIGNAL(clicked(int,int)),this, SLOT(squareClicked(int,int)));
            m_squareValues.push_back(0);
        }
    }
}

void board::checkForWin()
{
    m_squareValues = getIntState(); //MEMORY LEAK?

    /* CHECKING TIME */
    //ROW 1
    if (abs(m_squareValues.at(0) + m_squareValues.at(1) + m_squareValues.at(2)) == 3)
    {
        m_winner = m_turn;
        m_winType = ROW1;
    }
    //ROW 2
    if (abs(m_squareValues.at(3) + m_squareValues.at(4) + m_squareValues.at(5)) == 3)
    {
        m_winner = m_turn;
        m_winType = ROW2;
    }
    //ROW 3
    if (abs(m_squareValues.at(6) + m_squareValues.at(7) + m_squareValues.at(8)) == 3)
    {
        m_winner = m_turn;
        m_winType = ROW3;
    }
    //COL 1
    if (abs(m_squareValues.at(0) + m_squareValues.at(3) + m_squareValues.at(6)) == 3)
    {
        m_winner = m_turn;
        m_winType = COL1;
    }
    //COL 2
    if (abs(m_squareValues.at(1) + m_squareValues.at(4) + m_squareValues.at(7)) == 3)
    {
        m_winner = m_turn;
        m_winType = COL2;
    }
    //COL 3
    if (abs(m_squareValues.at(2) + m_squareValues.at(5) + m_squareValues.at(8)) == 3)
    {
        m_winner = m_turn;
        m_winType = COL3;
    }
    //DIAG DOWN
    if (abs(m_squareValues.at(0) + m_squareValues.at(4) + m_squareValues.at(8)) == 3)
    {
        m_winner = m_turn;
        m_winType = DIAGDOWN;
    }
    //DIAG UP
    if (abs(m_squareValues.at(6) + m_squareValues.at(4) + m_squareValues.at(2)) == 3)
    {
        m_winner = m_turn;
        m_winType = DIAGUP;
    }

    //CHECK FOR JACK
    if (m_turnCounter == 9 && m_winType == PLAYING)
    {
        m_winType = JACK;
    }
}
int board::stateToInt(State state) const
{
    switch(state)
    {
        case NAUGHT:
            return 1;
            break;
        case CROSS:
            return -1;
            break;
        case NONE:
            return 0;
            break;
    }
}

std::vector<int> board::getIntState() const
{
    std::vector<int> tempSquareValues;
    for (int idx = 0; idx < 9; idx++)
    {
        tempSquareValues.push_back(stateToInt(m_squares.at(idx)->getState()));
    }
    return tempSquareValues;
}

void board::resetGame()
{
    /* Delete all the objects stored in the vector, then the vector */
    for (int idx = 0; idx < m_squares.size(); idx ++)
    {
        delete m_squares.at(idx);
    }
    m_squares.clear();
    m_squareValues.clear();

    /* MAKE OUR SQUARES AGAIN*/
    for (int idxY = 0; idxY < 3; idxY++)
    {
        for (int idxX = 0; idxX < 3; idxX++)
        {
            square *tempSquare = new square(idxX, idxY, this);
            m_squares.push_back(tempSquare);
            squareGrid->addWidget(tempSquare, idxY, idxX);
            connect(tempSquare,SIGNAL(clicked(int,int)),this, SLOT(squareClicked(int,int)));
            m_squareValues.push_back(0);
        }
    }
    /* Reset the winners from last round */
    m_winner = NONE;
    m_winType =PLAYING;
    m_turnCounter = 0;

    /* UPDATE THE TOP TEXT */
    if (m_turn == NAUGHT)
    {
        emit updateTurnHeader("Player 2's Turn [O]");
    }
    else if (m_turn == CROSS)
    {
        emit updateTurnHeader("Player 1's Turn [X]");
    }
    /* TAKE THE COMPUTERS TURN IF IT IS ITS TURN AND PLAYING */
    if (m_turn == NAUGHT && m_vsComputer == true)
    {
        takeComputerTurn();
    }
}

void board::displayWin()
{
    if (m_winner == NAUGHT)
    {
        emit updateTurnHeader("Player 2 [O] Wins!");
        std::cout << "Player 2 wins updated";
    }
    else if (m_winner == CROSS)
    {
        emit updateTurnHeader("Player 1 [X] Wins!");
        std::cout << "Player 1 wins updated";
    }
    else if (m_winType == JACK)
    {
        emit updateTurnHeader("It's a jack!");
        std::cout << "It's a Jack updated";
    }
}

void board::nextTurn()
{
    if (m_turn == NAUGHT && m_winType == PLAYING)
    {
        m_turn = CROSS;
        emit updateTurnHeader("Player 2's Turn [X]");
    }
    else if (m_turn == CROSS && m_winType == PLAYING)
    {
        m_turn = NAUGHT;
        emit updateTurnHeader("Player 2's Turn [O]");
    }
}

void board::paintEvent(QPaintEvent *)
{
    QPen pen(Qt::black, this->height()/50, Qt::SolidLine);

    QPainter painter(this);
    //pen.setWidth(2);
    //pen.setColor("#000000");
    painter.setPen(pen);
    painter.drawLine(this->width()/3, 0, this->width()/3, this->height());
    painter.drawLine(2*this->width()/3, 0, 2*this->width()/3, this->height());
    painter.drawLine(0, this->height()/3, this->width(), this->height()/3);
    painter.drawLine(0, 2*this->height()/3, this->width(), 2*this->height()/3);

    /* THE WINNING LINES ARE DRAWN HERE */
    pen.setColor(Qt::red);
    pen.setWidth(height()/25);
    painter.setPen(pen);
    switch (m_winType)
    {
        case ROW1:
            painter.drawLine(this->width()/6, this->height()/6, 5*this->width()/6, this->height()/6);
            this->update();
            break;
        case ROW2:
            painter.drawLine(this->width()/6, this->height()/2, 5*this->width()/6, this->height()/2);
            this->update();
            break;
        case ROW3:
            painter.drawLine(this->width()/6, 5*this->height()/6, 5*this->width()/6, 5*this->height()/6);
            this->update();
            break;
        case COL1:
            painter.drawLine(this->width()/6, this->height()/6, this->width()/6, 5*this->height()/6);
            this->update();
            break;
        case COL2:
            painter.drawLine(this->width()/2, this->height()/6, this->width()/2, 5*this->height()/6);
            this->update();
            break;
        case COL3:
            painter.drawLine(5*this->width()/6, this->height()/6, 5*this->width()/6, 5*this->height()/6);
            this->update();
            break;
        case DIAGDOWN:
            painter.drawLine(this->width()/6, this->height()/6, 5*this->width()/6, 5*this->height()/6);
            this->update();
            break;
        case DIAGUP:
            painter.drawLine(this->width()/6, 5*this->height()/6, 5*this->width()/6, this->height()/6);
            this->update();
            break;
    }
}

void board::resizeEvent(QResizeEvent *)
{
    int height = this->height();
    int width = this->width();
    if (height <= width)
    {
        this->resize(height, height);
    }
    else if (width <= height)
    {
        this->resize(width, width);
    }
}

void board::vsComputer(bool option)
{
    std::cout << "Bool m_vsComputer is set to " << option << std::endl;
    m_vsComputer = option;
}

void board::squareClicked(int xPos, int yPos)
{
    std::cout << "The square at " << xPos << ", " << yPos <<" has been clicked!" << std::endl;
    if (m_squares.at(3*yPos + xPos)->getState() == NONE && m_winType == PLAYING)
    {
        m_squares.at(3*yPos + xPos)->setState(m_turn);
        m_turnCounter++;

        checkForWin();
        if (m_winType != PLAYING)
        {
            displayWin();
            std::cout << "WE HAVE A WINNER!!" << std::endl;
        }
        nextTurn();
        if (m_vsComputer == true && m_turn == NAUGHT && m_winType == PLAYING)
        {
            takeComputerTurn();
        }
    }
}

void board::takeComputerTurn()
{
    std::cout << "Called computer function" << std::endl;
    int intScores[9];
    vectorToArray(intScores, m_squareValues);
    int position;
    position = BestNextMove(intScores, 1);
    m_squares.at(position)->setState(m_turn);
    checkForWin();
    nextTurn();
    m_turnCounter++;
}

void board::vectorToArray(int intArray[], std::vector<int> intVector) const
{
    //This is a very NAUGHTY function. In the way it's used, it shouldn't be a problem though.
    int size = intVector.size();
    for (int idx = 0; idx < size; idx++)
    {
        intArray[idx] = intVector[idx];
    }
}
