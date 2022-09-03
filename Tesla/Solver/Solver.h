#pragma once

#include <vector>
#include <utility>

struct SolverEdge
{
    unsigned lNode;
    unsigned rNode;
    double resistance;
    double voltage;

    SolverEdge(unsigned lNode, unsigned rNode, double resistance, double voltage) :
        lNode(lNode),
        rNode(rNode),
        resistance(resistance),
        voltage(voltage)
    {}
};

struct SolverNode
{
    std::vector<std::pair<int, unsigned>> incidentEdges;
};

class Solver
{
public:
    Solver();

    unsigned addEdge(unsigned lNode, unsigned rNode, double resistance, double voltage);

    unsigned addNode();

    void compute();

    double getPotential(int i) const;
    double getCurrent(int i) const;

private:
    std::vector<SolverNode> m_nodes;
    std::vector<SolverEdge> m_edges;

    std::vector<double> m_potentials;
    std::vector<double> m_currents;
};
