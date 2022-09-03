#pragma once
#include "ComponentTI.h"

class WireTI : public ComponentTI
{
public:
    WireTI(double squareBreadth, double thickness);

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    double m_thickness = 0.05;
};
