#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QFile>

class Controler;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(Controler * c, QObject *parent = 0);
    
private:
    Controler * controler_;
    int socket_;

    //methods
    void sendTrame(char* data);
    void sendSize(int t);


signals:
    void error(QString s);
    void finished();
    
public slots:
    void run();
    void sendFile();
    
};

#endif // CLIENT_H
