#ifndef PARAMDIALOG_H
#define PARAMDIALOG_H

#include <QDialog>

#include "ComponentDI.h"

class QLineEdit;

class ParamDialog : public QDialog
{
public:
    ParamDialog(const std::vector<ComponentParam>& params);

    void save();

    std::vector<ComponentParam> getEditedParams() const
    {
        return m_params;
    }

private:
    std::vector<ComponentParam> m_params;
    std::vector<QLineEdit*> m_lineEdits;

    QPushButton* m_cancelBtn;
    QPushButton* m_okBtn;
};

#endif // PARAMDIALOG_H
