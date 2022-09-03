#include "CornerWireDI.h"

#include <Scale/ScaleCollection.h>

#include <QBrush>
#include <QPen>
#include <QPainter>

CornerWireDI::CornerWireDI(double squareBreadth, double thickness, int orientation) :
    ComponentDI(squareBreadth, orientation),
    m_thickness(thickness)
{
}

void CornerWireDI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfThickness = 0.5 * m_thickness * m_squareBreadth;
    double halfSquareBreadth = 0.5 * m_squareBreadth;

    QColor color;
    if (m_runMode == Drawing)
    {
        color = Qt::black;
    }
    else
    {
        if (m_displaySettings->getCurrentQuantity() == Potential)
        {
            color = m_displaySettings->getColorFromCurrentScale(m_NLsolver->getPotential(m_rightNode));
        }
        else if (m_displaySettings->getCurrentQuantity() == Current)
        {
            color = m_displaySettings->getColorFromCurrentScale(std::abs(m_NLsolver->getCurrent(m_edge)));
        }
    }

    painter->setBrush(color);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->drawRect(QRectF(-halfThickness, -halfThickness, halfThickness * 2 , halfSquareBreadth + halfThickness));
    painter->drawRect(QRectF(-halfThickness, -halfThickness , halfSquareBreadth + halfThickness, halfThickness * 2));

    if (m_runMode == Running && m_displaySettings->getCurrentQuantity() == Current)
    {
        ComponentDI::paintArrow(painter, {0., 0.25 * m_squareBreadth}, m_squareBreadth * 0.1, color, m_NLsolver->getCurrent(m_edge) > 0. ? 1 : 3);
        ComponentDI::paintArrow(painter, {0.25 * m_squareBreadth, 0.}, m_squareBreadth * 0.1, color, m_NLsolver->getCurrent(m_edge) > 0. ? 0 : 2);
    }
}

std::vector<int *> CornerWireDI::getAllInnerNodes()
{
    return std::vector<int *>();
}

void CornerWireDI::resetNodes()
{
    m_rightNode = -1;
    m_bottomNode = -1;
}

std::vector<NLSolverElement> CornerWireDI::getNLSolverElements()
{
    std::vector<NLSolverElement> res;
    std::vector<unsigned> nodes;
    nodes.push_back(m_bottomNode); nodes.push_back(m_rightNode);
    std::vector<unsigned> edges;
    edges.push_back(m_edge);
    DoubleFunction equation = [](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return nodePotentials[1] - nodePotentials[0];};
    std::vector<DoubleFunction> edgeDerivatives, nodeDerivatives;
    edgeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return 0.;});
    nodeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return -1.;});
    nodeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return 1.;});
    res.emplace_back(edges, nodes, equation, edgeDerivatives, nodeDerivatives);

    return res;
}

int *CornerWireDI::getNodeAtInternal(int direction)
{
    if (direction == 2)
    {
        return &m_bottomNode;
    }
    else if (direction == 1)
    {
        return &m_rightNode;
    }
    return nullptr;
}

std::vector<std::pair<int *, SolverEdge> > CornerWireDI::getAllEdges()
{
    std::vector<std::pair<int*, SolverEdge>> res;
    res.emplace_back(&m_edge, SolverEdge(m_bottomNode, m_rightNode, 0., 0.));
    return res;
}

void CornerWireDI::resetEdges()
{
    m_edge = -1;
}
