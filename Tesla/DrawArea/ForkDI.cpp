#include "ForkDI.h"

#include <Scale/ScaleCollection.h>

#include <QBrush>
#include <QPen>
#include <QPainter>


ForkDI::ForkDI(double squareBreadth, double thickness, int orientation) :
    ComponentDI(squareBreadth, orientation),
    m_thickness(thickness)
{
}

void ForkDI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfThickness = 0.5 * m_thickness * m_squareBreadth;
    double halfSquareBreadth = 0.5 * m_squareBreadth;

    QColor leftColor, upperColor, lowerColor;
    if (m_runMode == Drawing)
    {
        leftColor = upperColor = lowerColor = Qt::black;
    }
    else
    {
        if (m_displaySettings->getCurrentQuantity() == Potential)
        {
            double potential = m_NLsolver->getPotential(m_leftNode);
            leftColor = m_displaySettings->getColorFromCurrentScale(potential);
            upperColor = m_displaySettings->getColorFromCurrentScale(potential);
            lowerColor = m_displaySettings->getColorFromCurrentScale(potential);
        }
        else if (m_displaySettings->getCurrentQuantity() == Current)
        {
            leftColor = m_displaySettings->getColorFromCurrentScale(std::abs(m_NLsolver->getCurrent(m_leftEdge)));
            upperColor = m_displaySettings->getColorFromCurrentScale(std::abs(m_NLsolver->getCurrent(m_upperEdge)));
            lowerColor = m_displaySettings->getColorFromCurrentScale(std::abs(m_NLsolver->getCurrent(m_lowerEdge)));
        }
    }

    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->setBrush(leftColor);
    painter->drawRect(QRectF(-halfSquareBreadth, 0. - halfThickness, halfSquareBreadth, 2*halfThickness));
    painter->setBrush(upperColor);
    painter->drawRect(QRectF(-halfThickness, -halfSquareBreadth, 2*halfThickness, halfSquareBreadth));
    painter->setBrush(lowerColor);
    painter->drawRect(QRectF(-halfThickness, 0., 2*halfThickness, halfSquareBreadth));


    if (m_runMode == Running && m_displaySettings->getCurrentQuantity() == Current)
    {
        ComponentDI::paintArrow(painter, {-0.25 * m_squareBreadth, -0.}, m_squareBreadth * 0.1, leftColor, m_NLsolver->getCurrent(m_leftEdge) > 0. ? 0 : 2);
        ComponentDI::paintArrow(painter, { 0., -0.25 * m_squareBreadth}, m_squareBreadth * 0.1, upperColor, m_NLsolver->getCurrent(m_upperEdge) > 0. ? 1 : 3);
        ComponentDI::paintArrow(painter, {0., 0.25 * m_squareBreadth}, m_squareBreadth * 0.1, lowerColor, m_NLsolver->getCurrent(m_lowerEdge) > 0. ? 3 : 1);
    }

    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->setBrush(Qt::black);
    painter->drawEllipse({0., 0.}, 2.5 * halfThickness, 2.5 * halfThickness);
}

std::vector<int *> ForkDI::getAllInnerNodes()
{
    std::vector<int *> retVal;
    retVal.push_back(&m_midNode);
    return retVal;
}

int *ForkDI::getNodeAtInternal(int direction)
{
    if (direction == 3)
    {
        return &m_leftNode;
    }
    else if (direction == 0)
    {
        return &m_upperNode;
    }
    else if (direction == 2)
    {
        return &m_lowerNode;
    }
    return nullptr;
}

void ForkDI::resetNodes()
{
    m_leftNode = -1;
    m_upperNode = -1;
    m_lowerNode = -1;
    m_midNode = -1;
}

std::vector<std::pair<int *, SolverEdge> > ForkDI::getAllEdges()
{
    std::vector<std::pair<int*, SolverEdge>> res;
    res.emplace_back(&m_leftEdge, SolverEdge(m_leftNode, m_midNode, 0., 0.));
    res.emplace_back(&m_upperEdge, SolverEdge(m_midNode, m_upperNode, 0., 0.));
    res.emplace_back(&m_lowerEdge, SolverEdge(m_midNode, m_lowerNode, 0., 0.));
    return res;
}

void ForkDI::resetEdges()
{
    m_leftEdge = -1;
    m_upperEdge = -1;
    m_lowerEdge = -1;
}

std::vector<NLSolverElement> ForkDI::getNLSolverElements()
{
    std::vector<NLSolverElement> res;

    {
        std::vector<unsigned> nodes;
        nodes.push_back(m_leftNode); nodes.push_back(m_midNode);
        std::vector<unsigned> edges;
        edges.push_back(m_leftEdge);
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
    }

    {
        std::vector<unsigned> nodes;
        nodes.push_back(m_upperNode); nodes.push_back(m_midNode);
        std::vector<unsigned> edges;
        edges.push_back(m_upperEdge);
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
    }

    {
        std::vector<unsigned> nodes;
        nodes.push_back(m_lowerNode); nodes.push_back(m_midNode);
        std::vector<unsigned> edges;
        edges.push_back(m_lowerEdge);
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
    }

    return res;
}

