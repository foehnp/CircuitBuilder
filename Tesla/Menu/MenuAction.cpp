#include "MenuAction.h"

#include <QGraphicsSceneMouseEvent>

#include <QPainter>
#include <QPen>

MenuAction::MenuAction(double squareBreadth, Menu* parentMenu, MainView* mainView) :
    m_squareBreadth(squareBreadth),
    m_parentMenu(parentMenu),
    m_mainView(mainView)
{

}

QRectF MenuAction::boundingRect() const
{
    return QRectF(0., 0., m_squareBreadth, m_squareBreadth);
}

void MenuAction::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //paint background
    QColor backgroundColor = isAvailable() ? Qt::white : Qt::gray;
    QPen pen;
    pen.setColor(backgroundColor);
    QBrush brush(backgroundColor);
    painter->setBrush(brush);
    painter->setPen(pen);
    painter->drawRect(QRectF(0., 0., m_squareBreadth, m_squareBreadth));

}

void MenuAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    if (!isAvailable())
    {
        return;
    }
    executeAction();
}
