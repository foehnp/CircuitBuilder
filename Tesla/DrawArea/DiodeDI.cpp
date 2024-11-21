#include "DiodeDI.h"

#include <Scale/ScaleCollection.h>

#include <QBrush>
#include <QPen>
#include <QPainter>

#include <math.h>


DiodeDI::DiodeDI(double squareBreadth, double thickness, int orientation) :
    ComponentDI(squareBreadth, orientation),
    m_thickness(thickness)
{
}

void DiodeDI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfThickness = 0.5 * m_thickness * m_squareBreadth;
    double halfSquareBreadth = 0.5 * m_squareBreadth;

    QColor leftColor, rightColor;
    if (m_runMode == Drawing)
    {
        leftColor = rightColor = Qt::black;
    }
    else
    {
        if (m_displaySettings->getCurrentQuantity() == Potential)
        {
            leftColor = m_displaySettings->getColorFromCurrentScale(m_NLsolver->getPotential(m_leftNode));
            rightColor = m_displaySettings->getColorFromCurrentScale(m_NLsolver->getPotential(m_rightNode));
        }
        else if (m_displaySettings->getCurrentQuantity() == Current)
        {
            leftColor = rightColor = m_displaySettings->getColorFromCurrentScale(std::abs(m_NLsolver->getCurrent(m_edge)));
        }
    }

    painter->setBrush(leftColor);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->drawRect(QRectF(-halfSquareBreadth, -halfThickness, halfSquareBreadth, 2.*halfThickness));

    painter->setBrush(rightColor);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->drawRect(QRectF(0., -halfThickness, halfSquareBreadth, 2*halfThickness));

    painter->setBrush(Qt::black);;
    painter->setPen(QPen(Qt::PenStyle::NoPen));

    QVector<QPointF> trianglePoints({{-0.3*halfSquareBreadth, -0.4*halfSquareBreadth},
                            {-0.3*halfSquareBreadth, 0.4*halfSquareBreadth},
                            {0.3*halfSquareBreadth, 0.}});
    QPolygonF triangle(trianglePoints);
    painter->drawPolygon(triangle);

    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->drawRect(QRectF(0.3*halfSquareBreadth, -0.4*halfSquareBreadth, 2.*halfThickness, 0.8*halfSquareBreadth));

}

std::vector<int *> DiodeDI::getAllInnerNodes()
{
    return std::vector<int *>();
}

int *DiodeDI::getNodeAtInternal(int direction)
{
    if (direction == 3)
    {
        return &m_leftNode;
    }
    else if (direction == 1)
    {
        return &m_rightNode;
    }
    return nullptr;
}

void DiodeDI::resetNodes()
{
    m_leftNode = -1;
    m_rightNode = -1;
}

std::vector<std::pair<int *, SolverEdge> > DiodeDI::getAllEdges()
{
    std::vector<std::pair<int*, SolverEdge>> res;
    res.emplace_back(&m_edge, SolverEdge(m_leftNode, m_rightNode, 0., 0.));
    return res;
}

void DiodeDI::resetEdges()
{
    m_edge = -1;
}

std::vector<NLSolverElement> DiodeDI::getNLSolverElements()
{
    std::vector<NLSolverElement> res;
    std::vector<unsigned> nodes;
    nodes.push_back(m_leftNode); nodes.push_back(m_rightNode);
    std::vector<unsigned> edges;
    edges.push_back(m_edge);

    const double reverseCurrent = 0.01;
    const double thermalVoltage = 0.025;

    DoubleFunction equation = [reverseCurrent, thermalVoltage](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return (exp((nodePotentials[1] - nodePotentials[0])/thermalVoltage) - 1) *  reverseCurrent - edgeCurrents[0];};

    std::vector<DoubleFunction> edgeDerivatives, nodeDerivatives;

    edgeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials){return -1.;});

    nodeDerivatives.push_back([reverseCurrent, thermalVoltage](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return -exp((nodePotentials[1] - nodePotentials[0])/thermalVoltage) * reverseCurrent / thermalVoltage;});

    nodeDerivatives.push_back([reverseCurrent, thermalVoltage](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return exp((nodePotentials[1] - nodePotentials[0])/thermalVoltage) * reverseCurrent / thermalVoltage;});

    res.emplace_back(edges, nodes, equation, edgeDerivatives, nodeDerivatives, "Diode");

    return res;
}
