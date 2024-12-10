#include "NLSolver.h"

#include <Eigen/Dense>

#include <QDebug>

#include <algorithm>


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

void NLSolver::markNonConservativeNode(const std::vector<int> &nodes)
{
    m_nonConservativeNodes.insert(nodes.begin(), nodes.end());
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

bool NLSolver::compute()
{
    m_currents.clear();
    m_potentials.clear();
    int numEdges = m_edges.size();
    int numNodes = m_nodes.size();
    int numElements = m_solverElements.size();
    int numNonConservativeNodes = m_nonConservativeNodes.size();
    int numEqs   = numElements + numNodes - numNonConservativeNodes;
    if (numEqs == 0)
    {
        return false;
    }

    Eigen::VectorXd vals = Eigen::VectorXd::Zero(numEqs);

    int maxIters = 100;
    int iter = 0;
    for (; iter < maxIters; ++iter)
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

        int usedNodes = 0;
        for (int i=0; i<numNodes; ++i)
        {
            if (m_nonConservativeNodes.count(i) > 0)
            {
                continue;
            }
            int rowIdx = numElements + usedNodes;
            for (const auto& edge : m_nodes[i].incidentEdges)
            {
                y(rowIdx) += edge.first * vals(edge.second);
            }
            ++usedNodes;
        }
//        y(numEqs-1) = vals(numEqs-1);
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

        usedNodes = 0;
        for (int i=0; i<numNodes; ++i)
        {
            if (m_nonConservativeNodes.count(i) > 0)
            {
                continue;
            }
            int rowIdx = numElements + usedNodes;
            for (const auto& edge : m_nodes[i].incidentEdges)
            {
                J(rowIdx, edge.second) = edge.first;
            }
            ++usedNodes;
        }

        // Do Newton step
        Eigen::VectorXd diff = J.colPivHouseholderQr().solve(y);
        vals = vals - diff;
    }

    //check if we've exceeded the number of allowed iterations
    if (iter == maxIters)
    {
        return false;
    }
    //check if we have NaNs
    for (const auto& val : vals)
    {
        if (val != val)
        {
            return false;
        }
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
    // Make average potential equal to zero
//    double meanOfPots = std::accumulate(m_potentials.begin(), m_potentials.end(), 0.) / m_potentials.size();
//    std::for_each(m_potentials.begin(), m_potentials.end(), [meanOfPots](double& val){val -= meanOfPots;});

    return true;
}

double NLSolver::getPotential(int i) const
{
    return m_potentials[i];
}

double NLSolver::getCurrent(int i) const
{
    return m_currents[i];
}

bool NLSolver::getMinMaxPotential(double &min, double &max) const
{
    if (m_potentials.empty())
    {
        return false;
    }

    min = *std::min_element(m_potentials.begin(), m_potentials.end());
    max = *std::max_element(m_potentials.begin(), m_potentials.end());
    return true;
}

bool NLSolver::getMinMaxCurrent(double &min, double &max) const
{
    if (m_currents.empty())
    {
        return false;
    }
    min = fabs(m_currents[0]);
    max = fabs(m_currents[0]);
    for (int i = 0; i < m_currents.size(); ++i)
    {
        min = std::min(min, fabs(m_currents[i]));
        max = std::max(max, fabs(m_currents[i]));
    }
    return true;
}
