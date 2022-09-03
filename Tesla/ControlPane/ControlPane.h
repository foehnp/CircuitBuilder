#pragma once

#include <QGraphicsItem>
#include <EnumsDefsEtc.h>

class MainView;

class ControlPane : public QGraphicsItem
{
public:
    ControlPane(MainView* mainView, double squareBreadth);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    void setState(const RunMode& runMode);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    MainView* m_mainView = nullptr;
    double m_squareBreadth;

    RunMode m_state = Drawing;

};
