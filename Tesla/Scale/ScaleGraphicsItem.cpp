#include "ScaleGraphicsItem.h"
#include "ScaleWidget.h"

#include <MainView.h>

#include <QPainter>

ScaleGraphicsItem::ScaleGraphicsItem(double width, double height) :
    m_width(width),
    m_height(height)
{
    m_colors = std::vector<QColor>(m_steps, Qt::gray);
}

void ScaleGraphicsItem::setScale(const ColorFunction& colorFunc)
{
    double stepDelta = 1. / m_steps;
    for (int i = 0; i < m_steps; ++i)
    {
        m_colors[m_steps - i - 1] = colorFunc((i+0.5) * stepDelta);
    }
}

void ScaleGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double stepHeight = m_height / m_steps;
    for (int i = 0; i < m_steps; ++i)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush( m_colors[i]);
        painter->drawRect(QRectF(0., i * stepHeight, m_width, stepHeight));
    }
}

QRectF ScaleGraphicsItem::boundingRect() const
{
    return QRectF(0., 0., m_width, m_height);
}


