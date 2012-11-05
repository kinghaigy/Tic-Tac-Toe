#ifndef SQUARE_H
#define SQUARE_H

#include <QWidget>
#include <QColor>

enum State {
    NONE,
    NAUGHT,
    CROSS
};

class square : public QWidget
{
    Q_OBJECT
public:
    explicit square(int xPos, int yPos, QWidget *parent = 0);
    void setState(State state);
    State getState() const;


    
signals:
    void clicked(int xPos, int yPos);

public slots:
    
private:
    State m_state;
    int m_xPos;
    int m_yPos;

protected:
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
};

#endif // SQUARE_H
