#include "SaveAsAction.h"

#include "Menu.h"

#include <MainView.h>

#include <QFileDialog>

SaveAsAction::SaveAsAction(double squareBreadth, Menu* parentMenu, MainView* mainView) :
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

void SaveAsAction::executeAction()
{
    QString dir("C:/Users/foehn/OneDrive/Documents/dump/");
    QString fileName = QFileDialog::getSaveFileName(m_mainView, QFileDialog::tr("Save File"),
                               dir,
                               QFileDialog::tr("Circuit files (*.crc)"));
    m_mainView->saveToFile(fileName);
}

bool SaveAsAction::isAvailable() const
{
    return m_parentMenu->isAvailableGeneral();
}
