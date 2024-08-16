#pragma once

#include "ComponentDI.h"

class BulbDI : public ComponentDI
{
public:
    BulbDI(double squareBreadth, double thickness, int orientation);

    std::vector<int*> getAllInnerNodes() override;
    void resetNodes() override;

    std::vector<std::pair<int*, SolverEdge>> getAllEdges() override;
    void resetEdges() override;

    std::vector<NLSolverElement> getNLSolverElements() override;

    ComponentName getComponentName() const override
    {
        return Bulb;
    }

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int* getNodeAtInternal(int direction) override;

private:
    double m_wireThickness = 0.05;
    double m_radius = 0.25;


    int m_leftOuterNode = -1;
    int m_rightOuterNode = -1;

    int m_edge = -1;
};
