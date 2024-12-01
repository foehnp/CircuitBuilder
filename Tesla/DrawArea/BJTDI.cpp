#include "BJTDI.h"

#include <Scale/ScaleCollection.h>

#include <QBrush>
#include <QPen>
#include <QPainter>


BJTDI::BJTDI(double squareBreadth, double thickness, int orientation) :
    ComponentDI(squareBreadth, orientation),
    m_thickness(thickness)
{
}

void BJTDI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double halfThickness = 0.5 * m_thickness * m_squareBreadth;
    double halfSquareBreadth = 0.5 * m_squareBreadth;

    // for the labels
    QFont font = painter->font();
    font.setPointSizeF(7.);
    font.setBold(true);
    painter->setFont(font);

    QColor baseColor, collectorColor, emitterColor;
    if (m_runMode == Drawing)
    {
        baseColor = collectorColor = emitterColor = Qt::black;
    }
    else
    {
        if (m_displaySettings->getCurrentQuantity() == Potential)
        {
            double basePotential = m_NLsolver->getPotential(m_baseNode);
            baseColor = m_displaySettings->getColorFromCurrentScale(basePotential);

            double collectorPotential = m_NLsolver->getPotential(m_collectorNode);
            collectorColor = m_displaySettings->getColorFromCurrentScale(collectorPotential);

            double emitterPotential = m_NLsolver->getPotential(m_emitterNode);
            emitterColor = m_displaySettings->getColorFromCurrentScale(emitterPotential);
        }
        else if (m_displaySettings->getCurrentQuantity() == Current)
        {
            double em_cur = m_NLsolver->getCurrent(m_emitterEdge);
            double co_cur = m_NLsolver->getCurrent(m_collectorEdge);
            double base_cur = em_cur - co_cur;
            baseColor = m_displaySettings->getColorFromCurrentScale(std::abs(base_cur));
            collectorColor = m_displaySettings->getColorFromCurrentScale(std::abs(co_cur));
            emitterColor = m_displaySettings->getColorFromCurrentScale(std::abs(em_cur));
        }
    }


    {
        // Left wire (base)
        painter->setPen(QPen(Qt::PenStyle::NoPen));
        painter->setBrush(baseColor);
        painter->drawRect(QRectF(-halfSquareBreadth, - halfThickness, halfSquareBreadth*0.5 - halfThickness*3., 2*halfThickness));

        painter->setPen(baseColor);
        painter->drawText(QPointF(-m_squareBreadth * 0.45, -m_squareBreadth * 0.05), QString("B"));
    }
    {
        // Semiconductior wire
        painter->setPen(QPen(Qt::PenStyle::NoPen));
        painter->setBrush(Qt::black);
        painter->drawRect(QRectF(-halfSquareBreadth*0.5 - halfThickness*3.,  -halfSquareBreadth * 0.4, halfThickness*2., halfSquareBreadth * 0.8));
    }
    {
        // Top wire
        painter->setPen(QPen(Qt::PenStyle::NoPen));
        painter->setBrush(collectorColor);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawRect(QRectF(-halfThickness,  -halfSquareBreadth, halfThickness*2., halfSquareBreadth * 0.3));
    }
    {
        // Upper diagonal wire (collector)
        painter->setPen(QPen(Qt::PenStyle::NoPen));
        painter->setBrush(collectorColor);
        QVector<QPointF> points({{-halfThickness, -0.7*halfSquareBreadth},
                                 {halfThickness, -0.7*halfSquareBreadth},
                                 {halfThickness, -0.7*halfSquareBreadth + 0.83*halfThickness},
                                 {-halfSquareBreadth*0.5 - halfThickness, -halfSquareBreadth*0.2},
                                 {-halfSquareBreadth*0.5 - halfThickness, -halfSquareBreadth*0.2 - halfThickness*2.83}});
        QPolygonF diagLine(points);
        painter->drawPolygon(diagLine);

        painter->setPen(collectorColor);
        painter->drawText(QPointF(-m_squareBreadth * 0.1, -m_squareBreadth * 0.1), QString("C"));
    }
    {
        // Bottom wire
        painter->setPen(QPen(Qt::PenStyle::NoPen));
        painter->setBrush(emitterColor);
        painter->setRenderHint(QPainter::Antialiasing);
        painter->drawRect(QRectF(-halfThickness,  halfSquareBreadth, halfThickness*2., -halfSquareBreadth * 0.3));
    }
    {
        // Lower diagonal wire (emitter)
        painter->setPen(QPen(Qt::PenStyle::NoPen));
        painter->setBrush(emitterColor);
        QVector<QPointF> points({{-halfThickness, 0.7*halfSquareBreadth},
                                 {halfThickness, 0.7*halfSquareBreadth},
                                 {halfThickness, 0.7*halfSquareBreadth - 0.83*halfThickness},
                                 {-halfSquareBreadth*0.5 - halfThickness, halfSquareBreadth*0.2},
                                 {-halfSquareBreadth*0.5 - halfThickness, halfSquareBreadth*0.2 + halfThickness*2.83}});
        QPolygonF diagLine(points);
        painter->drawPolygon(diagLine);

        painter->setPen(emitterColor);
        painter->drawText(QPointF(-m_squareBreadth * 0.1, m_squareBreadth * 0.2), QString("E"));
    }


    if (m_runMode == Running && m_displaySettings->getCurrentQuantity() == Current)
    {
//        ComponentDI::paintArrow(painter, {-0.25 * m_squareBreadth, -0.}, m_squareBreadth * 0.1, leftColor, m_NLsolver->getCurrent(m_leftEdge) > 0. ? 0 : 2);
//        ComponentDI::paintArrow(painter, { 0., -0.25 * m_squareBreadth}, m_squareBreadth * 0.1, lowerColor, m_NLsolver->getCurrent(m_upperEdge) > 0. ? 1 : 3);
//        ComponentDI::paintArrow(painter, {0., 0.25 * m_squareBreadth}, m_squareBreadth * 0.1, upperColor, m_NLsolver->getCurrent(m_lowerEdge) > 0. ? 3 : 1);
    }
}

