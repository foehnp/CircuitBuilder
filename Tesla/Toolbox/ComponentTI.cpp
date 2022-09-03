#include "ComponentTI.h"

#include "ToolboxArea.h"

#include <EnumsDefsEtc.h>

#include <QPainter>
#include <QPen>
#include <QGraphicsSceneMouseEvent>


ComponentTI::ComponentTI(double squareBreadth) :
    QGraphicsItem(),
    m_squareBreadth(squareBreadth)
{
}

void ComponentTI::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    paintBackground(painter, option, widget);
    paintSymbol(painter, option, widget);
}

void ComponentTI::paintBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfBreadth = m_squareBreadth * 0.5;
    double halfAbsoluteThickness = m_boundaryThickness * m_squareBreadth * 0.5;

    QPen pen;
    pen.setWidthF(halfAbsoluteThickness);
    QColor backgroundColor = m_isAvailable ?
               (m_selected ?  ITEM_SELECTED_COLOR : Qt::transparent)
              : Qt::gray;
    QBrush brush(backgroundColor);
    painter->setBrush(brush);
    painter->setPen(pen);
    painter->drawRect(QRectF(-halfBreadth, -halfBreadth, m_squareBreadth, m_squareBreadth));
}

QRectF ComponentTI::boundingRect() const
{
    double halfBreadth = m_squareBreadth * 0.5;
    return QRectF(-halfBreadth, -halfBreadth, m_squareBreadth, m_squareBreadth);
}

void ComponentTI::setIndex(int index)
{
    m_index = index;
}

int ComponentTI::getIndex() const
{
    return m_index;
}

void ComponentTI::setToolboxArea(ToolboxArea *toolboxArea)
{
    m_parentToolboxArea = toolboxArea;
}

ToolboxArea *ComponentTI::getToolboxArea() const
{
    return m_parentToolboxArea;
}

void ComponentTI::setSelected(int selected)
{
    if (selected == m_selected)
    {
        return;
    }
    m_selected = selected;
    update();
}

int ComponentTI::isSelected() const
{
    return m_selected;
}

void ComponentTI::setIsAvailable(bool isAvailable)
{
    m_isAvailable = isAvailable;
}

int ComponentTI::isAvailable() const
{
    return m_isAvailable;
}



void ComponentTI::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    if (m_isAvailable)
    {
        getToolboxArea()->setSelectedItem(m_index);
    }
}
