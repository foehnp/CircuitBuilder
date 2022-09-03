#pragma once

#include "ComponentTI.h"

class BulbTI : public ComponentTI
{
public:
    BulbTI(double squareBreadth, double thickness);

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    double m_wireThickness = 0.05;
    double m_radius = 0.25;

    QGraphicsRectItem* m_leftWire;
    QGraphicsRectItem* m_rightWire;
    QGraphicsRectItem* m_crossBar0;
    QGraphicsRectItem* m_crossBar1;
};
