#ifndef BJTDI_H
#define BJTDI_H

#include "ComponentDI.h"

class BJTDI : public ComponentDI
{
public:
    BJTDI(double squareBreadth, double thickness, int orientation);

    std::vector<int*> getAllInnerNodes() override;

    void resetNodes() override;

    std::vector<std::pair<int*, SolverEdge>> getAllEdges() override;
    void resetEdges() override;

    std::vector<NLSolverElement> getNLSolverElements() override;

    ComponentName getComponentName() const override
    {
        return BJT;
    }

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int* getNodeAtInternal(int direction) override;

private:
    double m_thickness = 0.1;

    int m_emitterNode = -1; //left
    int m_collectorNode = -1; // right
    int m_baseNode = -1; // bottom

    int m_emitterEdge = -1;
    int m_collectorEdge = -1;
};

#endif // BJTDI_H
