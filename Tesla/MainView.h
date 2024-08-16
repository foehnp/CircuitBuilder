#pragma once

#include <EnumsDefsEtc.h>

#include <QGraphicsView>

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

    void run();
    void stop();

    void autoAdjustScale(PhysicalQuantity quantity);

    bool saveToFile(const QString& path) const;

    bool loadFromFile(const QString& path);

private:
    void initializeDrawArea();
    void initializeToolboxArea();
    void updateValues();

private:
    QGraphicsScene* m_scene;

    Menu* m_menu;
    DrawAreaItem* m_drawArea;
    ToolboxArea* m_toolboxArea;
    ControlPane* m_controlPane;
    ScaleWidget* m_scaleWidget;


    double m_squareBreadth = 50.;
    int m_horiNumSquares = 6, m_vertNumSquares = 8;

    MouseModes m_currentMouseMode = MouseModes::NeutralMM;
    int m_currentMouseOrientation = 0;

    RunMode m_runMode = Drawing;

    QTimer* m_runTimer;

    std::shared_ptr<ScaleCollection> m_scaleCollection;
};
