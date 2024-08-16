#ifndef WIREDI_H
#define WIREDI_H

#include "ComponentDI.h"

class WireDI : public ComponentDI
{
public:
    WireDI(double squareBreadth, double thickness, int orientation);

    std::vector<int*> getAllInnerNodes() override;

    void resetNodes() override;

    std::vector<std::pair<int*, SolverEdge>> getAllEdges() override;
    void resetEdges() override;

    std::vector<NLSolverElement> getNLSolverElements() override;

    ComponentName getComponentName() const override
    {
        return Wire;
    }

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int* getNodeAtInternal(int direction) override;

private:
    double m_thickness = 0.1;

    int m_leftNode = -1;
    int m_rightNode = -1;

    int m_edge = -1;
};

#endif // WIREDI_H
