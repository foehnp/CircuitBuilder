#include "ScaleWidget.h"

#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGraphicsLayoutItem>

#include "MainView.h"
#include "ScaleCollection.h"

#include <limits>


ScaleWidget::ScaleWidget(double height, std::weak_ptr<ScaleCollection> scaleCollection, MainView* mainView) :
    m_height(height),
    m_scaleCollection(scaleCollection),
    m_mainView(mainView)
{
    setFixedHeight(m_height);
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    QVBoxLayout *colorScaleVLayout = new QVBoxLayout(this);
    hlayout->addLayout(colorScaleVLayout);
    QVBoxLayout *inputVLayout = new QVBoxLayout(this);
    hlayout->addLayout(inputVLayout);
    QVBoxLayout *righttVlayout = new QVBoxLayout(this);
    hlayout->addLayout(righttVlayout);


    m_upperLimitLineEdit = new QLineEdit();
    m_lowerLimitLineEdit = new QLineEdit();
    m_upperLimitLineEdit->setValidator(new QDoubleValidator(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 3, this) );
    m_lowerLimitLineEdit->setValidator(new QDoubleValidator(-std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 3, this) );
    inputVLayout->addWidget(m_upperLimitLineEdit);
    inputVLayout->addWidget(m_lowerLimitLineEdit);

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
    collection->setScaleMax(currentQuantity, m_upperLimitLineEdit->text().toDouble());
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
    collection->setScaleMin(currentQuantity, m_lowerLimitLineEdit->text().toDouble());
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
            m_upperLimitLineEdit->setText(QString::number(scale.m_scaleMax, 'f', 3));
            m_lowerLimitLineEdit->setText(QString::number(scale.m_scaleMin, 'f', 3));
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
    m_upperLimitLineEdit->setText(QString::number(currentScale.m_scaleMax, 'f', 3));
    m_lowerLimitLineEdit->setText(QString::number(currentScale.m_scaleMin, 'f', 3));
}

void ScaleWidget::makeInternalConnections()
{
    connect(m_physicalQuantityBox, &QComboBox::currentIndexChanged,  this, &ScaleWidget::on_physicalQuantityBox_currentIndexChanged);
    connect(m_upperLimitLineEdit, &QLineEdit::textEdited,  this, &ScaleWidget::on_upperLimitSpinBox_valueChanged);
    connect(m_lowerLimitLineEdit, &QLineEdit::textEdited,  this, &ScaleWidget::on_lowerLimitSpinBox_valueChanged);
    connect(m_adjustScaleButton, &QPushButton::clicked,  this, &ScaleWidget::on_adjustScaleButton_clicked);
}
