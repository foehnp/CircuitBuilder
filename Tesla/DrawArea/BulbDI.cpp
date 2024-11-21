#include "BulbDI.h"

#include <Scale/ScaleCollection.h>

#include <QPainter>

BulbDI::BulbDI(double squareBreadth, double thickness, int orientation) :
    ComponentDI(squareBreadth, orientation)
{
    m_userParams.emplace_back("Resistance", 10., 0.01, 1000, 2, "Ohm");
}

void BulbDI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfBreadth = 0.5 * m_squareBreadth;
    double wireThickness = m_squareBreadth * m_wireThickness;
    double radius = m_squareBreadth * m_radius;

    QColor leftColor, rightColor, bulbColor;
    if (m_runMode == Drawing)
    {
        leftColor = rightColor = Qt::black;
        bulbColor = Qt::white;
    }
    else
    {
        int r = 255 - (int)(std::min(1., m_userParams[0].val * 5. * std::pow(m_NLsolver->getCurrent(m_edge), 2)) * 255);
        bulbColor = QColor(255, 255, r);

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
    painter->drawRect(QRectF(-halfBreadth, - wireThickness*0.5, halfBreadth - radius, wireThickness));
    painter->setBrush(rightColor);
    painter->drawRect(QRectF(radius, - wireThickness*0.5, halfBreadth - radius, wireThickness));

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(bulbColor);
    QPen pen(Qt::black);
    pen.setWidthF(wireThickness);
    painter->setPen(pen);
    painter->drawEllipse(QRectF(-radius, -radius, 2*radius, 2*radius));
}

std::vector<int *> BulbDI::getAllInnerNodes()
{
    return std::vector<int *>();
}

void BulbDI::resetNodes()
{
    m_leftOuterNode = -1;
    m_rightOuterNode = -1;
}

std::vector<std::pair<int *, SolverEdge> > BulbDI::getAllEdges()
{
    std::vector<std::pair<int*, SolverEdge>> res;
    res.emplace_back(&m_edge, SolverEdge(m_leftOuterNode, m_rightOuterNode, 10., 0.));
    return res;
}

void BulbDI::resetEdges()
{
    m_edge = -1;
}

std::vector<NLSolverElement> BulbDI::getNLSolverElements()
{
    std::vector<NLSolverElement> res;
    std::vector<unsigned> nodes;
    nodes.push_back(m_leftOuterNode); nodes.push_back(m_rightOuterNode);
    std::vector<unsigned> edges;
    edges.push_back(m_edge);
    double R = m_userParams[0].val;
    DoubleFunction equation = [R](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return nodePotentials[1] - nodePotentials[0] - edgeCurrents[0] * R;};
    std::vector<DoubleFunction> edgeDerivatives, nodeDerivatives;
    edgeDerivatives.push_back([R](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return -R;});
    nodeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return -1.;});
    nodeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return 1.;});
    res.emplace_back(edges, nodes, equation, edgeDerivatives, nodeDerivatives, "Bulb");

    return res;
}

int *BulbDI::getNodeAtInternal(int direction)
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
