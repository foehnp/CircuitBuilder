#pragma once

#include <EnumsDefsEtc.h>

#include <map>

class ScaleCollection
{
public:
    struct Scale
    {
        double m_scaleMin = -1.;
        double m_scaleMax =  1.;
        std::string m_displayName =  "Unknown";

        Scale(double scaleMin, double scaleMax, const std::string& displayName) :
            m_scaleMin(scaleMin),
            m_scaleMax(scaleMax),
            m_displayName(displayName)
        {
        }

        Scale()
        {
        }
    };

public:
    ScaleCollection();

    void setCurrentQuantity(const PhysicalQuantity& quantity);
    PhysicalQuantity getCurrentQuantity() const;

    void setScale(const PhysicalQuantity& quantity, const double& scaleMin, const double& scaleMax, const std::string& displayName);

    void setScaleMin(const PhysicalQuantity& quantity, const double& scaleMin);
    void setScaleMax(const PhysicalQuantity& quantity, const double& scaleMax);

    QColor getColor(const PhysicalQuantity& quantity, const double& value) const;

    QColor getColorFromCurrentScale(const double& value) const;

    const std::map<PhysicalQuantity, Scale>& getAllScales() const;

private:
    QColor getColor(const Scale& scale, const double& value) const;

private:
    PhysicalQuantity m_currentQuantity;
    Scale* m_currentScalePtr;
    std::map<PhysicalQuantity, Scale> m_scales;
};
