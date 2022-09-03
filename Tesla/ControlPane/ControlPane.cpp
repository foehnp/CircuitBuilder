#include "ControlPane.h"

#include <MainView.h>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

ControlPane::ControlPane(MainView* mainView, double squareBreadth) :
    m_mainView(mainView),
    m_squareBreadth(squareBreadth)
{

}

void ControlPane::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRect(0., 0. , m_squareBreadth, m_squareBreadth);
    if (m_state == Drawing)
    {
        static const QPointF trianglePoints[3] = {    QPointF(m_squareBreadth * 0.2, m_squareBreadth * 0.2),
                                                      QPointF(m_squareBreadth * 0.2, m_squareBreadth * 0.8),
                                                      QPointF(m_squareBreadth * 0.8, m_squareBreadth * 0.5)
                                                 };
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::green);
        painter->drawPolygon(trianglePoints, 3);
    }
    if (m_state == Running)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        painter->drawRect(0.2*m_squareBreadth, 0.2*m_squareBreadth , 0.6*m_squareBreadth, 0.6*m_squareBreadth);
    }
}

QRectF ControlPane::boundingRect() const
{
    return QRectF(0., 0., m_squareBreadth, m_squareBreadth);
}

void ControlPane::setState(const RunMode &runMode)
{
    m_state = runMode;
    update();
}

void ControlPane::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!boundingRect().contains(event->pos()))
    {
        event->ignore();
        return;
    }
    if (QRectF(0., 0. , m_squareBreadth, m_squareBreadth).contains(event->pos()))
    {
        if (m_state == Drawing)
        {
            m_mainView->setCurrentRunMode(Running);
        }
        else if (m_state = Running)
        {
            m_mainView->setCurrentRunMode(Drawing);
        }
    }
}