std::vector<int *> BJTDI::getAllInnerNodes()
{
    return {};
}

int *BJTDI::getNodeAtInternal(int direction)
{
    if (direction == 2)
    {
        return &m_emitterNode;
    }
    else if (direction == 0)
    {
        return &m_collectorNode;
    }
    else if (direction == 3)
    {
        return &m_baseNode;
    }
    return nullptr;
}

void BJTDI::resetNodes()
{
    m_emitterNode = -1;
    m_collectorNode = -1;
    m_baseNode = -1;
}

std::vector<std::pair<int *, SolverEdge> > BJTDI::getAllEdges()
{
    std::vector<std::pair<int*, SolverEdge>> res;
    res.emplace_back(&m_emitterEdge, SolverEdge(m_baseNode, m_emitterNode, 0., 0.));
    res.emplace_back(&m_collectorEdge, SolverEdge(m_collectorNode, m_baseNode, 0., 0.));
    return res;
}

void BJTDI::resetEdges()
{
    m_emitterEdge = -1;
    m_collectorEdge = -1;
}

std::vector<NLSolverElement> BJTDI::getNLSolverElements()
{
    std::vector<NLSolverElement> res;
    res.reserve(2);

    const double b_F = 100.;
    const double b_R = 5.;

    const double I_S = 1e-13;
    const double log_I_S = log(I_S);
    const double V_T = 0.025;
    {
        // Eq for emitter current
        std::vector<unsigned> nodes;
        nodes.push_back(m_baseNode); nodes.push_back(m_emitterNode); nodes.push_back(m_collectorNode);
        std::vector<unsigned> edges;
        edges.push_back(m_emitterEdge);

        DoubleFunction equation = [V_T, b_F, I_S](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                    {return I_S * ( (1+1/b_F) * exp((nodePotentials[1] - nodePotentials[0])/V_T)
                                    -1
                                    -exp((nodePotentials[2] - nodePotentials[0])/V_T))
                                    -edgeCurrents[0];};

        std::vector<DoubleFunction> edgeDerivatives, nodeDerivatives;
        edgeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return -1.;});

        nodeDerivatives.push_back([V_T, b_F, I_S](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return I_S/V_T * ( -(1+1/b_F) * exp((nodePotentials[1] - nodePotentials[0])/V_T)
                                +exp((nodePotentials[2] - nodePotentials[0])/V_T));});

        nodeDerivatives.push_back([V_T, b_F, I_S](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return I_S/V_T * ( (1+1/b_F) * exp((nodePotentials[1] - nodePotentials[0])/V_T));});

        nodeDerivatives.push_back([V_T, b_F, I_S](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return I_S/V_T * (-exp((nodePotentials[2] - nodePotentials[0])/V_T));});

        res.emplace_back(edges, nodes, equation, edgeDerivatives, nodeDerivatives, "BJT_Em");
    }
    {
        // Eq for collector current
        std::vector<unsigned> nodes;
        nodes.push_back(m_baseNode); nodes.push_back(m_emitterNode); nodes.push_back(m_collectorNode);
        std::vector<unsigned> edges;
        edges.push_back(m_collectorEdge);

        DoubleFunction equation = [V_T, b_R, I_S](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                    {return I_S * (exp((nodePotentials[1] - nodePotentials[0])/V_T)
                                    + 1
                                    - (1+1/b_R) * exp((nodePotentials[2] - nodePotentials[0])/V_T))
                                    - edgeCurrents[0];};

        std::vector<DoubleFunction> edgeDerivatives, nodeDerivatives;
        edgeDerivatives.push_back([](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return -1.;});

        nodeDerivatives.push_back([V_T, b_R, I_S](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return I_S/V_T * (-exp((nodePotentials[1] - nodePotentials[0])/V_T)
                                + (1+1/b_R) * exp((nodePotentials[2] - nodePotentials[0])/V_T));});

        nodeDerivatives.push_back([V_T, b_R, I_S](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return I_S/V_T * (exp((nodePotentials[1] - nodePotentials[0])/V_T));});

        nodeDerivatives.push_back([V_T, b_R, I_S](std::vector<double> edgeCurrents, std::vector<double> nodePotentials)
                                {return -I_S /V_T * (1+1/b_R) * exp((nodePotentials[2] - nodePotentials[0])/V_T);});

        res.emplace_back(edges, nodes, equation, edgeDerivatives, nodeDerivatives, "BJT_Col");
    }

    return res;
}
