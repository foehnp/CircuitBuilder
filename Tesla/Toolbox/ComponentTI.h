#pragma once
#include <QGraphicsItem>

class ToolboxArea;

class ComponentTI : public QGraphicsItem
{
public:
    ComponentTI(double squareBreadth);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF boundingRect() const override;

    void setIndex(int index);
    int getIndex() const;

    void setToolboxArea(ToolboxArea* toolboxArea);
    ToolboxArea* getToolboxArea() const;

    void setSelected(int selected);
    int isSelected() const;

    void setIsAvailable(bool isAvailable);
    int isAvailable() const;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void paintBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;



protected:
    double m_squareBreadth;

    int m_selected = 0;

    bool m_isAvailable = true;

private:
    QGraphicsRectItem* m_backgroundSquare;
    double m_boundaryThickness = 0.02;

    int m_index;
    ToolboxArea* m_parentToolboxArea;
};

