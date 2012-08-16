#include"header/centralwidget.h"
CentralWidget::CentralWidget(QWidget *parent):QSplitter(parent)
{
    showFlag=true;
    channelList=0;
    process=new SpProcess(this);
    readConfig();
    channelList=new ChannelList(xmlUrl,this);
    box=new QGroupBox(this);
    groupBox=new QGroupBox(box);
    infoLabel=new QLabel(groupBox);
    infoLabel->setText(QObject::tr("Disconnected"));
    progressBar=new QProgressBar(groupBox);
    progressBar->setEnabled(false);
    ShowOrHide=new QToolButton(groupBox);
    ShowOrHide->setIcon(QIcon(":/resource/pic/hide.png"));
    ShowOrHide->setToolTip(QObject::tr("Hide Channel List"));
    fullScreenButton=new QToolButton(groupBox);
    fullScreenButton->setIcon(QIcon(":/resource/pic/fullscreen.png"));
    fullScreenButton->setToolTip(QObject::tr("Enter Full Screen"));
    prevButton=new QToolButton(groupBox);
    prevButton->setIcon(QIcon(":/resource/pic/prev.png"));
    prevButton->setToolTip(QObject::tr("Previous Channel"));
    prevButton->setEnabled(false);
    playButton=new QToolButton(groupBox);
    playButton->setIcon(QIcon(":/resource/pic/play.png"));
    playButton->setToolTip(QObject::tr("Play"));
    playButton->setEnabled(false);
    stopButton=new QToolButton(groupBox);
    stopButton->setIcon(QIcon(":/resource/pic/stop.png"));
    stopButton->setToolTip(QObject::tr("Stop"));
    nextButton=new QToolButton(groupBox);
    nextButton->setIcon(QIcon(":/resource/pic/next.png"));
    nextButton->setToolTip(QObject::tr("Next Channel"));
    nextButton->setEnabled(false);
    volumeSlider=new Phonon::VolumeSlider(groupBox);
    volumeSlider->setFixedWidth(100);

    hlayout=new QHBoxLayout(groupBox);
    hlayout->setSpacing(0);
    hlayout->setMargin(0);
    hlayout->addWidget(ShowOrHide);
    hlayout->addWidget(fullScreenButton);
    hlayout->addWidget(prevButton);
    hlayout->addWidget(playButton);
    hlayout->addWidget(stopButton);
    hlayout->addWidget(nextButton);
    hlayout->addWidget(infoLabel);
    hlayout->addWidget(progressBar);
    hlayout->addWidget(volumeSlider);
    groupBox->setLayout(hlayout);
    groupBox->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    videoWidget=new myVideoWidget(box);
    vlayout=new QVBoxLayout(box);
    vlayout->addWidget(videoWidget);
    vlayout->addWidget(groupBox);
    box->setLayout(vlayout);
    box->setStyleSheet(QString("QGroupBox {border:0px;}"));

    this->addWidget(channelList);
    this->addWidget(box);

    nm=new QNetworkAccessManager(this);//
    nm->setNetworkAccessible(QNetworkAccessManager::Accessible);//
    mediaObject=new Phonon::MediaObject(this);
    videoWidget->setParentWidget(this);
    audioOutput=new Phonon::AudioOutput(Phonon::VideoCategory,this);
    volumeSlider->setAudioOutput(audioOutput);
    volumeSlider->setEnabled(false);
    playFlag=false;

    connect(channelList->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(handleChoose(QTreeWidgetItem*,int)));
    connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(handleProcessFinished(int,QProcess::ExitStatus)));
    connect(channelList,SIGNAL(listReady(bool)),this,SLOT(handleListReady(bool)));
    connect(prevButton,SIGNAL(clicked()),this,SLOT(handlePrev()));
    connect(playButton,SIGNAL(clicked()),this,SLOT(handlePlayClick()));
    connect(stopButton,SIGNAL(clicked()),this,SLOT(handleStop()));
    connect(nextButton,SIGNAL(clicked()),this,SLOT(handleNext()));
    connect(fullScreenButton,SIGNAL(clicked()),this,SLOT(handleFullScreen()));
    connect(ShowOrHide,SIGNAL(clicked()),this,SLOT(handleShowOrHide()));
}

void CentralWidget::initVideo(QString u)
{
    setSource(u);
    Phonon::createPath(mediaObject,videoWidget);
    Phonon::createPath(mediaObject,audioOutput);
    playButton->setEnabled(true);
}

void CentralWidget::handlePlayOrPause(bool b)
{
    playFlag=b;
    QIcon i(playFlag?":/resource/pic/pause.png":":/resource/pic/play.png");
    playButton->setIcon(i);
    playButton->setToolTip(playFlag?tr("Pause"):tr("Play"));
    playFlag?mediaObject->play():mediaObject->pause();
}

void CentralWidget::handleFullScreen()
{
    if(videoWidget->isFullScreen())
    {
        videoWidget->setFullScreen(false);
        fullScreenButton->setToolTip(tr("Enter Full Screen"));
    }
    else
    {
        videoWidget->setFullScreen(true);
        fullScreenButton->setToolTip(tr("Exit Full Screen"));
    }
}

