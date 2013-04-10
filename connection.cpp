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

    if(send(socket_,bufMessage,BUFFER_MESSAGE_SIZE,0) <0){
       this->error("Cannot send message: ");
       exit(1);
    }

    int fileW = fileWeight();
    qDebug()<<fileW;
    if(fileW > 0){
        qDebug()<<"Waiting for a file of "+QString::number(fileW)+" o";
        tempFile_ = QString::number(qrand() % 50000);
        receiveFile(tempFile_,fileW);
    }
}



/**
 * receive the client file
 * @brief Connection::receiveFile
 * @param path
 * @param size
 */
void Connection::receiveFile(QString path, int size){

    char trame[BUFFER_FRAME];
    char subbf[BUFFER_FRAME];
    int longueur;
    int readByte = 0;

    QFile f(path);
    f.open(QIODevice::ReadWrite|QIODevice::Truncate);
    f.close();
    if(f.open(QIODevice::WriteOnly|QIODevice::Append)){
    int i=0;
        while(readByte < size){
            longueur = recv(socket_, trame, BUFFER_FRAME,0);
            if(longueur > 0){
                if(i==0 && trame[0] =='\0'){
                    memcpy(subbf, &trame[1], BUFFER_FRAME);
                    subbf[BUFFER_FRAME-1] = '\0';
                }
                else{
                    memcpy(subbf, trame, BUFFER_FRAME);
                }
                f.write(subbf,longueur);
                readByte+=longueur;
                i++;
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

        //data receive complete
        char bufMessage[BUFFER_MESSAGE_SIZE];
        bcopy("RECEPT-OK", bufMessage, BUFFER_MESSAGE_SIZE);
        qDebug()<<"reception complete";

        if(send(socket_,bufMessage,BUFFER_MESSAGE_SIZE,0) <0){
           this->error("Cannot send message: ");
           exit(1);
        }
    }
}
/**
 * read the client file size
 * @brief Connection::fileWeight
 * @return
 */
int Connection::fileWeight()
{
    qDebug()<<"file weight...";
    char subbf[BUFFER_MESSAGE_SIZE];
    char response[BUFFER_MESSAGE_SIZE];
    memcpy(response, "", BUFFER_MESSAGE_SIZE);
    QThread::sleep(1);
    if((::recv(socket_, response, BUFFER_MESSAGE_SIZE,0)) > 0){
        if(response[0] =='\0'){
            memcpy(subbf, &response[1], BUFFER_MESSAGE_SIZE-1);
            subbf[BUFFER_MESSAGE_SIZE-1] = '\0';
        }
        else{
            memcpy(subbf, response, BUFFER_MESSAGE_SIZE);
        }
    }
    else{
        qDebug()<<"didn't work well :(";
    }

    // send aknowledgment
    char bufMessage[BUFFER_MESSAGE_SIZE];
    bcopy("SIZE-OK", bufMessage, BUFFER_MESSAGE_SIZE);

    if(send(socket_,bufMessage,BUFFER_MESSAGE_SIZE,0) <0){
       this->error("Cannot send message: ");
       exit(1);
    }
    int returnValue = atoi(subbf);
    return returnValue;
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

    if(send(socket_,bufMessage,BUFFER_MESSAGE_SIZE,0) <0){
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
            if((length = ::recv(socket_, bufMessage, BUFFER_MESSAGE_SIZE -1,0)) > 0){
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

void Connection::sendMessage(char * s)
{
    char bufMessage[BUFFER_MESSAGE_SIZE];
    bcopy(s, bufMessage, BUFFER_MESSAGE_SIZE);

    if(send(socket_,bufMessage,BUFFER_MESSAGE_SIZE,0) <0){
       this->error("Cannot send message: ");
       exit(1);
    }
}

void Connection::receiveMessage(char * s)
{
    int longueur;
    char bufMessage[BUFFER_MESSAGE_SIZE];
    if((longueur = ::recv(socket_, bufMessage, BUFFER_MESSAGE_SIZE,0)) > 0){
    }
    bcopy(bufMessage, s, BUFFER_MESSAGE_SIZE);

}
