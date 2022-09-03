#include "EmptyComponentDI.h"

EmptyComponentDI::EmptyComponentDI(double squareBreadth, int orientation) :
    ComponentDI(squareBreadth, orientation)
{
}

std::vector<int *> EmptyComponentDI::getAllInnerNodes()
{
    return std::vector<int *>();
}

int *EmptyComponentDI::getNodeAtInternal(int direction)
{
    return nullptr;
}

std::vector<std::pair<int *, SolverEdge> > EmptyComponentDI::getAllEdges()
{
    return std::vector<std::pair<int*, SolverEdge>>();
}

void EmptyComponentDI::resetEdges()
{

}

std::vector<NLSolverElement> EmptyComponentDI::getNLSolverElements()
{
    return {};
}

void EmptyComponentDI::paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}
