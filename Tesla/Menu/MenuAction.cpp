#include "MenuAction.h"

#include <QGraphicsSceneMouseEvent>

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
    QPixmap pM(getIconPath());
    pM.scaledToHeight(m_squareBreadth);
    m_iconPixmapItem = new QGraphicsPixmapItem(pM);
    QRectF rect = m_iconPixmapItem->boundingRect();
    m_iconPixmapItem->setScale(m_squareBreadth / rect.height());
    m_iconPixmapItem->setPos(0., 0.);
    m_iconPixmapItem->setParentItem(this);
}

void MenuAction::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    executeAction();
}
