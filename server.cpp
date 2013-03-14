#include "server.h"
#include "controler.h"
#include "Parameters.h"
#include "connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;


Server::Server(Controler * c, QObject *parent) :
    QObject(parent)
{
    controler_ = c;
}

/**
 * return true whether there is space available for another client
 * using the global parameter NB_CLIENT_MAX
 *using mutex to avoid concurrency
 * @brief Server::slotAvailable
 * @return
 */
bool Server::slotAvailable()
{
    mutex_.lock();
    bool response = false;
    if(ConnectedClient_->size() < NB_CLIENT_MAX) response = true;
    mutex_.unlock();
    return response;
}

void Server::addConnection(Connection *c)
{
    mutex_.lock();
    ConnectedClient_->push_back(c);
    mutex_.unlock();
}

/**
 * Remove a Connection from the server.
 * The client will kill itself
 * @brief Server::removeConnection
 * @param id
 */
void Server::removeConnection(QUuid id)
{
    mutex_.lock();
    int position = -1;
    for(int i=0; i <ConnectedClient_->size();++i){
        if(ConnectedClient_->at(i)->getuid() == id) position = i;
    }
    if(position != -1) ConnectedClient_->removeAt(position);
    mutex_.unlock();
}

/**
 *this is the server main routine
 *he will listen to incoming connection
 *whatever they may be:
 * - asking for connection
 * - asking for disconnection (DISCONNECT)
 * - asking for a work (WORK)
 *each connection will have its own thread
 * @brief Server::run
 */
void Server::run()
{
    //initialization (not possible from constructor)

    ConnectedClient_ = new QList<Connection *>();

    //end of initialization

    qDebug()<<"the server is setting up!!";

    //setting up the server

    int initialSocket;
    sockaddr_in adresse_locale;
    hostent* ptr_hote;
    int incomingConnectionSocket;
    int longueur_adresse_courante;
    sockaddr_in  adresse_client_courant;

    char machine[256];
    gethostname(machine,SIZE_MAX_NOM);

    if((ptr_hote = gethostbyname(machine)) == NULL) //on récupère le pointeur vers hostent contenant entre autre le nom d'hote
    {
        this->error("Cannot retrieve server hostent by its name.Aborting...");
        exit(1);
    }

    qDebug()<<"socket creation...";
    bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
    adresse_locale.sin_family = ptr_hote->h_addrtype;
    adresse_locale.sin_addr.s_addr = INADDR_ANY;
    adresse_locale.sin_port = htons(controler_->getPort());
    qDebug()<<"Tether to the port " + QString::number(ntohs(adresse_locale.sin_port));
    qDebug()<<"socket created!";

    if((initialSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        qDebug()<<"Unable to set up the socket in TCP/IP. The port is probably already used.";
        this->error("Unable to set up the socket in TCP/IP. Aborting...");
        exit(1);
    }

    if((bind(initialSocket, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0){
           this->error("Unable to link the socket with the server. Abort...");
           exit(1);
       }
    qDebug()<<"socket linked to the server!";
    qDebug()<<"Server running!";

    qDebug()<<"Listening...";

    listen(initialSocket,NB_CLIENT_MAX);
    // écoute du port à travers le socket, NB_CLIENT connexion en attente max

    for(;;)
    {
        longueur_adresse_courante = sizeof(adresse_client_courant);
        if((incomingConnectionSocket = accept(initialSocket,(sockaddr*)(&adresse_client_courant),(socklen_t *) &longueur_adresse_courante)) <0){
            this->error("One client connection failed in an unusual way. THe server buffer may be full. Aborting... ");
            exit(1);
        }
        qDebug()<<"Incoming connection.";

        //launch new thread with incomingConnectionSocket as parameter

        QUuid id = QUuid::createUuid();
        Connection * newConnection = new Connection(this, incomingConnectionSocket, id);
        QThread * connectionThread = new QThread;
        newConnection->moveToThread(connectionThread);

        //action listeners
        connect(newConnection, SIGNAL(error(QString)), this, SLOT(connectionError(QString)));
        connect(connectionThread, SIGNAL(started()), newConnection, SLOT(run()));
        connect(newConnection,SIGNAL(finished()),connectionThread,SLOT(quit()));
        connect(newConnection,SIGNAL(finished()),connectionThread,SLOT(deleteLater()));
        connect(connectionThread,SIGNAL(finished()),connectionThread,SLOT(deleteLater()));

        qDebug()<<"starting...";
        connectionThread->start();
        qDebug()<<"started!";

    }

}

void Server::connectionError(QString s)
{
    qDebug()<<"Connection error: "+s;
}
