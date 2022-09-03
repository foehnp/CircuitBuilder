#include "Solver.h"

#include <Eigen/Dense>

#include <QDebug>

Solver::Solver()
{

}

unsigned Solver::addEdge(unsigned lNode, unsigned rNode, double resistance, double voltage)
{
    unsigned index = m_edges.size();
    m_edges.emplace_back(lNode, rNode, resistance, voltage);
    SolverNode& lSolverNode = m_nodes[lNode];
    SolverNode& rSolverNode = m_nodes[rNode];
    lSolverNode.incidentEdges.emplace_back(-1, index);
    rSolverNode.incidentEdges.emplace_back( 1, index);
    return index;
}

unsigned Solver::addNode()
{
    m_nodes.emplace_back();
    return m_nodes.size() - 1;
}

void Solver::compute()
{
    m_currents.clear();
    m_potentials.clear();
    int numEdges = m_edges.size();
    int numNodes = m_nodes.size();
    int numEqs   = numEdges + numNodes;
    if (numEqs == 0)
    {
        return;
    }
    Eigen::MatrixXd M = Eigen::MatrixXd::Zero(numEqs, numEqs);
    Eigen::VectorXd u = Eigen::VectorXd::Zero(numEqs);
    for (int i=0; i<numEdges; ++i)
    {
        M(i, numEdges + m_edges[i].lNode) = -1;
        M(i, numEdges + m_edges[i].rNode) =  1;
        if (m_edges[i].resistance != nan(0))
        {
            M(i, i) = m_edges[i].resistance;
        }
        u(i) = m_edges[i].voltage;
    }
    for (int i=0; i<numNodes; ++i)
    {
        int rowIdx = numEdges + i;
        for (const auto& edge : m_nodes[i].incidentEdges)
        {
            M(rowIdx, edge.second) = edge.first;
        }
    }

//    for (int i = 0; i < numEqs; ++i)
//    {
//        std::string debugString;
//        for (int j = 0; j < numEqs; ++j)
//        {
//            debugString += std::to_string(M(i,j)) + ",";
//        }
//        qDebug() << debugString.c_str() << " || " << std::to_string(u(i)).c_str();
//    }

    Eigen::VectorXd res = M.colPivHouseholderQr().solve(u);

    m_currents.reserve(numEdges);
    for (int i=0; i<numEdges; ++i)
    {
        m_currents.push_back(res[i]);
    }
    m_potentials.reserve(numNodes);
    for (int i=0; i<numNodes; ++i)
    {
        m_potentials.push_back(res[numEdges+i]);
    }
}

double Solver::getPotential(int i) const
{
    return m_potentials[i];
}

double Solver::getCurrent(int i) const
{
    return m_currents[i];
}
