#include "BulbTI.h"

#include <QPainter>

BulbTI::BulbTI(double squareBreadth, double thickness) :
    ComponentTI(squareBreadth),
    m_wireThickness(thickness)
{

}

void BulbTI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfBreadth = 0.5 * m_squareBreadth;
    double wireThickness = m_squareBreadth * m_wireThickness;
    double radius = m_squareBreadth * m_radius;

    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->setBrush(Qt::black);

    painter->drawRect(QRectF(-halfBreadth, - wireThickness*0.5, halfBreadth - radius, wireThickness));
    painter->drawRect(QRectF(radius, - wireThickness*0.5, halfBreadth - radius, wireThickness));

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::white);
    QPen pen(Qt::black);
    pen.setWidthF(wireThickness);
    painter->setPen(pen);
    painter->drawEllipse(QRectF(-radius, -radius, 2*radius, 2*radius));
}
