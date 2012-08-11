#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QtGui>
#include"header/centralwidget.h"
class MainWindow:public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=0);
private:
    void closeEvent(QCloseEvent *);
    CentralWidget *player;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *favoriteMenu;
    QMenu *helpMenu;

    QAction *openAction;
    QAction *exitAction;
    QAction *settingsAction;
    QAction *resetAction;
    QAction *fullScreenAction;
    QAction *hideListAction;
    QAction *addToListAction;
    QAction *clearAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
public slots:
    void handleOpen();
    void handleQuit();
    void handleHideList();
    void handleSettings();
    void handleAboutQt();
    void handleReset();
    void handleAdd();
    void handleClear();
};

#endif // MAINWINDOW_H
