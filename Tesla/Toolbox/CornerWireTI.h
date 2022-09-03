#ifndef CORNERWIRETI_H
#define CORNERWIRETI_H

#include "ComponentTI.h"

class CornerWireTI : public ComponentTI
{
public:
    CornerWireTI(double squareBreadth, double thickness);

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    double m_thickness = 0.05;
};

#endif // CORNERWIRETI_H
