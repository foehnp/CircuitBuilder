#include "Menu.h"

#include "MenuAction.h"
#include "SaveAction.h"
#include "SaveAsAction.h"
#include "OpenAction.h"

#include <MainView.h>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

Menu::Menu(MainView* mainView, double squareBreadth) :
    m_mainView(mainView),
    m_squareBreadth(squareBreadth)
{
    pushBackAction(new OpenAction(m_squareBreadth, this, m_mainView));
    pushBackAction(new SaveAction(m_squareBreadth, this, m_mainView));
    pushBackAction(new SaveAsAction(m_squareBreadth, this, m_mainView));
}

void Menu::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}

QRectF Menu::boundingRect() const
{
    return QRectF(0., 0., m_squareBreadth, m_squareBreadth);
}

void Menu::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void Menu::pushBackAction(MenuAction *menuAction)
{
    m_toolboxItemList.push_back(menuAction);
    menuAction->setParentItem(this);
    menuAction->setPos((m_toolboxItemList.size() - 1) * m_squareBreadth, 0.);
}

void Menu::setIsAvailableGeneral(bool isOn)
{
    m_isAvailableGeneral = isOn;
}

bool Menu::isAvailableGeneral() const
{
    return m_isAvailableGeneral;
}
