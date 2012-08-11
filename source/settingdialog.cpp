#include "header/settingdialog.h"
#include "ui_settingdialog.h"
#include<QtGui>
SettingDialog::SettingDialog(CentralWidget *c,QWidget *parent) :
    QDialog(parent),player(c),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    sopcast="sp-sc-auth";
    ui->sopInput->setText(sopcast);
    url="http://channel.sopcast.com/gchlxml";
    ui->urlLineEdit->setText(url);
    ui->inSpinBox->setMinimum(3000);
    ui->inSpinBox->setMaximum(65535);
    ui->outSpinBox->setMinimum(3000);
    ui->outSpinBox->setMaximum(65535);
    if(player->process->customPort)
    {
        ui->checkBox->setChecked(true);
        QStringList ports=player->process->getPorts();
        ui->inSpinBox->setValue(ports.first().toInt());
        ui->outSpinBox->setValue(ports.last().toInt());
    }
    else
        ui->groupBox->setEnabled(false);
    connect(ui->sopButton,SIGNAL(clicked()),this,SLOT(handleSopClick()));
    connect(ui->sopInput,SIGNAL(textChanged(QString)),this,SLOT(handleTextChanged(QString)));
    connect(ui->checkBox,SIGNAL(clicked(bool)),ui->groupBox,SLOT(setEnabled(bool)));
    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(handleConfirm()));
    connect(ui->cancelButon,SIGNAL(clicked()),this,SLOT(close()));
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::handleSopClick()
{
    QString s=QFileDialog::getOpenFileName(this,QString("Specify Sopcast"));
    if(!s.isEmpty())
     {
        ui->sopInput->setText(s);
    }
}

void SettingDialog::handleTextChanged(QString s)
{
    if(!s.isEmpty()&&s!=sopcast)
    {
        sopcast=s;
        qDebug()<<sopcast;
    }
}

void SettingDialog::handleUrlChanged(QString s)
{
    if(s.startsWith("http://"))
        return;
    if(s!=url)
        url=s;
}

void SettingDialog::handleConfirm()
{
    if(ui->checkBox->isChecked())
    {
        int i=ui->inSpinBox->value();
        int o=ui->outSpinBox->value();
        if(i==o)
        {
            QMessageBox::warning(this,tr("Invalid Settings"),tr("In port can't equal with out port,settings won't be applied!"));
            return;
        }
        player->process->setPort(true,i,o);
    }
    else
        player->process->setPort(false,0,0);
    player->process->setBin(sopcast);
    player->channelList->setListUrl(url);
    player->saveConfig();
    close();
}
