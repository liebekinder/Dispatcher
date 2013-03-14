#include "connection.h"
#include "server.h"
#include "Parameters.h"

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

Connection::Connection(Server * s, int sock, QUuid id, QObject *parent) :
    QObject(parent)
{
    server_ = s;
    socket_ = sock;
    uid_ = id;
}

QUuid Connection::getuid()
{
    return uid_;
}

/**
 * Management of a client disconnection
 * @brief Connection::askQuit
 */
void Connection::askQuit()
{
    server_->removeConnection(uid_);
    emit finished();
}
/**
 * management of a client request to do work
 * include file reception, work and sending the result
 * @brief Connection::askWork
 */
void Connection::askWork()
{
    work();
}

void Connection::work(){
    char bufMessage[BUFFER_MESSAGE_SIZE];
    bcopy("WORK-OK", bufMessage, BUFFER_MESSAGE_SIZE);
    qDebug()<<"Work accepted";

    if(write(socket_,bufMessage,BUFFER_MESSAGE_SIZE) <0){
       this->error("Cannot send message: ");
       exit(1);
    }

    int fileW;
    if((fileW = fileWeight()) > 0){
        qDebug()<<"Waiting for a file of "+QString::number(fileW)+" o";
        tempFile_ = QString::number(qrand() % 50000);
        receiveFile(tempFile_,fileW);
    }
}

void Connection::sendMessage(char * s)
{
    char bufMessage[BUFFER_MESSAGE_SIZE];
    bcopy(s, bufMessage, BUFFER_MESSAGE_SIZE);

    if(write(socket_,bufMessage,BUFFER_MESSAGE_SIZE) <0){
       this->error("Cannot send message: ");
       exit(1);
    }
}

void Connection::receiveMessage(char * s)
{
    int longueur;
    char bufMessage[BUFFER_MESSAGE_SIZE];
    if((longueur = ::read(socket_, bufMessage, BUFFER_MESSAGE_SIZE -1)) > 0){
    }
    bcopy(bufMessage, s, BUFFER_MESSAGE_SIZE);

}

/**
 * receive the client file
 * @brief Connection::receiveFile
 * @param path
 * @param size
 */
void Connection::receiveFile(QString path, int size){

    char trame[BUFFER_FRAME];
    int longueur;
    int readByte = 0;

    QFile f(path);
    f.open(QIODevice::ReadWrite|QIODevice::Truncate);
    f.close();
    if(f.open(QIODevice::WriteOnly|QIODevice::Append)){

        while(readByte < size){
            longueur = read(socket_, trame, BUFFER_FRAME);
            if(longueur > 0){
                f.write(trame,longueur);
                readByte+=longueur;
            }
            else{
                if(longueur == -1) qDebug()<<"didn't manage to read this one";
                else{
                    qDebug()<<"trasnmisison's over!";
                    readByte=size;
                }
            }
        }
        //loop
    }
}
/**
 * read the client file size
 * @brief Connection::fileWeight
 * @return
 */
int Connection::fileWeight()
{
    char response[30] = "";
    if((::read(socket_, response, sizeof(response))) > 0){
    }
    else{
        qDebug()<<"didn't work well :(";
    }

    // send aknowledgment
    char bufMessage[BUFFER_MESSAGE_SIZE];
    bcopy("SIZE-OK", bufMessage, BUFFER_MESSAGE_SIZE);

    if(write(socket_,bufMessage,BUFFER_MESSAGE_SIZE) <0){
       this->error("Cannot send message: ");
       exit(1);
    }

    return atoi(response);
}

/**
 * main routine for message managment
 * wait until
 * @brief Connection::run
 */
void Connection::run()
{
    qDebug()<<"connection started!";

    //initialization

    //end of initialization

    char bufMessage[BUFFER_MESSAGE_SIZE];
    bool ETATCO = server_->slotAvailable();

    if(ETATCO) bcopy("CONNECT-OK", bufMessage, BUFFER_MESSAGE_SIZE);
    else bcopy("CONNECT-NO", bufMessage, BUFFER_MESSAGE_SIZE);

    if(write(socket_,bufMessage,BUFFER_MESSAGE_SIZE) <0){
       this->error("Cannot send message to client: "+uid_.toString());
       exit(1);
    }

    if(ETATCO){
        bool again = true;
        while(again){
            //while the client is still alive

            qDebug()<<"Waiting for question from client";
            int length;
            memset(bufMessage,'\0',BUFFER_MESSAGE_SIZE);
            if((length = ::read(socket_, bufMessage, BUFFER_MESSAGE_SIZE -1)) > 0){                
                qDebug()<<bufMessage;
                if(strcmp(bufMessage,"DISCONNECT") == 0){
                    //client asking to quit
                    qDebug()<<"stop message";
                    again = false;
                    server_->removeConnection(uid_);
                }
                else if(strcmp(bufMessage,"WORK") == 0){
                    //work to do!
                    askWork();
                }
                else{
                    qDebug()<<"unknown message";
                    again = false;
                    this->error("the following instruction doesn't make sense: ");
                }
            }
            else{
                qDebug()<<"client disconnecting";
                again = false;
            }
        }


    }

    close(socket_);
    qDebug()<<"closing socket...";

    emit finished();

}
