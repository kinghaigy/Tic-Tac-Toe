#include "square.h"
#include <QPen>
#include <QPainter>
#include <QColor>
#include <QRect>
#include <QString>

square::square(int xPos, int yPos, QWidget *parent):
    QWidget(parent),
    m_xPos(xPos),
    m_yPos(yPos),
    m_state(NONE)
{
    //setFixedSize(parent->width()/3, parent->height()/3);
}
void square::setState(State state)
{
    m_state = state;
    this->update();
}

State square::getState() const
{
    return m_state;
}

void square::mousePressEvent(QMouseEvent *)
{
    emit clicked(m_xPos, m_yPos);
}


void square::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    painter.setBrush(brush);
    /* MAKE A RECTANGLE TO HOLD TEXT THE SAME SIZE AS THE BOX */
    QRect textBox(1,3,this->width(), this->height()-3);

    /* MAKE A PEN THAT IS BLACK AND HAS A THICKNESS OF 2PX */
    QPen pen(Qt::black, 0, Qt::SolidLine);


    /* MAKE A NEW FONT AND GIVE IT A FONT WE KNOW */
    QFont font("Comic Sans"); //heheh

    /* SET THAT FONT SIZE TO SOMETHING AWESOME */
    font.setPixelSize(this->height()*0.8);

    /* TELL THE PAINTER WHAT FONT TO USE AND WHICH PEN TO DRAW LINES/TEXT WITH */
    painter.setFont(font);
    painter.setPen(pen);

    /* DRAWN THAT TEXT BOX ON TOP OF IT. THE TEXT BOX IS THE SAME SIGN AND ALL THE TEXT IS CENTERD */
    if(m_state == NAUGHT) {
        pen.setColor(Qt::black);
        painter.drawText(textBox, Qt::AlignCenter, "O");
    }
    else if (m_state == CROSS) {
        pen.setColor(Qt::black);
        painter.drawText(textBox, Qt::AlignCenter, "X");
    }

}

