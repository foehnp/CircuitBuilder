#ifndef FORKTI_H
#define FORKTI_H

#include "ComponentTI.h"

class ForkTI : public ComponentTI
{
public:
    ForkTI(double squareBreadth, double thickness);

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    double m_thickness = 0.05;
};

#endif // FORKTI_H
