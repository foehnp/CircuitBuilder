#ifndef BJTTI_H
#define BJTTI_H

#include "ComponentTI.h"

class BJTTI : public ComponentTI
{
public:
    BJTTI(double squareBreadth, double thickness);

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    double m_thickness = 0.05;
};

#endif // BJTTI_H
