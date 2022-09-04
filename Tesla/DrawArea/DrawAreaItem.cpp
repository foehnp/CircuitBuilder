#include "DrawAreaItem.h"

#include <DrawArea/WireDI.h>
#include <DrawArea/CornerWireDI.h>
#include <DrawArea/EmptyComponentDI.h>
#include <DrawArea/BatteryDI.h>
#include <DrawArea/BulbDI.h>
#include <DrawArea/ForkDI.h>
#include <DrawArea/DiodeDI.h>


#include <MainView.h>


#include <QGraphicsSceneMouseEvent>

DrawAreaItem::DrawAreaItem(int horiNumSquares, int vertNumSquares,
                               double squareBreadth, MainView* mainView,
                               QGraphicsItem*parent):
    QGraphicsItem(parent),
    m_horiNumSquares(horiNumSquares),
    m_vertNumSquares(vertNumSquares),
    m_squareBreadth(squareBreadth),
    m_mainView(mainView)
{
    m_components = std::vector<std::vector<ComponentDI*>>(m_horiNumSquares, std::vector<ComponentDI*>(m_vertNumSquares, nullptr));
    for (int i = 0; i < horiNumSquares; ++i)
    {
        for (int j = 0; j < vertNumSquares; ++j)
        {
            m_components[i][j] = addComponentDI(EmptyComponent, i, j, 0);
        }
    }
}

void DrawAreaItem::setCurrentRunMode(const RunMode &runMode)
{
    m_runMode = runMode;
    for (int i = 0; i < m_horiNumSquares; ++i)
    {
        for (int j = 0; j < m_vertNumSquares; ++j)
        {
            m_components[i][j]->setCurrentRunMode(runMode);
        }
    }
}

RunMode DrawAreaItem::getCurrentRunMode() const
{
    return m_runMode;
}

ComponentDI* DrawAreaItem::addComponentDI(ComponentName name, int m, int n, int orientation)
{
    delete m_components[m][n];
    ComponentDI*& componentDI = m_components[m][n];
    componentDI = nullptr;
    if (name == Wire)
    {
        componentDI = new WireDI(m_squareBreadth, 0.05, orientation);
    }
    else if ( name == EmptyComponent)
    {
        componentDI = new EmptyComponentDI(m_squareBreadth, orientation);
    }
    else if ( name == CornerWire)
    {
        componentDI = new CornerWireDI(m_squareBreadth, 0.05, orientation);
    }
    else if ( name == Fork)
    {
        componentDI = new ForkDI(m_squareBreadth, 0.05, orientation);
    }
    else if ( name == Battery)
    {
        componentDI = new BatteryDI(m_squareBreadth, 0.05, orientation);
    }
    else if ( name == Bulb)
    {
        componentDI = new BulbDI(m_squareBreadth, 0.05, orientation);
    }
    else if ( name == Diode)
    {
        componentDI = new DiodeDI(m_squareBreadth, 0.05, orientation);
    }
    componentDI->setParentItem(this);
    componentDI->setPos((m+0.5)*m_squareBreadth, (n+0.5)*m_squareBreadth);
    componentDI->setDisplaySettings(m_displaySettings);
    update();
    return componentDI;
}

void DrawAreaItem::setDisplaySettings(std::shared_ptr<ScaleCollection> displaySettings)
{
    m_displaySettings = displaySettings;
    for (int j = 0; j < m_vertNumSquares; ++j)
    {
        for (int i = 0; i < m_horiNumSquares; ++i)
        {
            m_components[i][j]->setDisplaySettings(displaySettings);
        }
    }
}


void DrawAreaItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

QRectF DrawAreaItem::boundingRect() const
{
    return QRectF(0., 0., m_squareBreadth*m_horiNumSquares, m_squareBreadth*m_vertNumSquares);
}

void DrawAreaItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    double mDouble = event->pos().x() / m_squareBreadth;
    double nDouble = event->pos().y() / m_squareBreadth;
    if (0. < mDouble && mDouble < m_horiNumSquares &&
        0. < nDouble && nDouble < m_vertNumSquares)
    {
        int m = int(mDouble);
        int n = int(nDouble);
        onItemSelected(m, n);
    }
    event->accept();
}

void DrawAreaItem::onItemSelected(int m, int n)
{
    int mouseOrientation = m_mainView->getCurrentMouseOrientation();

    MouseModes mouseMode = m_mainView->getCurrentMouseMode();
    if (mouseMode == MouseModes::NeutralMM)
    {
        return;
    }
    else if (mouseMode == MouseModes::EraserMM)
    {
        addComponentDI(ComponentName::EmptyComponent, m, n, mouseOrientation);
    }
    else if (mouseMode == MouseModes::WireMM)
    {
        addComponentDI(ComponentName::Wire, m, n, mouseOrientation);
    }
    else if (mouseMode == MouseModes::CornerWireMM)
    {
        addComponentDI(ComponentName::CornerWire, m, n, mouseOrientation);
    }
    else if (mouseMode == MouseModes::ForkMM)
    {
        addComponentDI(ComponentName::Fork, m, n, mouseOrientation);
    }
    else if (mouseMode == MouseModes::BatteryMM)
    {
        addComponentDI(ComponentName::Battery, m, n, mouseOrientation);
    }
    else if (mouseMode == MouseModes::BulbMM)
    {
        addComponentDI(ComponentName::Bulb, m, n, mouseOrientation);
    }
    else if (mouseMode == MouseModes::DiodeMM)
    {
        addComponentDI(ComponentName::Diode, m, n, mouseOrientation);
    }
}

