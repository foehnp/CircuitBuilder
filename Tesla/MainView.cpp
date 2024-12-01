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
#include <Toolbox/BJTTI.h>

#include <Scale/ScaleCollection.h>
#include <Scale/ScaleGraphicsItem.h>
#include <Scale/ScaleWidget.h>

#include <QWheelEvent>
#include <QFile>
#include <QGraphicsProxyWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

#include <fstream>

#include <Menu/Menu.h>

MainView::MainView()
{
    m_scene = new QGraphicsScene;
    setScene(m_scene);

    m_menu = new Menu(this, m_squareBreadth * 0.6);
    m_drawArea = new DrawAreaItem(m_horiNumSquares, m_vertNumSquares, m_squareBreadth, this);
    m_toolboxArea = new ToolboxArea(m_squareBreadth, this);
    m_controlPane = new ControlPane(this, m_squareBreadth);
    m_scene->addItem(m_menu);
    m_scene->addItem(m_drawArea);
    m_scene->addItem(m_toolboxArea);
    m_scene->addItem(m_controlPane);
    m_menu->setPos(m_squareBreadth*0.5, m_squareBreadth*0.5);
    double drawAreaYStart = m_menu->boundingRect().bottomLeft().y() + m_squareBreadth*0.8;
    m_drawArea->setPos(m_squareBreadth*0.5, drawAreaYStart);
    m_toolboxArea->setPos(m_horiNumSquares*m_squareBreadth + m_squareBreadth*1., drawAreaYStart);

    initializeDrawArea();
    initializeToolboxArea();

    QPointF pos = m_drawArea->boundingRect().bottomLeft() + m_drawArea->pos();
    m_controlPane->setPos(pos.x(), pos.y() + m_squareBreadth*0.2);

    m_scaleCollection = std::make_shared<ScaleCollection>();
    m_scaleCollection->setScale(Potential, -1., 1., "Potential");
    m_scaleCollection->setScale(Current, 0., 0.01, "Current");
    m_scaleCollection->setCurrentQuantity(Potential);
    m_scaleGraphicsItem = new ScaleGraphicsItem(m_squareBreadth*0.3, m_squareBreadth*1.5);
    m_scene->addItem(m_scaleGraphicsItem);
    m_scaleGraphicsItem->setScale(m_scaleCollection->getColorScaleFunc());
    pos = m_controlPane->boundingRect().topRight() + m_controlPane->pos() + QPointF(m_squareBreadth*0.4, 0.);
    m_scaleGraphicsItem->setPos(pos);

    m_scaleWidget = new ScaleWidget(m_squareBreadth*1.5, m_scaleCollection, this);
    QGraphicsProxyWidget * proxyWidget = m_scene->addWidget(m_scaleWidget);
    pos = m_scaleGraphicsItem->boundingRect().topRight() + m_scaleGraphicsItem->pos() + QPointF(m_squareBreadth*0.1, 0.);
    proxyWidget->setPos(pos);
    proxyWidget->setZValue(1);
    m_drawArea->setDisplaySettings(m_scaleCollection);

    m_scene->setSceneRect(QRect(0., 0.,(m_horiNumSquares+ 2.5)*m_squareBreadth, (m_controlPane->boundingRect().bottomLeft() + m_controlPane->pos()).y() + 40.));
}

void MainView::setCurrentRunMode(const RunMode &runMode)
{
    if (runMode == Running)
    {
        if (!run())
        {
            QMessageBox::information(
                this,
                tr("Computation failed."),
                tr("Computation failed. Maybe there is a short circuit or something, or the solver failed.") );
            return;
        }
    }
    else if (runMode == Drawing)
    {
        stop();
    }

    m_runMode = runMode;
    m_menu->setIsAvailableGeneral(runMode == Drawing);
    m_toolboxArea->setIsAvailable(runMode == Drawing);
    m_drawArea->setCurrentRunMode(runMode);
    m_controlPane->setState(runMode);
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

bool MainView::run()
{
    m_drawArea->constructNLSolver();
    if (!updateValues())
    {
        return false;
    }
    m_runTimer = std::make_unique<QTimer>(this);
    connect(&*m_runTimer, &QTimer::timeout, this, &MainView::updateValues);
    m_runTimer->start(500);
    return true;
}

void MainView::stop()
{
    m_runTimer.reset();
}

void MainView::autoAdjustScale(PhysicalQuantity quantity)
{
    double min, max;
    if (!m_drawArea->getMinMaxValues(quantity, min, max))
    {
        return;
    }
    m_scaleCollection->setScaleMin(quantity, min);
    m_scaleCollection->setScaleMax(quantity, max);
}


static QString DRAWAREA_NAME = "draw_area";

bool MainView::saveToFile(const QString& path) const
{
    QJsonDocument document;
    QJsonObject drawAreaObject;
    m_drawArea->saveToFile(drawAreaObject);
    QJsonObject topLevelObject;
    topLevelObject[DRAWAREA_NAME] = drawAreaObject;

    document.setObject(topLevelObject);
    QByteArray byteArray = document.toJson();

    std::ofstream ofs(path.toStdString());
    ofs << byteArray.toStdString();
    ofs.close();

    m_menu->setFilePath(path);
    return true;
}

bool MainView::loadFromFile(const QString& path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QByteArray byteArray = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(byteArray);
    if (!document.isObject())
    {
        return false;
    }
    QJsonObject topLevelObject = document.object();

    if (!topLevelObject.contains(DRAWAREA_NAME))
    {
        return false;
    }
    QJsonValue drawAreaValue = topLevelObject[DRAWAREA_NAME];
    if (!drawAreaValue.isObject())
    {
        return false;
    }
    if (!m_drawArea->loadFromFile(drawAreaValue.toObject()))
    {
        return false;
    }
    m_menu->setFilePath(path);
    return true;
}

bool MainView::updateValues()
{
    return m_drawArea->updateValues();
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
    BJTTI* bJTTI = new BJTTI(m_squareBreadth, wireThickness);

    m_toolboxArea->pushBackItem(eTI);
    m_toolboxArea->pushBackItem(erTI);
    m_toolboxArea->pushBackItem(wTI);
    m_toolboxArea->pushBackItem(cwTI);
    m_toolboxArea->pushBackItem(forkTI);
    m_toolboxArea->pushBackItem(batTI);
    m_toolboxArea->pushBackItem(bulbTI);
    m_toolboxArea->pushBackItem(diodeTI);
    m_toolboxArea->pushBackItem(bJTTI);

    m_toolboxArea->setSelectedItem(0);
}
