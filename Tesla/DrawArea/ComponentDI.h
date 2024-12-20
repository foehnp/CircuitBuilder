#pragma once

#include <Solver/Solver.h>
#include <Solver/NLSolver.h>

#include <QGraphicsItem>
#include <EnumsDefsEtc.h>

#include <limits>
#include <memory>
#include <string>

#include <vector>
#include <cmath>

class QJsonObject;
class ScaleCollection;

struct ComponentParam
{
    QString displayName;
    double val = 0.;
    double minVal = 0.;
    double maxVal = std::numeric_limits<double>::max();
    double decimals = 2;
    QString unit;
    ComponentParam(const QString& displayName, double val, double minVal, double maxVal, unsigned decimals, const QString& unit) :
        displayName(displayName),
        val(val),
        minVal(minVal),
        maxVal(maxVal),
        decimals(decimals),
        unit(unit)
    {
    }
};


class ComponentDI : public QGraphicsItem
{

public:
    ComponentDI(double squareBreadth, int orientation);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override final;

    QRectF boundingRect() const override;

    void setOrientation(int orientation){m_orientation = orientation;}
    int getOrientation() const
    {
        return m_orientation;
    }

    void setCurrentRunMode(const RunMode& runMode);
    RunMode getCurrentRunMode() const;

    int* getNodeAt(int direction);
    virtual std::vector<int*> getAllInnerNodes() = 0;
    virtual void resetNodes() = 0;

    virtual std::vector<std::pair<int*, SolverEdge>> getAllEdges() = 0;
    virtual void resetEdges() = 0;

    virtual std::vector<NLSolverElement> getNLSolverElements() = 0;

    virtual std::vector<int> getNonConservativeNodes()
    {
        return {};
    }

    void setDisplaySettings(const std::shared_ptr<ScaleCollection>& displaySettings);

    void setSolver(const std::shared_ptr<Solver>& solver);

    void setNLSolver(const std::shared_ptr<NLSolver>& NLsolver);

    virtual ComponentName getComponentName() const = 0;

    void runParamDialog();

    std::vector<std::pair<QString, double>> getUserParamsForPersistence() const;

    void setUserParamsFromPersistence(const std::unordered_map<QString, double>& persParams);


private:
    void paintBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void paintSymbol(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;
//    virtual void paintSymbolWithVoltage(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) = 0;

    virtual int* getNodeAtInternal(int direction) = 0;

protected:
    double m_squareBreadth;
    int m_orientation;

    RunMode m_runMode = Drawing;

    std::vector<ComponentParam> m_userParams;

    std::shared_ptr<ScaleCollection> m_displaySettings;
    std::shared_ptr<Solver> m_solver;
    std::shared_ptr<NLSolver> m_NLsolver;

private:
    double m_boundaryThickness = 0.02;

public:

    static void paintArrow(QPainter *painter, const QPointF& pos, const double& height, const QColor& color, int orientation);

};
