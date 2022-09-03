#pragma once

#include "ComponentTI.h"

#include <QGraphicsPixmapItem>

class EraserTI : public ComponentTI
{
public:
    EraserTI(double squareBreadth);

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    QGraphicsPixmapItem* m_eraserIcon;
};
