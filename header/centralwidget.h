#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H
#include<QtGui>
#include"header/myVideoWidget.h"
#include<phonon/VolumeSlider>
#include<phonon/MediaObject>
#include<phonon/AudioOutput>
#include"header/channellist.h"
#include"header/spprocess.h"
class SettingDialog;

class CentralWidget:public QSplitter
{
    Q_OBJECT
public:
    friend class SettingDialog;
    CentralWidget(QWidget *parent=0);
    ChannelList *channelList;
    void initVideo(QString);
    void setSource(QString);
    bool showFlag;
    void readConfig();
    void saveConfig();
private:
    myVideoWidget *videoWidget;
    Phonon::VolumeSlider *volumeSlider;
    Phonon::MediaObject *mediaObject;
    Phonon::AudioOutput *audioOutput;
    QToolButton *playButton;
    QToolButton *stopButton;
    QToolButton *prevButton;
    QToolButton *nextButton;
    QToolButton *ShowOrHide;
    QToolButton *fullScreenButton;
    QLabel *infoLabel;
    QProgressBar *progressBar;
    QGroupBox *groupBox,*box;
    QHBoxLayout *hlayout;
    QVBoxLayout *vlayout;
    QNetworkAccessManager *nm;
    SpProcess *process;
    QString xmlUrl;
    bool playFlag;
    void parseConfig(QString);
public slots:
    void handleStop();
    void handleChoose(QTreeWidgetItem*,int);
    void handlePlayOrPause(bool);
    void handlePlayClick();
    void handleFullScreen();
    void handleShowOrHide();
    void handlePlay();
    void handlePrev();
    void handleNext();
    void handlePlayState(Phonon::State newstate, Phonon::State oldstate);
    void handleListReady(bool);
    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // CENTRALWIDGET_H
