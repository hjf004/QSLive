#include"header/mainwindow.h"
#include"header/settingdialog.h"
#include"header/channeldialog.h"
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{
    player=new CentralWidget(this);
    setCentralWidget(player);

    QFile style(QString(":/resource/qss/style.qss"));
    style.open(QFile::ReadOnly);
    QTextStream filetext(&style);
    setStyleSheet(filetext.readAll());

    menuBar=new QMenuBar(this);

    fileMenu=new QMenu(QObject::tr("&File"),menuBar);
    openAction=new QAction(QObject::tr("&Open Url"),fileMenu);
    openAction->setShortcut(QKeySequence(QKeySequence::Open));
    exitAction=new QAction(QObject::tr("&Quit"),fileMenu);
    exitAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
    fileMenu->addAction(openAction);
    fileMenu->addAction(exitAction);
    menuBar->addMenu(fileMenu);

    editMenu=new QMenu(QObject::tr("&Edit"),menuBar);
    settingsAction=new QAction(QObject::tr("&Settings"),editMenu);
    resetAction=new QAction(QObject::tr("&Reset"),editMenu);
    editMenu->addAction(settingsAction);
    editMenu->addAction(resetAction);
    menuBar->addMenu(editMenu);

    viewMenu=new QMenu(QObject::tr("&View"),menuBar);
    fullScreenAction=new QAction(QObject::tr("Play on &Fullscreen"),viewMenu);
    hideListAction=new QAction(QObject::tr("&Hide Channel List"),viewMenu);
    viewMenu->addAction(fullScreenAction);
    viewMenu->addAction(hideListAction);
    menuBar->addMenu(viewMenu);

    favoriteMenu=new QMenu(QObject::tr("Favorite"),menuBar);
    addToListAction=new QAction(QObject::tr("&Add to Favorite Channel  List"),favoriteMenu);
    addToListAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_A));
    clearAction=new QAction(QObject::tr("&Clear Favorite Channel List"),favoriteMenu);
    favoriteMenu->addAction(addToListAction);
    favoriteMenu->addAction(clearAction);
    menuBar->addMenu(favoriteMenu);

    helpMenu=new QMenu(QObject::tr("&Help"),menuBar);
    aboutAction=new QAction(QObject::tr("About"),helpMenu);
    aboutQtAction=new QAction(QObject::tr("About Qt"),helpMenu);
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
    menuBar->addMenu(helpMenu);

    connect(openAction,SIGNAL(triggered()),this,SLOT(handleOpen()));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(handleQuit()));
    connect(settingsAction,SIGNAL(triggered()),this,SLOT(handleSettings()));
    connect(resetAction,SIGNAL(triggered()),this,SLOT(handleReset()));
    connect(fullScreenAction,SIGNAL(triggered()),player,SLOT(handleFullScreen()));
    connect(hideListAction,SIGNAL(triggered()),this,SLOT(handleHideList()));
    connect(addToListAction,SIGNAL(triggered()),this,SLOT(handleAdd()));
    connect(clearAction,SIGNAL(triggered()),this,SLOT(handleClear()));
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(handleAbout()));
    connect(aboutQtAction,SIGNAL(triggered()),this,SLOT(handleAboutQt()));
    setMenuBar(menuBar);
    setWindowTitle(QObject::tr("QSLive--Online TV Player"));
}

void MainWindow::closeEvent(QCloseEvent *ce)
{
    player->handleStop();
    player->channelList->saveList();
    ce->accept();
}

void MainWindow::handleOpen()
{
    bool ok;
    QString url=QInputDialog::getText(this,QObject::tr("Please input the Channel Url"),QObject::tr("Url"),QLineEdit::Normal,QString("sop://"),&ok);
    QString u=url.trimmed();
    if(ok&&u.startsWith("sop://"))
    {
        QTreeWidgetItem *item=player->channelList->addToList(u,u);
        player->handleChoose(item,0);
    }
}

void MainWindow::handleQuit()
{
    this->close();
    exit(0);
}

void MainWindow::handleHideList()
{
    player->handleShowOrHide();
    if(player->showFlag)
        hideListAction->setText(QObject::tr("&Hide Channel List"));
    else
        hideListAction->setText(QObject::tr("&Show Channel List"));
}

void MainWindow::handleSettings()
{
    SettingDialog s(player,this);
    s.setWindowTitle(QObject::tr("Settings"));
    s.exec();
}

void MainWindow::handleAboutQt()
{
    QMessageBox::aboutQt(this,QObject::tr("About Qt"));
}

void MainWindow::handleReset()
{
    QString config((QDir::homePath().append("/.config/QSLive/config")));
    if(QFile::exists(config))
        QFile::remove(config);
}

void MainWindow::handleAdd()
{
    ChannelDialog cd(player,this);
    cd.setWindowTitle(QObject::tr("Add Channel To List"));
    cd.exec();
}

void MainWindow::handleClear()
{
    QTreeWidgetItem *item=player->channelList->treeWidget->topLevelItem(0);
    QList<QTreeWidgetItem *> items=item->takeChildren();
    item=0;
    foreach(item,items)
    {
        delete item;
    }
    player->channelList->saveList();
}

void MainWindow::handleAbout()
{
    QMessageBox::information(this,QObject::tr("About QSLive"),QObject::tr("QSLive is a front-end "
                 "of sopcast which allows you to watch free online TV channels,"
                 "and also to broadcast your own with peer to peer (P2P) technology."
                 "The linux version of sopcast only provide a command line back "
                 "-end,and you can only watch online tv with media player such as "
                 "mplayer or vlc.QSLive written with Qt makes it easy to watch online "
                 "tv with sopcast in linux."));
}
