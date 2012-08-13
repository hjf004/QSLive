#include"header/myVideoWidget.h"
#include"header/centralwidget.h"
myVideoWidget::myVideoWidget(QWidget *parent):Phonon::VideoWidget(parent)
{
}

void myVideoWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(this->isFullScreen())
        this->exitFullScreen();
    else
        this->setFullScreen(true);
}

void myVideoWidget::keyReleaseEvent(QKeyEvent *ke)
{
    qDebug()<<"Key "<<ke->key();
    if(isFullScreen())
    {
        if(ke->key()==Qt::Key_Escape)
            this->exitFullScreen();
        else if(ke->key()==Qt::Key_Up||ke->key()==Qt::Key_Down)
        {
           cWidget->handlePlayClick();
        }
        else if(ke->key()==Qt::Key_Right)
            cWidget->handleNext();
        else if(ke->key()==Qt::Key_Left)
            cWidget->handlePrev();
    }
}
void myVideoWidget::setParentWidget(CentralWidget *c)
{
    cWidget=c;
}
