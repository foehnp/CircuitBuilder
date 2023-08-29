#include "ScaleWidget.h"

#include <QDoubleSpinBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "MainView.h"
#include "ScaleCollection.h"

ScaleWidget::ScaleWidget(double breadth, std::weak_ptr<ScaleCollection> scaleCollection, MainView* mainView) :
    m_breadth(breadth),
    m_scaleCollection(scaleCollection),
    m_mainView(mainView)
{
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    QVBoxLayout *leftVlayout = new QVBoxLayout(this);
    hlayout->addLayout(leftVlayout);
    QVBoxLayout *righttVlayout = new QVBoxLayout(this);
    hlayout->addLayout(righttVlayout);
    m_upperLimitSpinBox = new QDoubleSpinBox();
    m_upperLimitSpinBox->setDecimals(4);
    m_upperLimitSpinBox->setMinimum(0.);
    m_upperLimitSpinBox->setMaximum(10.);
    m_upperLimitSpinBox->setSingleStep(0.0001);
    m_lowerLimitSpinBox = new QDoubleSpinBox();
    m_lowerLimitSpinBox->setDecimals(4);
    m_lowerLimitSpinBox->setMinimum(-10.);
    m_lowerLimitSpinBox->setMaximum(0.);
    m_lowerLimitSpinBox->setSingleStep(0.0001);
    leftVlayout->addWidget(m_upperLimitSpinBox);
    leftVlayout->addWidget(m_lowerLimitSpinBox);
    m_physicalQuantityBox = new QComboBox();
    righttVlayout->addWidget(m_physicalQuantityBox);
    m_adjustScaleButton = new QPushButton();
    m_adjustScaleButton->setText("Adjust scale");
    righttVlayout->addWidget(m_adjustScaleButton);

    fillFromScaleCollection();
    makeInternalConnections();
}

void ScaleWidget::on_upperLimitSpinBox_valueChanged()
{
    auto collection = m_scaleCollection.lock();
    if (!collection)
    {
        return;
    }
    PhysicalQuantity& currentQuantity = m_quantities[m_physicalQuantityBox->currentIndex()];
    collection->setScaleMax(currentQuantity, m_upperLimitSpinBox->value());
    updateSpinBoxesFromCollection();
}

void ScaleWidget::on_lowerLimitSpinBox_valueChanged()
{
    auto collection = m_scaleCollection.lock();
    if (!collection)
    {
        return;
    }
    PhysicalQuantity& currentQuantity = m_quantities[m_physicalQuantityBox->currentIndex()];
    collection->setScaleMin(currentQuantity, m_lowerLimitSpinBox->value());
    updateSpinBoxesFromCollection();
}

void ScaleWidget::on_physicalQuantityBox_currentIndexChanged()
{
    auto collection = m_scaleCollection.lock();
    if (!collection)
    {
        return;
    }
    PhysicalQuantity currentQuantity = m_quantities[m_physicalQuantityBox->currentIndex()];
    collection->setCurrentQuantity(currentQuantity);
    updateSpinBoxesFromCollection();
}

void ScaleWidget::on_adjustScaleButton_clicked()
{
    auto collection = m_scaleCollection.lock();
    if (!collection)
    {
        return;
    }
    m_mainView->autoAdjustScale(collection->getCurrentQuantity());
    updateSpinBoxesFromCollection();
}

void ScaleWidget::fillFromScaleCollection()
{
    auto collection = m_scaleCollection.lock();
    if (!collection)
    {
        return;
    }

    PhysicalQuantity currentQuantity = collection->getCurrentQuantity();
    auto scaleMap = collection->getAllScales();
    m_quantities.clear();
    m_quantities.reserve(scaleMap.size());
    for (const auto& [quantity, scale] : scaleMap)
    {
        m_quantities.push_back(quantity);
        m_physicalQuantityBox->addItem(scale.m_displayName.c_str());
        if (quantity == currentQuantity)
        {
            m_physicalQuantityBox->setCurrentIndex(m_quantities.size() - 1);
            m_upperLimitSpinBox->setValue(scale.m_scaleMax);
            m_lowerLimitSpinBox->setValue(scale.m_scaleMin);
        }
    }

}

void ScaleWidget::updateSpinBoxesFromCollection()
{
    auto collection = m_scaleCollection.lock();
    if (!collection)
    {
        return;
    }
    PhysicalQuantity currentQuantity = collection->getCurrentQuantity();
    auto scaleMap = collection->getAllScales();
    ScaleCollection::Scale currentScale = scaleMap[currentQuantity];
    m_upperLimitSpinBox->setValue(currentScale.m_scaleMax);
    m_lowerLimitSpinBox->setValue(currentScale.m_scaleMin);
}

void ScaleWidget::makeInternalConnections()
{
    connect(m_physicalQuantityBox, &QComboBox::currentIndexChanged,  this, &ScaleWidget::on_physicalQuantityBox_currentIndexChanged);
    connect(m_upperLimitSpinBox, &QDoubleSpinBox::valueChanged,  this, &ScaleWidget::on_upperLimitSpinBox_valueChanged);
    connect(m_lowerLimitSpinBox, &QDoubleSpinBox::valueChanged,  this, &ScaleWidget::on_lowerLimitSpinBox_valueChanged);
    connect(m_adjustScaleButton, &QPushButton::clicked,  this, &ScaleWidget::on_adjustScaleButton_clicked);
}
