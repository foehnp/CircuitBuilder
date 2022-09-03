#pragma once

#include "ComponentTI.h"

class EmptyComponentTI : public ComponentTI
{
public:
    EmptyComponentTI(double squareBreadth);

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;


};
