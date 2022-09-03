#include "ToolboxArea.h"

#include "ComponentTI.h"

#include <MainView.h>

ToolboxArea::ToolboxArea(double squareBreadth, MainView* mainView) :
    m_squareBreadth(squareBreadth),
    m_mainView(mainView),
    m_currSelected(-1)
{
}

void ToolboxArea::pushBackItem(ComponentTI *toolboxItem)
{
    m_toolboxItemList.push_back(toolboxItem);
    toolboxItem->setParentItem(this);
    toolboxItem->setToolboxArea(this);
    toolboxItem->setIndex(m_toolboxItemList.size() - 1);
    toolboxItem->setPos(0.5 * m_squareBreadth, (m_toolboxItemList.size() - 0.5) * m_squareBreadth);

}

void ToolboxArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

QRectF ToolboxArea::boundingRect() const
{
    return QRectF(0., 0., m_squareBreadth, m_squareBreadth*m_toolboxItemList.size());
}

void ToolboxArea::setSelectedItem(int index)
{
    if (index == m_currSelected)
    {
        return;
    }
    m_currSelected = index;
    for (int i = 0; i < m_toolboxItemList.size(); ++i)
    {
        if (i == m_currSelected)
        {
            m_toolboxItemList[i]->setSelected(true);
        }
        else
        {
            m_toolboxItemList[i]->setSelected(false);
        }
    }
    if (m_currSelected == 0)
    {
        m_mainView->setCurrentMouseMode(MouseModes::NeutralMM);
    }
    else if (m_currSelected == 1)
    {
        m_mainView->setCurrentMouseMode(MouseModes::EraserMM);
    }
    else if (m_currSelected == 2)
    {
        m_mainView->setCurrentMouseMode(MouseModes::WireMM);
    }
    else if (m_currSelected == 3)
    {
        m_mainView->setCurrentMouseMode(MouseModes::CornerWireMM);
    }
    else if (m_currSelected == 4)
    {
        m_mainView->setCurrentMouseMode(MouseModes::ForkMM);
    }
    else if (m_currSelected == 5)
    {
        m_mainView->setCurrentMouseMode(MouseModes::BatteryMM);
    }
    else if (m_currSelected == 6)
    {
        m_mainView->setCurrentMouseMode(MouseModes::BulbMM);
    }
}

void ToolboxArea::setIsAvailable(bool isAvailable)
{
    if (m_isAvailable == isAvailable)
    {
        return;
    }
    m_isAvailable = isAvailable;

    if (!isAvailable)
    {
        m_mainView->setCurrentMouseMode(MouseModes::NeutralMM);
    }
    else
    {
        setSelectedItem(m_currSelected);
    }

    for (auto& item : m_toolboxItemList)
    {
        item->setIsAvailable(isAvailable);
    }
    update();
}

