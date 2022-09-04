#include "NLSolver.h"

#include <Eigen/Dense>

#include <QDebug>


NLSolver::NLSolver()
{

}

unsigned NLSolver::addNode()
{
    m_nodes.emplace_back();
    return m_nodes.size() - 1;
}


unsigned NLSolver::addEdge(unsigned lNode, unsigned rNode)
{
    unsigned index = m_edges.size();
    m_edges.emplace_back(lNode, rNode);
    NLSolverNode& lSolverNode = m_nodes[lNode];
    NLSolverNode& rSolverNode = m_nodes[rNode];
    lSolverNode.incidentEdges.emplace_back(-1, index);
    rSolverNode.incidentEdges.emplace_back( 1, index);
    return index;
}

void NLSolver::addElement(const NLSolverElement& element)
{
    m_solverElements.emplace_back(element);
}

std::vector<double> getSubVector(const Eigen::VectorXd& allVals, unsigned start, const std::vector<unsigned>& indices)
{
    std::vector<double> retVal;
    retVal.reserve(indices.size());
    for (const unsigned& index : indices)
    {
        retVal.push_back(allVals(start + index));
    }
    return retVal;
}

void NLSolver::compute()
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

    Eigen::VectorXd vals = Eigen::VectorXd::Zero(numEqs);

    while (true)
    {
        // Calculate f(val)
        Eigen::VectorXd y = Eigen::VectorXd::Zero(numEqs);
        for (int i = 0; i < m_solverElements.size(); ++i)
        {
            NLSolverElement& element = m_solverElements[i];
            std::vector<double> edgeArguments = getSubVector(vals, 0, element.edges);
            std::vector<double> nodeArguments = getSubVector(vals, numEdges, element.nodes);
            y(i) = element.equation(edgeArguments, nodeArguments);
        }
        for (int i=0; i<numNodes/* - 1*/; ++i)
        {
            int rowIdx = numEdges + i;
            for (const auto& edge : m_nodes[i].incidentEdges)
            {
                y(rowIdx) += edge.first * vals(edge.second);
            }
        }
        y(numEqs-1) = vals(numEqs-1);
        if (y.norm() < m_convergenceTolerance)
        {
            break;
        }

        // Construct the jacobian
        Eigen::MatrixXd J = Eigen::MatrixXd::Zero(numEqs, numEqs);

        for (int i = 0; i < m_solverElements.size(); ++i)
        {
            NLSolverElement& element = m_solverElements[i];
            for (int j = 0; j < element.edges.size(); ++j)
            {
                std::vector<double> edgeArguments = getSubVector(vals, 0, element.edges);
                std::vector<double> nodeArguments = getSubVector(vals, numEdges, element.nodes);
                J(i, element.edges[j]) = element.edgeDerivatives[j](edgeArguments, nodeArguments);
            }
            for (int j = 0; j < element.nodes.size(); ++j)
            {
                std::vector<double> edgeArguments = getSubVector(vals, 0, element.edges);
                std::vector<double> nodeArguments = getSubVector(vals, numEdges, element.nodes);
                J(i, numEdges + element.nodes[j]) = element.nodeDerivatives[j](edgeArguments, nodeArguments);
            }
        }
        for (int i=0; i<numNodes/* - 1*/; ++i)
        {
            int rowIdx = numEdges + i;
            for (const auto& edge : m_nodes[i].incidentEdges)
            {
                J(rowIdx, edge.second) = edge.first;
            }
        }
        J(numEqs-1, numEqs-1) = 1.;

//        for (int i = 0; i < numEqs; ++i)
//        {
//            std::string debugString;
//            for (int j = 0; j < numEqs; ++j)
//            {
//                debugString += std::to_string(J(i,j)) + ",";
//            }
//            qDebug() << debugString.c_str() /*<< " || " << std::to_string(u(i)).c_str()*/;
//        }

//        Eigen::MatrixXd I = J.inverse();
//        for (int i = 0; i < numEqs; ++i)
//        {
//            std::string debugString;
//            for (int j = 0; j < numEqs; ++j)
//            {
//                debugString += std::to_string(I(i,j)) + ",";
//            }
//            qDebug() << debugString.c_str() /*<< " || " << std::to_string(u(i)).c_str()*/;
//        }

//        qDebug() << "det(J) = " << J.determinant() << "\n";

        // Do Newton step
        Eigen::VectorXd diff = J.colPivHouseholderQr().solve(y);
        vals = vals - diff;
    }

    m_currents.reserve(numEdges);
    for (int i=0; i<numEdges; ++i)
    {
        m_currents.push_back(vals[i]);
    }
    m_potentials.reserve(numNodes);
    for (int i=0; i<numNodes; ++i)
    {
        m_potentials.push_back(vals[numEdges+i]);
    }
}

double NLSolver::getPotential(int i) const
{
    return m_potentials[i];
}

double NLSolver::getCurrent(int i) const
{
    return m_currents[i];
}
