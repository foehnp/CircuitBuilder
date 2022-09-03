#pragma once

#include <QGraphicsWidget>
#include <QWidget>

#include <EnumsDefsEtc.h>

#include <memory>
#include <vector>

class ScaleCollection;
class QComboBox;
class QDoubleSpinBox;

class ScaleWidget : public QWidget
{
public:
    ScaleWidget(double breadth, std::weak_ptr<ScaleCollection> scaleCollection);

public slots:
    void on_upperLimitSpinBox_valueChanged();
    void on_lowerLimitSpinBox_valueChanged();
    void on_physicalQuantityBox_currentIndexChanged();

private:
    void fillFromScaleCollection();

    void updateSpinBoxesFromCollection();

    void makeInternalConnections();

private:
    double m_breadth;

    std::weak_ptr<ScaleCollection> m_scaleCollection;

    QDoubleSpinBox* m_upperLimitSpinBox;
    QDoubleSpinBox* m_lowerLimitSpinBox;
    QComboBox* m_physicalQuantityBox;

    std::vector<PhysicalQuantity> m_quantities;


};
