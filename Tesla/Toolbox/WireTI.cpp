#include "WireTI.h"

#include <QBrush>
#include <QPen>
#include <QPainter>

WireTI::WireTI(double squareBreadth, double thickness) :
    ComponentTI(squareBreadth),
    m_thickness(thickness)
{
}

void WireTI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfThickness = 0.5 * m_thickness * m_squareBreadth;
    double halfSquareBreadth = 0.5 * m_squareBreadth;

    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->drawRect(QRectF(-halfSquareBreadth, 0. - halfThickness, m_squareBreadth, 2*halfThickness));
}
