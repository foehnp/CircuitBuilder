#ifndef SCALEGRAPHICSITEM_H
#define SCALEGRAPHICSITEM_H

#include "ScaleCollection.h"

#include <QGraphicsItem>

class MainView;
class ScaleWidget;

class ScaleGraphicsItem : public QGraphicsItem
{
public:
    ScaleGraphicsItem(double width, double height);

    void setScale(const ColorFunction& colorFunc);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override final;

    QRectF boundingRect() const override;

private:
    double m_width;
    double m_height;
    int m_steps = 40;
    std::vector<QColor> m_colors;
};


#endif // SCALEGRAPHICSITEM_H
