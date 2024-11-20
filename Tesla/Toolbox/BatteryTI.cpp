#include "BatteryTI.h"

#include <QPen>
#include <QPainter>

BatteryTI::BatteryTI(double squareBreadth, double thickness) :
    ComponentTI(squareBreadth),
    m_wireThickness(thickness)
{
}

void BatteryTI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfBreadth = 0.5 * m_squareBreadth;
    double wireThickness = m_squareBreadth * m_wireThickness;
    double gapThickness = m_squareBreadth * m_gapThickness;
    double leftHeight = m_squareBreadth * m_leftHeight;
    double rightHeight = m_squareBreadth * m_rightHeigt;
    double signHeight = m_squareBreadth * 0.15;
    double leftSignX = -m_squareBreadth * 0.35;
    double rightSignX = m_squareBreadth * 0.1;


    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->drawRect(QRectF(-halfBreadth, - wireThickness*0.5, halfBreadth - 0.5*gapThickness - wireThickness, wireThickness));
    painter->drawRect(QRectF(-0.5*gapThickness - wireThickness, -leftHeight*0.5, wireThickness, leftHeight));

    painter->setPen(Qt::black);
    QFont font = painter->font();
    font.setPointSizeF(12.);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(QPointF(leftSignX, -signHeight), QString("+"));

    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->drawRect(QRectF(0.5*gapThickness + wireThickness, - wireThickness*0.5,  halfBreadth - 0.5*gapThickness - wireThickness, wireThickness));
    painter->drawRect(QRectF(0.5*gapThickness, -rightHeight*0.5, wireThickness, rightHeight));

    painter->setPen(Qt::black);
    painter->drawText(QPointF(rightSignX, -signHeight), QString("-"));

}
