#include "MainView.h"

#include <ControlPane/ControlPane.h>

#include <DrawArea/DrawAreaItem.h>

#include <Toolbox/ToolboxArea.h>
#include <Toolbox/ComponentTI.h>
#include <Toolbox/WireTI.h>
#include <Toolbox/EmptyComponentTI.h>
#include <Toolbox/CornerWireTI.h>
#include <Toolbox/EraserTI.h>
#include <Toolbox/BatteryTI.h>
#include <Toolbox/BulbTI.h>
#include <Toolbox/ForkTI.h>
#include <Toolbox/DiodeTI.h>

#include <Scale/ScaleCollection.h>
#include <Scale/ScaleWidget.h>

#include <QWheelEvent>
#include <QTimer>
#include <QGraphicsProxyWidget>

MainView::MainView()
{
    m_scene = new QGraphicsScene;
    setScene(m_scene);

    m_drawArea = new DrawAreaItem(m_horiNumSquares, m_vertNumSquares, m_squareBreadth, this);
    m_toolboxArea = new ToolboxArea(m_squareBreadth, this);
    m_controlPane = new ControlPane(this, m_squareBreadth);
    m_scene->addItem(m_drawArea);
    m_scene->addItem(m_toolboxArea);
    m_scene->addItem(m_controlPane);
    m_drawArea->setPos(m_squareBreadth*0.5, m_squareBreadth*0.5);
    m_toolboxArea->setPos(m_horiNumSquares*m_squareBreadth + m_squareBreadth*1., 0.5*m_squareBreadth);

    initializeDrawArea();
    initializeToolboxArea();

    QPointF pos = m_drawArea->boundingRect().bottomLeft() + m_drawArea->pos();
    m_controlPane->setPos(pos.x(), pos.y() + m_squareBreadth*0.2);

    m_scaleCollection = std::make_shared<ScaleCollection>();
    m_scaleCollection->setScale(Potential, -1., 1., "Potential");
    m_scaleCollection->setScale(Current, -1., 1., "Current");
    m_scaleCollection->setCurrentQuantity(Potential);
    m_scaleWidget = new ScaleWidget(m_squareBreadth, m_scaleCollection);
    QGraphicsProxyWidget * proxyWidget = m_scene->addWidget(m_scaleWidget);
    pos = m_controlPane->boundingRect().topRight() + m_controlPane->pos();
    proxyWidget->setPos(pos.x() + m_squareBreadth*0.2, pos.y());
    proxyWidget->setZValue(1);
    m_drawArea->setDisplaySettings(m_scaleCollection);

    m_scene->setSceneRect(QRect(0., 0.,(m_horiNumSquares+ 2.5)*m_squareBreadth, (m_controlPane->boundingRect().bottomLeft() + m_controlPane->pos()).y() + 40.));
}

void MainView::setCurrentRunMode(const RunMode &runMode)
{
    m_runMode = runMode;
    m_drawArea->setCurrentRunMode(runMode);
    m_controlPane->setState(runMode);
    if (runMode == Running)
    {
        m_toolboxArea->setIsAvailable(false);
        run();
    }
    else if (runMode == Drawing)
    {
        m_toolboxArea->setIsAvailable(true);
        stop();
    }
}

RunMode MainView::getCurrentRunMode() const
{
    return m_runMode;
}

void MainView::setCurrentMouseMode(const MouseModes &mouseMode)
{
    m_currentMouseMode = mouseMode;
}

MouseModes MainView::getCurrentMouseMode() const
{
    return m_currentMouseMode;
}

void MainView::setCurrentMouseOrientation(int orientation)
{
    m_currentMouseOrientation = orientation;
}

int MainView::getCurrentMouseOrientation() const
{
    return m_currentMouseOrientation;
}

void MainView::wheelEvent(QWheelEvent *event)
{
    int rawDelta = event->angleDelta().y();
    int delta = (event->angleDelta().y() / 120) % 4;
    int newOrientation = (m_currentMouseOrientation + delta + 4) % 4;
    if (newOrientation != m_currentMouseOrientation)
    {
        m_currentMouseOrientation = newOrientation;
    }
}

void MainView::run()
{
    m_drawArea->constructSolver();
    m_drawArea->constructNLSolver();
    updateValues();
    m_runTimer = new QTimer(this);
    connect(m_runTimer, &QTimer::timeout, this, &MainView::updateValues);
    m_runTimer->start(500);
}

void MainView::stop()
{
    delete m_runTimer;
}

void MainView::updateValues()
{
    m_drawArea->updateValues();
}

void MainView::initializeDrawArea()
{

}

void MainView::initializeToolboxArea()
{
    double wireThickness = 0.05;

    EmptyComponentTI* eTI = new EmptyComponentTI(m_squareBreadth);
    WireTI* wTI = new WireTI(m_squareBreadth, wireThickness);
    CornerWireTI* cwTI = new CornerWireTI(m_squareBreadth, wireThickness);
    ForkTI* forkTI = new ForkTI(m_squareBreadth, wireThickness);
    EraserTI* erTI = new EraserTI(m_squareBreadth);
    BatteryTI* batTI = new BatteryTI(m_squareBreadth, wireThickness);
    BulbTI* bulbTI = new BulbTI(m_squareBreadth, wireThickness);
    DiodeTI* diodeTI = new DiodeTI(m_squareBreadth, wireThickness);

    m_toolboxArea->pushBackItem(eTI);
    m_toolboxArea->pushBackItem(erTI);
    m_toolboxArea->pushBackItem(wTI);
    m_toolboxArea->pushBackItem(cwTI);
    m_toolboxArea->pushBackItem(forkTI);
    m_toolboxArea->pushBackItem(batTI);
    m_toolboxArea->pushBackItem(bulbTI);
    m_toolboxArea->pushBackItem(diodeTI);
}
