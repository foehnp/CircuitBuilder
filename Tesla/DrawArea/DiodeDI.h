#ifndef DIODEDI_H
#define DIODEDI_H

#include "ComponentDI.h"

class DiodeDI : public ComponentDI
{
public:
    DiodeDI(double squareBreadth, double thickness, int orientation);

    std::vector<int*> getAllInnerNodes() override;

    void resetNodes() override;

    std::vector<std::pair<int*, SolverEdge>> getAllEdges() override;
    void resetEdges() override;

    std::vector<NLSolverElement> getNLSolverElements() override;

    ComponentName getComponentName() const override
    {
        return Diode;
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

#endif // DIODEDI_H
