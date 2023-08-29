#include "BJTTI.h"

#include <QPainter>

BJTTI::BJTTI(double squareBreadth, double thickness) :
    ComponentTI(squareBreadth),
    m_thickness(thickness)
{

}

void BJTTI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfThickness = 0.5 * m_thickness * m_squareBreadth;
    double halfSquareBreadth = 0.5 * m_squareBreadth;

    {
        // Left wire (base)
        painter->setPen(QPen(Qt::PenStyle::NoPen));
        painter->setBrush(Qt::black);
        painter->drawRect(QRectF(-halfSquareBreadth, - halfThickness, halfSquareBreadth*0.5 - halfThickness*3., 2*halfThickness));
    }
    {
        // Semiconductior wire
        painter->setPen(QPen(Qt::PenStyle::NoPen));
        painter->setBrush(Qt::black);
        painter->drawRect(QRectF(-halfSquareBreadth*0.5 - halfThickness*3.,  -halfSquareBreadth * 0.4, halfThickness*2., halfSquareBreadth * 0.8));
    }
    // Top wire
    {
        painter->setPen(QPen(Qt::PenStyle::NoPen));
        painter->setBrush(Qt::black);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawRect(QRectF(-halfThickness,  -halfSquareBreadth, halfThickness*2., halfSquareBreadth * 0.3));
    }
    {
        // Upper diagonal wire (collector)
        painter->setPen(QPen(Qt::PenStyle::NoPen));
        painter->setBrush(Qt::black);
        QVector<QPointF> points({{-halfThickness, -0.7*halfSquareBreadth},
                                 {halfThickness, -0.7*halfSquareBreadth},
                                 {halfThickness, -0.7*halfSquareBreadth + 0.83*halfThickness},
                                 {-halfSquareBreadth*0.5 - halfThickness, -halfSquareBreadth*0.2},
                                 {-halfSquareBreadth*0.5 - halfThickness, -halfSquareBreadth*0.2 - halfThickness*2.83}});
        QPolygonF diagLine(points);
        painter->drawPolygon(diagLine);
    }
    {
        // Bottom wire
        painter->setPen(QPen(Qt::PenStyle::NoPen));
        painter->setBrush(Qt::black);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawRect(QRectF(-halfThickness,  halfSquareBreadth, halfThickness*2., -halfSquareBreadth * 0.3));
    }
    {
        // Lower diagonal wire (collector)
        painter->setPen(QPen(Qt::PenStyle::NoPen));
        painter->setBrush(Qt::black);
        QVector<QPointF> points({{-halfThickness, 0.7*halfSquareBreadth},
                                 {halfThickness, 0.7*halfSquareBreadth},
                                 {halfThickness, 0.7*halfSquareBreadth - 0.83*halfThickness},
                                 {-halfSquareBreadth*0.5 - halfThickness, halfSquareBreadth*0.2},
                                 {-halfSquareBreadth*0.5 - halfThickness, halfSquareBreadth*0.2 + halfThickness*2.83}});
        QPolygonF diagLine(points);
        painter->drawPolygon(diagLine);
    }

}
