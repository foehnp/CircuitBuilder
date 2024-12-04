#include "GroundDI.h"

#include <Scale/ScaleCollection.h>

#include <QPen>
#include <QPainter>

GroundDI::GroundDI(double squareBreadth, double thickness, int orientation) :
    ComponentDI(squareBreadth, orientation),
    m_wireThickness(thickness)
{
}

void GroundDI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double thickness = m_wireThickness * m_squareBreadth;
    double halfThickness = 0.5 * thickness;
    double halfSquareBreadth = 0.5 * m_squareBreadth;
    double wireHeight = 0.45 * m_squareBreadth;
    double lineLength0 = 0.5 * m_squareBreadth;
    double lineLength1 = 0.35 * m_squareBreadth;
    double lineLength2 = 0.2 * m_squareBreadth;
    double interLineGap = thickness;

    QColor color;
    if (m_runMode == Drawing)
    {
        color = Qt::black;
    }
    else
    {
        if (m_displaySettings->getCurrentQuantity() == Potential)
        {
            color = m_displaySettings->getColorFromCurrentScale(m_NLsolver->getPotential(m_innerNode));
        }
        else if (m_displaySettings->getCurrentQuantity() == Current)
        {
            color = m_displaySettings->getColorFromCurrentScale(std::abs(m_NLsolver->getCurrent(m_edge)));
        }
    }


    painter->setBrush(color);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    // Wire
    painter->drawRect(QRectF(-halfThickness, -halfSquareBreadth, thickness, wireHeight));
    // Line 0
    painter->drawRect(QRectF(-0.5 * lineLength0, -halfSquareBreadth + wireHeight, lineLength0, thickness));
    // Line 1
    painter->drawRect(QRectF(-0.5 * lineLength1, -halfSquareBreadth + wireHeight + thickness + interLineGap, lineLength1, thickness));
    // Line 2
    painter->drawRect(QRectF(-0.5 * lineLength2, -halfSquareBreadth + wireHeight + 2*thickness + 2*interLineGap, lineLength2, thickness));
}

std::vector<int *> GroundDI::getAllInnerNodes()
{
    std::vector<int *> res;
    res.push_back(&m_innerNode);
    return res;
}

void GroundDI::resetNodes()
{
    m_innerNode = -1;
    m_outerNode = -1;
}

std::vector<std::pair<int *, SolverEdge> > GroundDI::getAllEdges()
{
    std::vector<std::pair<int*, SolverEdge>> res;
    res.emplace_back(&m_edge, SolverEdge(m_outerNode, m_innerNode, 0., 1.));
    return res;
}

void GroundDI::resetEdges()
{
    m_edge = -1;
}

std::vector<NLSolverElement> GroundDI::getNLSolverElements()
{
    std::vector<NLSolverElement> res;
    res.reserve(2);

    {
        std::vector<unsigned> nodes;
        nodes.push_back(m_outerNode); nodes.push_back(m_innerNode);
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
        res.emplace_back(edges, nodes, equation, edgeDerivatives, nodeDerivatives, "GroundEdge");
    }
    {
        std::vector<unsigned> nodes;
        nodes.push_back(m_innerNode);
        std::vector<unsigned> edges;
        DoubleFunction equation = [](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                    {return nodePotentials[0];};
        std::vector<DoubleFunction> edgeDerivatives, nodeDerivatives;
        nodeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return 1.;});
        res.emplace_back(edges, nodes, equation, edgeDerivatives, nodeDerivatives, "GroundCondition");
    }

    return res;
}

int *GroundDI::getNodeAtInternal(int direction)
{
    if (direction == 0)
    {
        return &m_outerNode;
    }
    return nullptr;
}
