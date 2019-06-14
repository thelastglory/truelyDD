#include "Choose.h"
#include <QHBoxLayout>

ChooseDlg::ChooseDlg(QWidget *parent) : QDialog(parent)
{
    QHBoxLayout* pHLayout = new QHBoxLayout(this);
    pHLayout->addWidget(pButtons1 = new QPushButton("同盘对战"));
    pHLayout->addWidget(pButtons2 = new QPushButton("弟弟"));
    pHLayout->addWidget(pButtons3 = new QPushButton("弟弟的对手"));

    connect(pButtons1, SIGNAL(clicked()), this, SLOT(slotClicked()));
    connect(pButtons2, SIGNAL(clicked()), this, SLOT(slotClicked()));
    connect(pButtons3, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

void ChooseDlg::slotClicked()
{
    QObject* s = sender();

    if(pButtons1 == s)
    {
        this->_selected = 0;
    }
    else if(pButtons2 == s)
    {
        this->_selected = 1;
     }
    else if(pButtons3 == s)
    {
        this->_selected = 2;
    }
    accept();
}

ChooseDlg::~ChooseDlg()
{

}

