#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QUuid>
#include <QMutex>
#include <QList>

class Controler;
class Connection;

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(Controler *c, QObject *parent = 0);
    bool slotAvailable();
    void addConnection(Connection * c);
    void removeConnection(QUuid id);

private:
    Controler * controler_;
    QList<Connection *> * ConnectedClient_;
    QMutex mutex_;

signals:
    void error(QString s);
    void finished();
    
public slots:
    void run();
    void connectionError(QString s);
    
};

#endif // SERVER_H
