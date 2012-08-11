#include "header/channeldialog.h"
#include "ui_channeldialog.h"

ChannelDialog::ChannelDialog(CentralWidget *c, QWidget *parent) :
    QDialog(parent),player(c),
    ui(new Ui::ChannelDialog)
{
    ui->setupUi(this);
    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(handleOkClick()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(close()));
}

ChannelDialog::~ChannelDialog()
{
    delete ui;
}

void ChannelDialog::handleOkClick()
{
    QString name=ui->nameInput->text().trimmed();
    QString url=ui->urlInput->text().trimmed();
    if(!name.isEmpty()&&url.startsWith("sop://"))
    {
        player->channelList->addToList(name,url);
        player->channelList->saveList();
    }
    else
        QMessageBox::warning(this,tr("Input Incorrect"),tr("Please input correct channel name and url!"));
    close();
}
