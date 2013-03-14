#ifndef CONTROLER_H
#define CONTROLER_H

#include <QObject>
#include <QCloseEvent>
#include <QMessageBox>
#include <QThread>

class Window;
class Server;
class Client;

class Controler : public QObject
{
    Q_OBJECT
public:
    Controler(Window *f, QObject *parent = 0);

    //methods
    void close(QCloseEvent * event);
    QString getMode();
    int getPort();
    const char * getIPaddress();
    QString getIn();
    QString getOut();

private:
    Window * window_;
    QString mode_;
    int port_;
    Server * server_;
    QThread * serverThread_;
    Client * client_;
    QThread * clientThread_;
    QString ipServer_;
    QString clientIn_;
    QString clientOut_;

    //methods
    void closingServer();
    void closingClient();

signals:
    
public slots:
    void runServer();
    void askClient();
    void runClient();
    void clientSendFile();

    //thread output
    void serverError(QString s);
    void clientError(QString s);
};

#endif // CONTROLER_H
