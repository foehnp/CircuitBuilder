#include "EraserTI.h"

#include <QPainter>


EraserTI::EraserTI(double squareBreadth) :
    ComponentTI(squareBreadth)
{
//    m_rubberIcon = new QGraphicsPixmapItem("Images/eraser.jpg");
    QPixmap pM(":/Images/eraserTrans.png");
    pM.scaledToHeight(squareBreadth);
    m_eraserIcon = new QGraphicsPixmapItem(pM);
    QRectF rect = m_eraserIcon->boundingRect();
    m_eraserIcon->setScale(0.8 * squareBreadth / rect.height());
    m_eraserIcon->setPos(-0.4 * squareBreadth,-0.4 * squareBreadth);
    m_eraserIcon->setParentItem(this);
}


void EraserTI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pM(":/Images/eraserTrans.png");
    pM.scaledToHeight(m_squareBreadth);
    painter->drawPixmap(QRectF(-0.4 * m_squareBreadth,-0.4 * m_squareBreadth, 0.8 * m_squareBreadth, 0.8 * m_squareBreadth), pM, pM.rect());
}