void CentralWidget::handleShowOrHide()
{
    showFlag=!showFlag;
    channelList->setVisible(showFlag);
    ShowOrHide->setIcon(showFlag?QIcon(":/resource/pic/hide.png"):QIcon(":/resource/pic/show.png"));
    ShowOrHide->setToolTip(showFlag?tr("Hide Channel List"):tr("Show Channel List"));
}

void CentralWidget::handlePlayState(Phonon::State newstate, Phonon::State oldstate)
{
    if(newstate==Phonon::ErrorState)
        mediaObject->play();
}

void CentralWidget::handleChoose(QTreeWidgetItem *t, int c)

{
    readConfig();
    channelList->treeWidget->setCurrentItem(t);
    QString channel=t->text(c);
    QString url=channelList->allChannels[channel];
    if(url.isEmpty())
        return;
    handleStop();
    connect(process,SIGNAL(readyRead()),process,SLOT(parseState()));
    connect(process,SIGNAL(readyRead()),process,SLOT(parseOut()));
    if(process->startSp(url))
    {
        QString pu(process->getPlayUrl());
        initVideo(pu);
        infoLabel->setText(tr("Connecting..."));
        connect(process,SIGNAL(readyPlay()),this,SLOT(handlePlay()));
        connect(process,SIGNAL(progressInfo(int)),progressBar,SLOT(setValue(int)));
    }
}

void CentralWidget::handleStop()
{
    mediaObject->stop();
    if(process->state()!=QProcess::NotRunning)
    {
        process->close();
        handlePlayOrPause(false);
    }
    infoLabel->setText(tr("Disconnected"));
    progressBar->reset();
    progressBar->setEnabled(false);
    playButton->setEnabled(false);
    volumeSlider->setEnabled(false);
    disconnect(process,SIGNAL(readyRead()),process,SLOT(parseOut()));
    disconnect(process,SIGNAL(progressInfo(int)),progressBar,SLOT(setValue(int)));
}

void CentralWidget::setSource(QString u)
{
    QUrl url(u);
    Phonon::MediaSource mediaSource(u);
    mediaObject->clear();
    mediaObject->setCurrentSource(mediaSource);
}

void CentralWidget::handlePlay()
{
    disconnect(process,SIGNAL(readyRead()),process,SLOT(parseState()));
    handlePlayOrPause(true);
    infoLabel->setText(tr("Buffering..."));
    volumeSlider->setEnabled(true);
    progressBar->setEnabled(true);
}

void CentralWidget::handlePrev()
{
    QTreeWidgetItem *i,*j;
    i=channelList->treeWidget->currentItem();
    if(i!=NULL)
    {
        j=channelList->treeWidget->itemAbove(i);
        if(j!=NULL)
            handleChoose(j,0);
    }
}

void CentralWidget::handleNext()
{
    QTreeWidgetItem *i,*j;
    i=channelList->treeWidget->currentItem();
    if(i!=NULL)
    {
        j=channelList->treeWidget->itemBelow(i);
        if(j!=NULL)
            handleChoose(j,0);
    }
}

void CentralWidget::handleListReady(bool b)
{
    prevButton->setEnabled(b);
    nextButton->setEnabled(b);
}

void CentralWidget::handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    handleStop();
}

void CentralWidget::handlePlayClick()
{
    qreal volume=audioOutput->volume();
    handlePlayOrPause(!playFlag);
    audioOutput->setVolume(volume);
}

void CentralWidget::readConfig()
{
    QString config((QDir::homePath().append("/.config/QSLive/config")));
    if(QFile::exists(config))
        parseConfig(config);
    else
        parseConfig(":/resource/config/config");
}

void CentralWidget::parseConfig(QString config)
{
    QFile f(config);
    if(f.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QString sop(f.readLine());
        int i=sop.indexOf('=');
        process->setBin(sop.mid(i+1).trimmed());
        QString url(f.readLine());
        i=url.indexOf('=');
        xmlUrl=url.mid(i+1).trimmed();
        if(channelList!=0)
            channelList->setListUrl(xmlUrl);
        QString inport(f.readLine());
        i=inport.indexOf('=');
        int inPort=inport.mid(i+1).toInt();
        QString outport(f.readLine());
        i=outport.indexOf('=');
        int outPort=outport.mid(i+1).toInt();
        if(inPort&&outPort)
            process->setPort(true,inPort,outPort);
        f.close();
    }
}

void CentralWidget::saveConfig()
{
    QString config((QDir::homePath().append("/.config/QSLive/config")));
    QFile f(config);
    if(f.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QStringList ports=process->getPorts();
        QTextStream out(&f);
        out<<"sopcast="<<process->getBin()<<"\n";
        out<<"channelxml="<<channelList->getListUrl()<<"\n";
        out<<"inport="<<ports.first()<<"\n";
        out<<"outport="<<ports.last()<<"\n";
        f.close();
    }
    else
        qDebug()<<"Failed to Save config.";
}
