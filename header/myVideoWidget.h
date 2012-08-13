#ifndef MYVIDEOWIDGET_H
#define MYVIDEOWIDGET_H
#include<phonon/VideoWidget>
#include<phonon/MediaObject>
#include<QtGui>
class CentralWidget;
class myVideoWidget:public Phonon::VideoWidget
{
    Q_OBJECT
public:
    myVideoWidget(QWidget *parent=0);
    void setParentWidget(CentralWidget *);
private:
    CentralWidget *cWidget;
protected:
    void keyReleaseEvent(QKeyEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
};

#endif // MYVIDEOWIDGET_H
