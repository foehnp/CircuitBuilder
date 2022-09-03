#include "ScaleCollection.h"

ScaleCollection::ScaleCollection()
{

}

void ScaleCollection::setCurrentQuantity(const PhysicalQuantity &quantity)
{
    m_currentQuantity = quantity;
    if (m_scales.find(quantity) == m_scales.end())
    {
        m_scales[quantity] = Scale(-1., 1., "Unknown");
    }
    m_currentScalePtr = &m_scales[quantity];
}

PhysicalQuantity ScaleCollection::getCurrentQuantity() const
{
    return m_currentQuantity;
}

void ScaleCollection::setScale(const PhysicalQuantity &quantity, const double &scaleMin, const double &scaleMax, const std::string& displayName)
{
    m_scales[quantity] = {scaleMin, scaleMax, displayName};
    if (m_currentQuantity == quantity)
    {
        m_currentScalePtr = &m_scales[quantity];
    }
}

void ScaleCollection::setScaleMin(const PhysicalQuantity &quantity, const double &scaleMin)
{
    Scale& scale = m_scales[quantity];
    scale.m_scaleMin = scaleMin;
}

void ScaleCollection::setScaleMax(const PhysicalQuantity &quantity, const double &scaleMax)
{
    Scale& scale = m_scales[quantity];
    scale.m_scaleMax = scaleMax;
}


QColor ScaleCollection::getColor(const PhysicalQuantity &quantity, const double &value) const
{
    auto finder = m_scales.find(quantity);
    if (finder == m_scales.end())
    {
        return Qt::black;
    }
    return getColor(finder->second, value);
}

QColor ScaleCollection::getColorFromCurrentScale(const double &value) const
{
    return getColor(*m_currentScalePtr, value);
}

const std::map<PhysicalQuantity, ScaleCollection::Scale> &ScaleCollection::getAllScales() const
{
    return m_scales;
}

QColor ScaleCollection::getColor(const ScaleCollection::Scale &scale, const double &value) const
{
    if (value <= scale.m_scaleMin)
    {
        return QColor(0, 0, 255);
    }
    else if (value > scale.m_scaleMax)
    {
        return QColor(255, 0, 0);
    }
    else
    {
        int scaledValue = (int)((value - scale.m_scaleMin) * 255. / (scale.m_scaleMax - scale.m_scaleMin));
        return QColor(scaledValue, 0, 255 - scaledValue);
    }
}
