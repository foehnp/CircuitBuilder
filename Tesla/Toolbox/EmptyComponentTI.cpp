#include "EmptyComponentTI.h"

EmptyComponentTI::EmptyComponentTI(double squareBreadth) :
    ComponentTI(squareBreadth)
{
}

void EmptyComponentTI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}
