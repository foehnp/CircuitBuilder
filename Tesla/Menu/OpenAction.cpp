#include "OpenAction.h"

#include <MainView.h>

#include <QFileDialog>

OpenAction::OpenAction(double squareBreadth, Menu* parentMenu, MainView* mainView) :
    MenuAction(squareBreadth, parentMenu, mainView)
{

}

void OpenAction::executeAction()
{
    QString dir("C:/Users/foehn/OneDrive/Documents/dump");
    QString fileName = QFileDialog::getOpenFileName(m_mainView, QFileDialog::tr("Open File"),
                               dir,
                               QFileDialog::tr("Circuit files (*.crc)"));
    m_mainView->loadFromFile(fileName);
}
