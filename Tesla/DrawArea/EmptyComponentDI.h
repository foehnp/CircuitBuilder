#pragma once

#include "ComponentDI.h"

class EmptyComponentDI : public ComponentDI
{
public:
    EmptyComponentDI(double squareBreadth, int orientation);

    std::vector<int*> getAllInnerNodes() override;
    void resetNodes() override {}

    std::vector<std::pair<int*, SolverEdge>> getAllEdges() override;
    void resetEdges() override;

    std::vector<NLSolverElement> getNLSolverElements() override;


    ComponentName getComponentName() const override
    {
        return EmptyComponent;
    }

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int* getNodeAtInternal(int direction) override;
};
