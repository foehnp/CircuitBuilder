#include "ParamDialog.h"


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleValidator>
#include <QPushButton>

ParamDialog::ParamDialog(const std::vector<ComponentParam>& params) :
    m_params(params)
{
    setWindowTitle("Parameters");
    QVBoxLayout *vLayOut = new QVBoxLayout(this);
    for (const ComponentParam& param : m_params)
    {
        QHBoxLayout *hLayOut = new QHBoxLayout(this);
        vLayOut->addLayout(hLayOut);


        QLineEdit* lineEdit = new QLineEdit();
        lineEdit->setText(QString::number(param.val, 'f', 3));
        lineEdit->setValidator(new QDoubleValidator(param.minVal, param.maxVal, param.decimals, this) );

        m_lineEdits.push_back(lineEdit);

        QLabel* nameLabel = new QLabel(param.displayName);
        QLabel* unitLabel = new QLabel(param.unit);

        hLayOut->addWidget(nameLabel);
        hLayOut->addWidget(lineEdit);
        hLayOut->addWidget(unitLabel);
    }

    QHBoxLayout *hLayOut = new QHBoxLayout(this);
    vLayOut->addLayout(hLayOut);

    m_cancelBtn = new QPushButton();
    m_cancelBtn->setText("Cancel");

    QObject::connect(m_cancelBtn, SIGNAL(clicked()), this, SLOT(reject()));

    m_okBtn = new QPushButton();
    m_okBtn->setText("Save changes");


    QObject::connect(m_okBtn, &QPushButton::clicked,
                         this, [this](){save();
        QDialog::done(QDialog::Accepted);});

    hLayOut->addWidget(m_okBtn);
    hLayOut->addWidget(m_cancelBtn);
    m_cancelBtn->setFocusPolicy(Qt::TabFocus);
}

void ParamDialog::save()
{
    for (int i = 0; i < m_params.size(); ++i)
    {
        m_params[i].val = m_lineEdits[i]->text().toDouble();
    }
}
