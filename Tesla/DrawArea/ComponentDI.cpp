#include "ComponentDI.h"

#include <EnumsDefsEtc.h>

#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPolygonF>

ComponentDI::ComponentDI(double squareBreadth, int orientation) :
    QGraphicsItem(),
    m_squareBreadth(squareBreadth),
    m_orientation(orientation)
{
    if (m_orientation != 0)
    {
        setRotation(-90.*m_orientation);
    }
}

void ComponentDI::setCurrentRunMode(const RunMode &runMode)
{
    m_runMode = runMode;
    update();
}

RunMode ComponentDI::getCurrentRunMode() const
{
    return m_runMode;
}

void ComponentDI::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    paintBackground(painter, option, widget);
    paintSymbol(painter, option, widget);
}

void ComponentDI::paintBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfBreadth = m_squareBreadth * 0.5;
    double halfAbsoluteThickness = m_boundaryThickness * m_squareBreadth * 0.5;

    QPen pen;
    pen.setWidthF(halfAbsoluteThickness);
    QBrush brush(Qt::transparent);
    painter->setBrush(brush);
    painter->setPen(pen);
    painter->drawRect(QRectF(-halfBreadth, -halfBreadth, m_squareBreadth, m_squareBreadth));
}

QRectF ComponentDI::boundingRect() const
{
    double halfBreadth = m_squareBreadth * 0.5;
    return QRectF(-halfBreadth, -halfBreadth, m_squareBreadth, m_squareBreadth);
}

int* ComponentDI::getNodeAt(int direction)
{
    int actualDirection = (direction + m_orientation + 4) % 4;
    return getNodeAtInternal(actualDirection);
}

void ComponentDI::setDisplaySettings(const std::shared_ptr<ScaleCollection> &displaySettings)
{
    m_displaySettings = displaySettings;
}

void ComponentDI::setSolver(const std::shared_ptr<Solver> &solver)
{
    m_solver = solver;
}

void ComponentDI::setNLSolver(const std::shared_ptr<NLSolver> &NLsolver)
{
    m_NLsolver = NLsolver;
}

void ComponentDI::paintArrow(QPainter *painter, const QPointF &pos, const double &height, const QColor& color, int orientation)
{
    double breadth = height * 0.35;
    const double& backspace = height;
    QVector<QPointF> points;
    if (orientation == 0)
    {
        points = {{-2*breadth - backspace, height},
                  {-breadth - backspace, height},
                  {0., 0.},
                  {-breadth - backspace, -height},
                  {-2*breadth - backspace, -height},
                  {-breadth, 0.}};
    }
    else if (orientation == 2)
    {
        points = {{2*breadth + backspace, height},
                  {breadth + backspace, height},
                  {0., 0.},
                  {breadth + backspace, -height},
                  {2*breadth + backspace, -height},
                  {breadth, 0.}};
    }
    else if (orientation == 1)
    {
        points = {{height, 2*breadth + backspace},
                  {height, breadth + backspace},
                  {0., 0.,},
                  {-height, breadth + backspace},
                  {-height, 2*breadth + backspace},
                  {0., breadth}};
    }
    else if (orientation == 3)
    {
        points = {{height, -2*breadth - backspace},
                  {height, -breadth - backspace},
                  {0., 0.,},
                  {-height, -breadth - backspace},
                  {-height, -2*breadth - backspace},
                  {0., -breadth}};
    }
    QPolygonF qpolygon(points);
    qpolygon.translate(pos);
    painter->setBrush(color);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->drawPolygon(qpolygon);
}
