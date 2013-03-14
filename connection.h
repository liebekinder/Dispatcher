#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QDebug>
#include <QUuid>
#include <QFile>

class Server;

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection(Server * s, int sock, QUuid id, QObject *parent = 0);
    QUuid getuid();
    
private:
    int socket_;
    Server * server_;
    QUuid uid_;
    QString tempFile_;

    //methods
    void askQuit();
    void askWork();
    int fileWeight();
    void receiveFile(QString path, int size);
    void work();
    void sendMessage(char *s);
    void receiveMessage(char *s);

signals:
    void error(QString s);
    void finished();
    
public slots:
    void run();
    
};

#endif // CONNECTION_H
