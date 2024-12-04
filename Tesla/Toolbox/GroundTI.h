#pragma once

#include "ComponentTI.h"

class GroundTI : public ComponentTI
{
public:
    GroundTI(double squareBreadth, double thickness);

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    double m_wireThickness = 0.05;
};
