#ifndef CHANNELDIALOG_H
#define CHANNELDIALOG_H

#include <QDialog>
#include"header/centralwidget.h"
namespace Ui {
class ChannelDialog;
}

class ChannelDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChannelDialog(CentralWidget *,QWidget *parent = 0);
    ~ChannelDialog();
    
private:
    Ui::ChannelDialog *ui;
    CentralWidget *player;
public slots:
    void handleOkClick();
};

#endif // CHANNELDIALOG_H
