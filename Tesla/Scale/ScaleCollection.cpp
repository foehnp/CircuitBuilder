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
    double scaledValue = (value - scale.m_scaleMin) / (scale.m_scaleMax - scale.m_scaleMin);
    return getColorScaleFunc()(scaledValue);
}

ColorFunction ScaleCollection::getColorScaleFunc() const
{
    ColorFunction colorFunc = [](double param){
        param = 1. - param;
        if (param <= 0.)
        {
            return QColor(255, 0, 0);
        }
        else if (param >= 1.)
        {
            return QColor(0, 0, 255);
        }
        else if (param < 0.25)
        {
            unsigned char charParam = (unsigned char)(param * 1024);
            return QColor(255, charParam, 0);
        }
        else if (param < 0.5)
        {
            unsigned char charParam = (unsigned char)((param-0.25) * 1024);
            return QColor(255-charParam, 255, 0);
        }
        else if (param < 0.75)
        {
            unsigned char charParam = (unsigned char)((param-0.5) * 1024);
            return QColor(0, 255, charParam);
        }
        else
        {
            unsigned char charParam = (unsigned char)((param-0.75) * 1024);
            return QColor(0, 255-charParam, 255);
        }
    };

    return colorFunc;
}
