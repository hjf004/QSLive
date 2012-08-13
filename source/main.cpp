#include"header/mainwindow.h"
int main(int argc,char **argv)
{
    QApplication app(argc,argv);
    app.setApplicationName("QSLive");
    app.setStyle(new QPlastiqueStyle);
    QTranslator tran;
    QLocale locale=QLocale::system();
    if(locale.language()==QLocale::Chinese)
        tran.load(":/resource/locale/zh_CN.qm");
    if(!tran.isEmpty())
        app.installTranslator(&tran);
    QString configDir(QDir::homePath().append("/.config/QSLive"));
    QDir d(configDir);
    if(!d.exists())
        d.mkdir(configDir);
    MainWindow *mwin=new MainWindow();
    mwin->showMaximized();
    return app.exec();
}
