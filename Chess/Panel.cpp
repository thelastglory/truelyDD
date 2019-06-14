#include "Panel.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include<QMessageBox>
#include<QApplication>

CtrlPanel::CtrlPanel(QWidget *parent) : QWidget(parent)
{
    QGridLayout* grid = new QGridLayout(this);
    QVBoxLayout* vLay = new QVBoxLayout;
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(2, 1);
    grid->setRowStretch(0, 1);
    grid->setRowStretch(2, 1);
    grid->addLayout(vLay, 1, 1);

    vLay->addWidget(this->pBack = new QPushButton("悔棋"));
    vLay->addWidget(this->pSurrender = new QPushButton("投降"));

    connect(this->pBack, SIGNAL(clicked()), this, SIGNAL(sigBack()));
    connect(this->pSurrender, SIGNAL(clicked()), this, SLOT(Surrend()));
}

void CtrlPanel::Surrend()
{
    QObject* s = sender();

    if(this->pSurrender == s){
       // close();
        QMessageBox::StandardButton ret;
        ret = QMessageBox::question(NULL, "yes or not", "你输了是否关闭");
        if(ret == QMessageBox::Yes){
             QApplication* app;
             app->exit(0);
        }

    }
}


CtrlPanel::~CtrlPanel()
{

}

