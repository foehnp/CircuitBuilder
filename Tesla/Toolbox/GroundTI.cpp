#include "GroundTI.h"

#include <QPen>
#include <QPainter>

GroundTI::GroundTI(double squareBreadth, double thickness) :
    ComponentTI(squareBreadth),
    m_wireThickness(thickness)
{
}

void GroundTI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double thickness = m_wireThickness * m_squareBreadth;
    double halfThickness = 0.5 * thickness;
    double halfSquareBreadth = 0.5 * m_squareBreadth;
    double wireHeight = 0.45 * m_squareBreadth;
    double lineLength0 = 0.5 * m_squareBreadth;
    double lineLength1 = 0.35 * m_squareBreadth;
    double lineLength2 = 0.2 * m_squareBreadth;
    double interLineGap = thickness;


    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    // Wire
    painter->drawRect(QRectF(-halfThickness, -halfSquareBreadth, thickness, wireHeight));
    // Line 0
    painter->drawRect(QRectF(-0.5 * lineLength0, -halfSquareBreadth + wireHeight, lineLength0, thickness));
    // Line 1
    painter->drawRect(QRectF(-0.5 * lineLength1, -halfSquareBreadth + wireHeight + thickness + interLineGap, lineLength1, thickness));
    // Line 2
    painter->drawRect(QRectF(-0.5 * lineLength2, -halfSquareBreadth + wireHeight + 2*thickness + 2*interLineGap, lineLength2, thickness));


}
