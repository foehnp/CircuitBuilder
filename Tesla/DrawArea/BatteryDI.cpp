#include "BatteryDI.h"

#include <Scale/ScaleCollection.h>

#include <QPen>
#include <QPainter>

BatteryDI::BatteryDI(double squareBreadth, double thickness, int orientation) :
    ComponentDI(squareBreadth, orientation),
    m_wireThickness(thickness)
{
    m_userParams.emplace_back("Voltage", 1., 0.001, 1000, 3, "V");
}

void BatteryDI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfBreadth = 0.5 * m_squareBreadth;
    double wireThickness = m_squareBreadth * m_wireThickness;
    double gapThickness = m_squareBreadth * m_gapThickness;
    double leftHeight = m_squareBreadth * m_leftHeight;
    double rightHeight = m_squareBreadth * m_rightHeigt;
    double signHeight = m_squareBreadth * 0.15;
    double leftSignX = -m_squareBreadth * 0.35;
    double rightSignX = m_squareBreadth * 0.1;

    QColor leftColor, rightColor;
    if (m_runMode == Drawing)
    {
        leftColor = rightColor = Qt::black;
    }
    else
    {
        if (m_displaySettings->getCurrentQuantity() == Potential)
        {
            leftColor = m_displaySettings->getColorFromCurrentScale(m_NLsolver->getPotential(m_leftOuterNode));
            rightColor = m_displaySettings->getColorFromCurrentScale(m_NLsolver->getPotential(m_rightOuterNode));
        }
        else if (m_displaySettings->getCurrentQuantity() == Current)
        {
            leftColor = rightColor = m_displaySettings->getColorFromCurrentScale(std::abs(m_NLsolver->getCurrent(m_edge)));
        }
    }

    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->setBrush(leftColor);
    painter->drawRect(QRectF(-halfBreadth, - wireThickness*0.5, halfBreadth - 0.5*gapThickness - wireThickness, wireThickness));
    painter->drawRect(QRectF(-0.5*gapThickness - wireThickness, -leftHeight*0.5, wireThickness, leftHeight));

    painter->setPen(QPen(leftColor));
    QFont font = painter->font();
    font.setPointSizeF(12.);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(QPointF(leftSignX, -signHeight), QString("+"));

    painter->setPen(QPen(Qt::PenStyle::NoPen));
    painter->setBrush(rightColor);
    painter->drawRect(QRectF(0.5*gapThickness + wireThickness, - wireThickness*0.5,  halfBreadth - 0.5*gapThickness - wireThickness, wireThickness));
    painter->drawRect(QRectF(0.5*gapThickness, -rightHeight*0.5, wireThickness, rightHeight));

    painter->setPen(QPen(rightColor));
    painter->drawText(QPointF(rightSignX, -signHeight), QString("-"));
}

std::vector<int *> BatteryDI::getAllInnerNodes()
{
    return std::vector<int *>();
}

void BatteryDI::resetNodes()
{
    m_leftOuterNode = -1;
    m_rightOuterNode = -1;
}

std::vector<std::pair<int *, SolverEdge> > BatteryDI::getAllEdges()
{
    std::vector<std::pair<int*, SolverEdge>> res;
    res.emplace_back(&m_edge, SolverEdge(m_leftOuterNode, m_rightOuterNode, 0., 1.));
    return res;
}

void BatteryDI::resetEdges()
{
    m_edge = -1;
}

std::vector<NLSolverElement> BatteryDI::getNLSolverElements()
{
    std::vector<NLSolverElement> res;

    const double V = m_userParams[0].val;

    std::vector<unsigned> nodes;
    nodes.push_back(m_leftOuterNode); nodes.push_back(m_rightOuterNode);
    std::vector<unsigned> edges;
    edges.push_back(m_edge);
    DoubleFunction equation = [V](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return nodePotentials[1] - nodePotentials[0] - V;};
    std::vector<DoubleFunction> edgeDerivatives, nodeDerivatives;
    edgeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return 0.;});
    nodeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return -1.;});
    nodeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return 1.;});
    res.emplace_back(edges, nodes, equation, edgeDerivatives, nodeDerivatives, "Battery");

    return res;
}

int *BatteryDI::getNodeAtInternal(int direction)
{
    if (direction == 3)
    {
        return &m_leftOuterNode;
    }
    else if (direction == 1)
    {
        return &m_rightOuterNode;
    }
    return nullptr;
}
