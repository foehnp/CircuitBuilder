#pragma once

#include "ComponentDI.h"

class ForkDI : public ComponentDI
{
public:
    ForkDI(double squareBreadth, double thickness, int orientation);

    std::vector<int*> getAllInnerNodes() override;

    void resetNodes() override;

    std::vector<std::pair<int*, SolverEdge>> getAllEdges() override;
    void resetEdges() override;

    std::vector<NLSolverElement> getNLSolverElements() override;

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int* getNodeAtInternal(int direction) override;

private:
    double m_thickness = 0.1;

    int m_leftNode = -1;
    int m_upperNode = -1;
    int m_lowerNode = -1;

    int m_midNode = -1;

    int m_leftEdge = -1;
    int m_upperEdge = -1;
    int m_lowerEdge = -1;
};
