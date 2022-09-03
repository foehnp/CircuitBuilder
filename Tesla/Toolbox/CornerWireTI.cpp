#include "CornerWireTI.h"

#include <QBrush>
#include <QPen>
#include <QPainter>


CornerWireTI::CornerWireTI(double squareBreadth, double thickness) :
    ComponentTI(squareBreadth),
    m_thickness(thickness)
{
}

void CornerWireTI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfThickness = 0.5 * m_thickness * m_squareBreadth;
    double halfSquareBreadth = 0.5 * m_squareBreadth;

    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->drawRect(QRectF(-halfThickness, -halfThickness, halfThickness * 2 , halfSquareBreadth + halfThickness));
    painter->drawRect(QRectF(-halfThickness, -halfThickness , halfSquareBreadth + halfThickness, halfThickness * 2));
}
