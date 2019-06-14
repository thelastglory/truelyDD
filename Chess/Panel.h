#ifndef CTRLPANEL_H
#define CTRLPANEL_H

#include <QWidget>
#include <QPushButton>

class CtrlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CtrlPanel(QWidget *parent = 0);
    ~CtrlPanel();

    QPushButton* pBack;
    QPushButton* pSurrender;

signals:
    void sigBack();

public slots:
    void Surrend();
};

#endif // CTRLPANEL_H
