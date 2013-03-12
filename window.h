#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QDebug>
#include <QListWidget>

class Controler;

class Window : public QMainWindow
{
    Q_OBJECT
    
public:
    Window(QWidget *parent = 0);
    ~Window();

    //getters
    int getPort();

    //GUI setting
    void setServerGui(int port);
    void setClientGui(int port);

    //listeners
    void closeEvent(QCloseEvent * event);
private:

    //data
    Controler * controler_;

    //initial selction window
    QWidget* centralWidget;
    QVBoxLayout * centralLayout;
    QPushButton * serverChoice;
    QPushButton * clientChoice;
    QHBoxLayout * portLayout;
    QSpinBox * numPort;
    QLabel * numPortLabel;

    //server
    QWidget * centralServer;
    QVBoxLayout * centralServerLayout;
    QLabel * serverStartInfo;
    QListWidget * listClientInfo;



};

#endif // WINDOW_H
