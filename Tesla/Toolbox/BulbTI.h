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
};
