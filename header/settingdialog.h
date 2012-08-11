#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include"header/centralwidget.h"
namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingDialog(CentralWidget*,QWidget *parent = 0);
    ~SettingDialog();
    
private:
    Ui::SettingDialog *ui;
    CentralWidget *player;
    QString sopcast;
    int inPort,outPort;
    QString url;
public slots:
    void handleSopClick();
    void handleTextChanged(QString);
    void handleUrlChanged(QString);
    void handleConfirm();
};

#endif // SETTINGDIALOG_H
