#include "ForkTI.h"

#include <QBrush>
#include <QPen>
#include <QPainter>

ForkTI::ForkTI(double squareBreadth, double thickness) :
    ComponentTI(squareBreadth),
    m_thickness(thickness)
{
}

void ForkTI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfThickness = 0.5 * m_thickness * m_squareBreadth;
    double halfSquareBreadth = 0.5 * m_squareBreadth;

    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->setBrush(Qt::black);
    painter->drawRect(QRectF(-halfSquareBreadth, 0. - halfThickness, halfSquareBreadth, 2*halfThickness));
    painter->drawRect(QRectF(-halfThickness, -halfSquareBreadth, 2*halfThickness, halfSquareBreadth));
    painter->drawRect(QRectF(-halfThickness, 0., 2*halfThickness, halfSquareBreadth));

    painter->drawEllipse({0., 0.}, 2.5 * halfThickness, 2.5 * halfThickness);
}
