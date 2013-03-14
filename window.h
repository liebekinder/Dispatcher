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
#include <QLineEdit>
#include <QFileDialog>

class Controler;

class Window : public QMainWindow
{
    Q_OBJECT
    
public:
    Window(QWidget *parent = 0);
    ~Window();

    //getters
    int getPort();
    QString getIPAddress();
    QString getInputFile();
    QString getOutputFile();

    //GUI setting
    void setServerGui(int port);
    void setClientGui(int port);
    void setClientGuiFinal(int port);

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

    //client
    QWidget * centralClientTemp;
    QVBoxLayout * centralClientTempLayout;
    QHBoxLayout * centralClientTempFormLayout;
    QLabel * centralClientPortInfo;
    QLabel * centralClientInfo;
    QLineEdit * IPType;
    QPushButton * IPTypeOk;
    //------
    QWidget * centralClient;
    QVBoxLayout * centralClientLayout;
    QLabel * clientLabelGeneral;
    QLabel * clientLabelInput;
    QLabel * clientLabelOutput;
    QLineEdit * clientLineEditInput;
    QLineEdit * clientLineEditOutput;
    QPushButton * clientButtonInput;
    QPushButton * clientButtonOutput;
    QPushButton * clientSend;
    QHBoxLayout * clientlayoutInput;
    QHBoxLayout * clientlayoutOutput;

public slots:
    void askFileIn();
    void askFileOut();

};

#endif // WINDOW_H
