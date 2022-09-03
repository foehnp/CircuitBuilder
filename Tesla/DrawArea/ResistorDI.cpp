#include "ResistorDI.h"

#include <Scale/ScaleCollection.h>

#include <QPainter>
#include <QLinearGradient>

ResistorDI::ResistorDI(double squareBreadth, double thickness, int orientation) :
    ComponentDI(squareBreadth, orientation)
{

}

void ResistorDI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    double halfBreadth = 0.5 * m_squareBreadth;
//    double wireThickness = m_squareBreadth * m_wireThickness;
//    double radius = m_squareBreadth * m_radius;

//    QColor leftColor, rightColor;
//    if (m_runMode == Drawing)
//    {
//        leftColor = rightColor = Qt::black;
//    }
//    else
//    {

//        if (m_displaySettings->getCurrentQuantity() == Potential)
//        {
//            leftColor = m_displaySettings->getColorFromCurrentScale(m_solver->getPotential(m_leftOuterNode));
//            rightColor = m_displaySettings->getColorFromCurrentScale(m_solver->getPotential(m_rightOuterNode));
//            QLinearGradient gradient()
//        }
//        else if (m_displaySettings->getCurrentQuantity() == Current)
//        {
//            leftColor = rightColor = m_displaySettings->getColorFromCurrentScale(std::abs(m_solver->getCurrent(m_edge)));
//        }
//    }

//    painter->setPen(QPen(Qt::PenStyle::NoPen));
//    painter->setBrush(leftColor);
//    painter->drawRect(QRectF(-halfBreadth, - wireThickness*0.5, halfBreadth - radius, wireThickness));
//    painter->setBrush(rightColor);
//    painter->drawRect(QRectF(radius, - wireThickness*0.5, halfBreadth - radius, wireThickness));

//    painter->setRenderHint(QPainter::Antialiasing);
//    painter->setBrush(bulbColor);
//    QPen pen(Qt::black);
//    pen.setWidthF(wireThickness);
//    painter->setPen(Qt::black);
//    painter->drawEllipse(QRectF(-radius, -radius, 2*radius, 2*radius));
}

std::vector<int *> ResistorDI::getAllInnerNodes()
{
    return std::vector<int *>();
}

void ResistorDI::resetNodes()
{
    m_leftOuterNode = -1;
    m_rightOuterNode = -1;
}

std::vector<std::pair<int *, SolverEdge> > ResistorDI::getAllEdges()
{
    std::vector<std::pair<int*, SolverEdge>> res;
    res.emplace_back(&m_edge, SolverEdge(m_leftOuterNode, m_rightOuterNode, 10., 0.));
    return res;
}

void ResistorDI::resetEdges()
{
    m_edge = -1;
}

std::vector<NLSolverElement> ResistorDI::getNLSolverElements()
{
    std::vector<NLSolverElement> res;
    std::vector<unsigned> nodes;
    nodes.push_back(m_leftOuterNode); nodes.push_back(m_rightOuterNode);
    std::vector<unsigned> edges;
    edges.push_back(m_edge);
    DoubleFunction equation = [](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return nodePotentials[1] - nodePotentials[0] - edgeCurrents[0] * 10.;};
    std::vector<DoubleFunction> edgeDerivatives, nodeDerivatives;
    edgeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return -10.;});
    nodeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return -1.;});
    nodeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                            {return 1.;});
    res.emplace_back(edges, nodes, equation, edgeDerivatives, nodeDerivatives);

    return res;
}

int *ResistorDI::getNodeAtInternal(int direction)
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
