#pragma once

#include <vector>
#include <functional>

//typedef double (*DoubleFunction)(std::vector<double>, std::vector<double>);
typedef std::function<double(std::vector<double>, std::vector<double>)> DoubleFunction;

struct NLSolverElement
{
    std::vector<unsigned> edges;
    std::vector<unsigned> nodes;
    DoubleFunction equation;
    std::vector<DoubleFunction> edgeDerivatives;
    std::vector<DoubleFunction> nodeDerivatives;
    std::string debugName;

    NLSolverElement(std::vector<unsigned> edges,
                    std::vector<unsigned> nodes,
                    DoubleFunction equation,
                    std::vector<DoubleFunction> edgeDerivatives,
                    std::vector<DoubleFunction> nodeDerivatives,
                    std::string debugName = "") :
        nodes(nodes),
        edges(edges),
        equation(equation),
        edgeDerivatives(edgeDerivatives),
        nodeDerivatives(nodeDerivatives),
        debugName(debugName)
    {}

};

struct NLSolverEdge
{
    unsigned lNode;
    unsigned rNode;

    NLSolverEdge(unsigned lNode, unsigned rNode) :
        lNode(lNode),
        rNode(rNode)
    {}
};


struct NLSolverNode
{
    std::vector<std::pair<int, unsigned>> incidentEdges;
};

class NLSolver
{
public:
    NLSolver();

    unsigned addNode();

    unsigned addEdge(unsigned lNode, unsigned rNode);

    void addElement(const NLSolverElement& element);

    void compute();

    double getPotential(int i) const;
    double getCurrent(int i) const;

private:
    std::vector<NLSolverNode> m_nodes;
    std::vector<NLSolverEdge> m_edges;
    std::vector<NLSolverElement> m_solverElements;

    std::vector<double> m_potentials;
    std::vector<double> m_currents;

    double m_convergenceTolerance = 0.00001;
};
