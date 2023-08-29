#pragma once

#include <QGraphicsWidget>
#include <QWidget>

#include <EnumsDefsEtc.h>

#include <memory>
#include <vector>

class MainView;
class ScaleCollection;
class QComboBox;
class QDoubleSpinBox;
class QPushButton;

class ScaleWidget : public QWidget
{
public:
    ScaleWidget(double breadth, std::weak_ptr<ScaleCollection> scaleCollection, MainView* mainView);

public slots:
    void on_upperLimitSpinBox_valueChanged();
    void on_lowerLimitSpinBox_valueChanged();
    void on_physicalQuantityBox_currentIndexChanged();
    void on_adjustScaleButton_clicked();

private:
    void fillFromScaleCollection();

    void updateSpinBoxesFromCollection();

    void makeInternalConnections();

private:
    double m_breadth;

    std::weak_ptr<ScaleCollection> m_scaleCollection;

    MainView* m_mainView;

    QDoubleSpinBox* m_upperLimitSpinBox;
    QDoubleSpinBox* m_lowerLimitSpinBox;
    QComboBox* m_physicalQuantityBox;
    QPushButton* m_adjustScaleButton;

    std::vector<PhysicalQuantity> m_quantities;


};
