#ifndef BOARD_H
#define BOARD_H

#include "square.h"
#include <QWidget>
#include <QGridLayout>
#include <vector>
#include <QString>


enum Win {
    PLAYING,
    ROW1,
    ROW2,
    ROW3,
    COL1,
    COL2,
    COL3,
    DIAGDOWN,
    DIAGUP,
    JACK
};

class board : public QWidget
{
    Q_OBJECT
public:
    explicit board(QWidget *parent = 0);
    void checkForWin();
    int stateToInt(State state) const;
    square* getSquare(int xPos, int yPos);
    std::vector<int> getIntState() const;


signals:
    void updateTurnHeader(QString string);

public slots:
    void resetGame();
    void vsComputer(bool option);
    void squareClicked(int xPos, int yPos);

private:
    std::vector<square*> m_squares;
    std::vector<int> m_squareValues;
   State m_winner;
   State m_turn;
   State m_lastStart;
   bool m_vsComputer;
   QGridLayout* squareGrid;
   void nextTurn();
   void displayWin();
   Win m_winType;
   void vectorToArray(int intArray[], std::vector<int> intVector) const;
   void takeComputerTurn();
   int m_turnCounter;

protected:
   void paintEvent(QPaintEvent *);
   void resizeEvent(QResizeEvent *);

};

#endif // BOARD_H
