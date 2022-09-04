#include "DiodeTI.h"

#include <QPainter>

DiodeTI::DiodeTI(double squareBreadth, double thickness) :
    ComponentTI(squareBreadth),
    m_thickness(thickness)
{
}

void DiodeTI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfThickness = 0.5 * m_thickness * m_squareBreadth;
    double halfSquareBreadth = 0.5 * m_squareBreadth;

    QColor leftColor = Qt::black, rightColor = Qt::black;

    painter->setBrush(leftColor);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->drawRect(QRectF(-halfSquareBreadth, -halfThickness, halfSquareBreadth, 2.*halfThickness));

    painter->setBrush(rightColor);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->drawRect(QRectF(0., -halfThickness, halfSquareBreadth, 2*halfThickness));

    painter->setBrush(Qt::black);;
    painter->setPen(QPen(Qt::PenStyle::NoPen));

    QVector<QPointF> trianglePoints({{-0.3*halfSquareBreadth, -0.4*halfSquareBreadth},
                            {-0.3*halfSquareBreadth, 0.4*halfSquareBreadth},
                            {0.3*halfSquareBreadth, 0.}});
    QPolygonF triangle(trianglePoints);
    painter->drawPolygon(triangle);

    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->drawRect(QRectF(0.3*halfSquareBreadth, -0.4*halfSquareBreadth, 2.*halfThickness, 0.8*halfSquareBreadth));
}
