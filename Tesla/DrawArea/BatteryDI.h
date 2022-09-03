#pragma once

#include <DrawArea/ComponentDI.h>

class QPainter;

class BatteryDI : public ComponentDI
{
public:
    BatteryDI(double squareBreadth, double thickness, int orientation);

    std::vector<int*> getAllInnerNodes() override;
    void resetNodes() override;

    std::vector<std::pair<int*, SolverEdge>> getAllEdges() override;
    void resetEdges() override;

    std::vector<NLSolverElement> getNLSolverElements() override;

private:
    void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int* getNodeAtInternal(int direction) override;

private:
    double m_wireThickness = 0.05;
    double m_gapThickness = 0.1;
    double m_leftHeight = 0.8;
    double m_rightHeigt = 0.3;


    int m_leftOuterNode = -1;
    int m_rightOuterNode = -1;

    int m_edge = -1;
};
