#pragma once

#include <DrawArea/ComponentDI.h>

class QPainter;

class GroundDI : public ComponentDI
{
public:
    GroundDI(double squareBreadth, double thickness, int orientation);

    std::vector<int*> getAllInnerNodes() override;
    void resetNodes() override;

    std::vector<std::pair<int*, SolverEdge>> getAllEdges() override;
    void resetEdges() override;

    std::vector<NLSolverElement> getNLSolverElements() override;

    std::vector<int> getNonConservativeNodes() override
    {
        std::vector<int> res;
        res.push_back(m_innerNode);
        return res;
    }

    ComponentName getComponentName() const override
    {
        return Ground;
    }

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int* getNodeAtInternal(int direction) override;

private:
    double m_wireThickness = 0.05;
    double m_gapThickness = 0.1;

    int m_innerNode = -1;
    int m_outerNode = -1;

    int m_edge = -1;
};
