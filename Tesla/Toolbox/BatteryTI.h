#pragma once

#include "ComponentTI.h"

class BatteryTI : public ComponentTI
{
public:
    BatteryTI(double squareBreadth, double thickness);

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    double m_wireThickness = 0.05;
    double m_gapThickness = 0.1;
    double m_leftHeight = 0.8;
    double m_rightHeigt = 0.3;
};
