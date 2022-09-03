#pragma once

#include <QGraphicsItem>

class ComponentTI;
class MainView;

class ToolboxArea : public QGraphicsItem
{
public:
    ToolboxArea(double squareBreadth, MainView* mainView);

    void pushBackItem(ComponentTI* toolboxItem);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF boundingRect() const override;

    void setSelectedItem(int index);

    void setIsAvailable(bool isOn);

private:
    double m_squareBreadth;
    std::vector<ComponentTI*> m_toolboxItemList;
    int m_currSelected;

    bool m_isAvailable;

    MainView* m_mainView;
};
