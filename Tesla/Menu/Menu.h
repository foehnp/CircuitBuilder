#pragma once

#include <QGraphicsItem>
#include <EnumsDefsEtc.h>

class MainView;
class MenuAction;

class Menu : public QGraphicsItem
{
public:
    Menu(MainView* mainView, double squareBreadth);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void pushBackAction(MenuAction* menuAction);

private:
    MainView* m_mainView = nullptr;
    double m_squareBreadth;
    std::vector<MenuAction*> m_toolboxItemList;

    RunMode m_state = Drawing;

};