void DrawAreaItem::constructSolver()
{
    for (int j = 0; j < m_vertNumSquares; ++j)
    {
        for (int i = 0; i < m_horiNumSquares; ++i)
        {
            m_components[i][j]->resetNodes();
            m_components[i][j]->resetEdges();
        }
    }

    m_solver = std::make_shared<Solver>();
    // Connect outer nodes of components
    for (int j = 0; j < m_vertNumSquares; ++j)
    {
        for (int i = 0; i < m_horiNumSquares; ++i)
        {
            ComponentDI* currDI = m_components[i][j];

            if(int* leftNode = currDI->getNodeAt(3))
            {
                if (*leftNode == -1)
                {
                    *leftNode = m_solver->addNode();
                }
            }
            if(int* topNode = currDI->getNodeAt(0))
            {
                if (*topNode == -1)
                {
                    *topNode = m_solver->addNode();
                }
            }

            if(int* rightNode = currDI->getNodeAt(1))
            {
                *rightNode = m_solver->addNode();
                if (i < m_horiNumSquares - 1)
                {
                    ComponentDI* rightDI = m_components[i+1][j];
                    if (int* neighborNode = rightDI->getNodeAt(3))
                    {
                        *neighborNode = *rightNode;
                    }
                }
            }
            if(int* bottomNode = currDI->getNodeAt(2))
            {
                *bottomNode = m_solver->addNode();
                if (j < m_vertNumSquares - 1)
                {
                    ComponentDI* bottomDI = m_components[i][j+1];
                    if (int* neighborNode = bottomDI->getNodeAt(0))
                    {
                        *neighborNode = *bottomNode;
                    }
                }
            }

            std::vector<int*> innerNodes = m_components[i][j]->getAllInnerNodes();
            for (const auto& innerNode : innerNodes)
            {
                *innerNode = m_solver->addNode();
            }
        }
    }

    for (int j = 0; j < m_vertNumSquares; ++j)
    {
        for (int i = 0; i < m_horiNumSquares; ++i)
        {
            std::vector<std::pair<int*, SolverEdge>> edgePairs = m_components[i][j]->getAllEdges();
            for (const auto& edgePair : edgePairs)
            {
                const SolverEdge& edge = edgePair.second;
                *edgePair.first = m_solver->addEdge(edge.lNode, edge.rNode, edge.resistance, edge.voltage);
            }
        }
    }

    for (int j = 0; j < m_vertNumSquares; ++j)
    {
        for (int i = 0; i < m_horiNumSquares; ++i)
        {
            m_components[i][j]->setSolver(m_solver);
        }
    }
}

void DrawAreaItem::constructNLSolver()
{
    for (int j = 0; j < m_vertNumSquares; ++j)
    {
        for (int i = 0; i < m_horiNumSquares; ++i)
        {
            m_components[i][j]->resetNodes();
            m_components[i][j]->resetEdges();
        }
    }

    m_NLsolver = std::make_shared<NLSolver>();
    // Connect outer nodes of components
    for (int j = 0; j < m_vertNumSquares; ++j)
    {
        for (int i = 0; i < m_horiNumSquares; ++i)
        {
            ComponentDI* currDI = m_components[i][j];

            if(int* leftNode = currDI->getNodeAt(3))
            {
                if (*leftNode == -1)
                {
                    *leftNode = m_NLsolver->addNode();
                }
            }
            if(int* topNode = currDI->getNodeAt(0))
            {
                if (*topNode == -1)
                {
                    *topNode = m_NLsolver->addNode();
                }
            }

            if(int* rightNode = currDI->getNodeAt(1))
            {
                *rightNode = m_NLsolver->addNode();
                if (i < m_horiNumSquares - 1)
                {
                    ComponentDI* rightDI = m_components[i+1][j];
                    if (int* neighborNode = rightDI->getNodeAt(3))
                    {
                        *neighborNode = *rightNode;
                    }
                }
            }
            if(int* bottomNode = currDI->getNodeAt(2))
            {
                *bottomNode = m_NLsolver->addNode();
                if (j < m_vertNumSquares - 1)
                {
                    ComponentDI* bottomDI = m_components[i][j+1];
                    if (int* neighborNode = bottomDI->getNodeAt(0))
                    {
                        *neighborNode = *bottomNode;
                    }
                }
            }

            std::vector<int*> innerNodes = m_components[i][j]->getAllInnerNodes();
            for (const auto& innerNode : innerNodes)
            {
                *innerNode = m_NLsolver->addNode();
            }
        }
    }

    for (int j = 0; j < m_vertNumSquares; ++j)
    {
        for (int i = 0; i < m_horiNumSquares; ++i)
        {
            std::vector<std::pair<int*, SolverEdge>> edgePairs = m_components[i][j]->getAllEdges();
            for (const auto& edgePair : edgePairs)
            {
                const SolverEdge& edge = edgePair.second;
                *edgePair.first = m_NLsolver->addEdge(edge.lNode, edge.rNode);
            }
        }
    }

    for (int j = 0; j < m_vertNumSquares; ++j)
    {
        for (int i = 0; i < m_horiNumSquares; ++i)
        {
            std::vector<NLSolverElement> elements = m_components[i][j]->getNLSolverElements();
            for (const auto& element : elements)
            {
                m_NLsolver->addElement(element);
            }
        }
    }

    for (int j = 0; j < m_vertNumSquares; ++j)
    {
        for (int i = 0; i < m_horiNumSquares; ++i)
        {
            m_components[i][j]->setNLSolver(m_NLsolver);
        }
    }
}

void DrawAreaItem::updateValues()
{
    m_solver->compute();
    m_NLsolver->compute();
    update();
}
