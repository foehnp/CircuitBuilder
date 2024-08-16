#include "OpenAction.h"

#include "Menu.h"

#include <MainView.h>

#include <QFileDialog>

OpenAction::OpenAction(double squareBreadth, Menu* parentMenu, MainView* mainView) :
    MenuAction(squareBreadth, parentMenu, mainView)
{
    QPixmap pM(getIconPath());
    pM.scaledToHeight(squareBreadth);
    QGraphicsPixmapItem* m_iconPixmapItem = new QGraphicsPixmapItem(pM);
    QRectF rect = m_iconPixmapItem->boundingRect();
    m_iconPixmapItem->setScale(squareBreadth / rect.height());
    m_iconPixmapItem->setPos(0., 0.);
    m_iconPixmapItem->setParentItem(this);
}

void OpenAction::executeAction()
{
    QString dir("C:/Users/foehn/OneDrive/Documents/dump");
    QString fileName = QFileDialog::getOpenFileName(m_mainView, QFileDialog::tr("Open File"),
                               dir,
                               QFileDialog::tr("Circuit files (*.crc)"));
    m_mainView->loadFromFile(fileName);
    m_parentMenu->setFilePath(fileName);
}

bool OpenAction::isAvailable() const
{
    return m_parentMenu->isAvailableGeneral();
}
