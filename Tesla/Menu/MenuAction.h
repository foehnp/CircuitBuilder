#pragma once

#include <QGraphicsItem>

class MainView;
class Menu;

class MenuAction : public QGraphicsItem
{
public:
    MenuAction(double squareBreadth, Menu* parentMenu, MainView* mainView);
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;


protected:
    virtual QString getIconPath() const = 0;
    virtual void executeAction() = 0;

    virtual bool isAvailable() const = 0;

private:
    double m_squareBreadth;

    QGraphicsPixmapItem* m_iconPixmapItem;

protected:
    Menu* m_parentMenu;
    MainView* m_mainView;
};
