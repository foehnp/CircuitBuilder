#pragma once

#include <EnumsDefsEtc.h>

#include <Solver/Solver.h>
#include <Solver/NLSolver.h>

#include <QGraphicsItem>

#include <memory>

class ComponentDI;
class MainView;
class ScaleCollection;

class DrawAreaItem : public QGraphicsItem
{
public:
    struct ScaleSettings
    {
        int whatToShow = 0; // 0 = Voltage, 1 = Current
        double scaleMin = -1;
        double scaleMax = 1;
    };

public:
    DrawAreaItem(int horiNumSquares, int vertNumSquares,
                   double squareBreadth, MainView* mainView,
                   QGraphicsItem *parent = nullptr);

    void setCurrentRunMode(const RunMode& runMode);
    RunMode getCurrentRunMode() const;

    ComponentDI* addComponentDI(ComponentName name, int m, int n, int orientation);

    void setDisplaySettings(std::shared_ptr<ScaleCollection> displaySettings);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF boundingRect() const override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

//    void constructSolver();

    void constructNLSolver();

    void updateValues();

    bool getMinMaxValues(PhysicalQuantity quantity, double& min, double& max) const;

private:
    void onItemSelected(int m, int n);

private:
    int m_horiNumSquares, m_vertNumSquares;
    double m_squareBreadth;

    std::vector<std::vector<ComponentDI*>> m_components;

    MainView* m_mainView;

    RunMode m_runMode = Drawing;

//    std::shared_ptr<Solver> m_solver;
    std::shared_ptr<NLSolver> m_NLsolver;

//    std::vector<double> m_potentials;
//    std::vector<double> m_currents;

    std::shared_ptr<ScaleCollection> m_displaySettings;

};
