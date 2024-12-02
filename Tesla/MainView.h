#pragma once

#include <EnumsDefsEtc.h>

#include <QGraphicsView>
#include <QTimer>

class ScaleGraphicsItem;
class Menu;
class DrawAreaItem;
class ToolboxArea;
class ControlPane;
class QTimer;
class ScaleWidget;
class ScaleCollection;

class MainView : public QGraphicsView
{
public:
    MainView();

    void setCurrentRunMode(const RunMode& runMode);
    RunMode getCurrentRunMode() const;

    void setCurrentMouseMode(const MouseModes& mouseMode);
    MouseModes getCurrentMouseMode() const;

    void setCurrentMouseOrientation(int orientation);
    int getCurrentMouseOrientation() const;

    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent *event) override;

    bool run();
    void stop();

    void autoAdjustScale(PhysicalQuantity quantity);

    bool saveToFile(const QString& path) const;

    bool loadFromFile(const QString& path);

private:
    void initializeDrawArea();
    void initializeToolboxArea();
    bool updateValues();

private:
    QGraphicsScene* m_scene;

    Menu* m_menu;
    DrawAreaItem* m_drawArea;
    ToolboxArea* m_toolboxArea;
    ControlPane* m_controlPane;
    ScaleGraphicsItem* m_scaleGraphicsItem;
    ScaleWidget* m_scaleWidget;


    double m_squareBreadth = 50.;
    int m_horiNumSquares = 10, m_vertNumSquares = 10;

    MouseModes m_currentMouseMode = MouseModes::NeutralMM;
    int m_currentMouseOrientation = 0;

    RunMode m_runMode = Drawing;

    std::unique_ptr<QTimer> m_runTimer;

    std::shared_ptr<ScaleCollection> m_scaleCollection;

};
