#include "client.h"
#include "controler.h"
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

Client::Client(Controler *c, QObject *parent) :
    QObject(parent)
{
    controler_ = c;
}

/**
 * main routine of client
 *
 * @brief Client::run
 */
void Client::run()
{

    //initialization

    //end of initialization

    qDebug()<< "setting up client";

    sockaddr_in adresse_locale;
    hostent* ptr_hote;
    int longueur;
    int port = controler_->getPort();
    char bufMessage[BUFFER_MESSAGE_SIZE];

    if((ptr_hote = gethostbyname(controler_->getIPaddress())) == NULL){
               this->error("Unable to connect to server. Please check that your ip address is correct. Abort...");
               exit(1);
       }

    bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
    adresse_locale.sin_family = AF_INET;
    adresse_locale.sin_port = htons(port);

    qDebug()<<"socket initialized";

    if((socket_ = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            this->error("Unable to set the socket with TCP/IP on port: " + QString::number(ntohs(adresse_locale.sin_port)));
            exit(1);
    }

    //  :: is here to force the use of the method connect from socket.h
    // connect is also a method of QObject
    if((::connect(socket_, (sockaddr*)(&adresse_locale), (socklen_t)(sizeof(adresse_locale)))) < 0){
            this->error("Unable to link the socket with the client");
            exit(1);
    }

    qDebug()<< "client is running!";

    qDebug()<<"connection established!";

    //Waiting for server
    if((longueur = ::read(socket_, bufMessage, BUFFER_MESSAGE_SIZE)) > 0){
            //
            if(strcmp(bufMessage,"CONNECT-OK") == 0){
                //connection ok
                qDebug()<<"connection accepted!";

            }
            else if(strcmp(bufMessage,"CONNECT-NO") == 0){
                //connection no
                qDebug()<<"connection rejected! Too many clients";
                close(socket_);
            }
            else{
                qDebug()<<"The response doesn't make sense...";
                close(socket_);
            }
    }
}

void Client::sendFile()
{
    qDebug()<<"send a file!";
int longueur;
    char bufMessage[BUFFER_MESSAGE_SIZE];
    bcopy("WORK", bufMessage, BUFFER_MESSAGE_SIZE);

    if(::write(socket_,bufMessage,BUFFER_MESSAGE_SIZE) <0){
       this->error("Cannot send message to server: ");
       exit(1);
    }

    memcpy(bufMessage, "", BUFFER_MESSAGE_SIZE);
    if((longueur = ::read(socket_, bufMessage, BUFFER_MESSAGE_SIZE)) > 0){
        //
        qDebug()<<bufMessage;
        //problem with that buffer. All seems to be 1o to far...
        //here the value is \0WORK_OK the first time
        //then you add a \0 in front at every call
        //if sleep isn't here, bufMessage is bullshit the first time
        //then correct the second and the generate a sigpipe error "broken pipe" on the write upside.

        char subbf[BUFFER_MESSAGE_SIZE];
        if(bufMessage[0] =='\0'){
            memcpy(subbf, &bufMessage[1], BUFFER_MESSAGE_SIZE-1);
            subbf[BUFFER_MESSAGE_SIZE-1] = '\0';
        }
        else{
            memcpy(subbf, bufMessage, BUFFER_MESSAGE_SIZE);
        }

        //if(true){
        if(strcmp(subbf,"WORK-OK") == 0){
            qDebug()<<"Ready to go!";

            QFile f(controler_->getIn());
            if(f.open(QIODevice::ReadOnly)){
                qDebug()<<"file open";
                //int pos = 0;
                char data[BUFFER_FRAME];
                int sizeSend = f.size();
                int byteWritten;
                int byteRead = sizeSend;

                sendSize(sizeSend);

                while(!f.atEnd()){
                    byteRead = f.read(data, BUFFER_FRAME);
                    if(byteRead == 0) break;
                    if(byteRead < 0){
                        qDebug()<<"fail to write into buffer";
                        exit(1);
                    }
                    void* p = data;
                    while(byteRead >0){
                        qDebug()<<"sending a packet";
                        byteWritten = write(socket_,p,byteRead);
                        if(byteWritten <=0){
                            qDebug()<<"fail to send packet";
                            exit(1);
                        }
                        byteRead -= byteWritten;
                    }
                }

                //aknoledgment of correct received file
                memcpy(bufMessage, "", BUFFER_MESSAGE_SIZE);
                if((::read(socket_, bufMessage, BUFFER_MESSAGE_SIZE)) > 0){
                    if(bufMessage[0] =='\0'){
                        memcpy(subbf, &bufMessage[1], BUFFER_MESSAGE_SIZE-1);
                        subbf[BUFFER_MESSAGE_SIZE-1] = '\0';
                    }
                    else{
                        memcpy(subbf, bufMessage, BUFFER_MESSAGE_SIZE);
                    }
                        qDebug()<< subbf;
                }
            }
            else{
                qDebug()<<"unable to open file";
            }
        }
        else if(strcmp(subbf,"WORK-NO") == 0){
            qDebug()<<"No work wanted!";
        }
        else{
            qDebug()<<"The response doesn't make sense...";
            qDebug()<<subbf;
            //close(socket_);
        }
    }
    //lal
}

void Client::sendTrame(char* data){

    if(write(socket_,data,sizeof(data)) <0){
       qDebug()<<"Have not be able to send";
       exit(1);
    }

}

void Client::sendSize(int t){
    char tmp[BUFFER_MESSAGE_SIZE];
    memcpy(tmp, "", BUFFER_MESSAGE_SIZE);
    sprintf(tmp, "%i", t);
    qDebug()<<"file to send weight";
    qDebug()<<tmp;

    if(::write(socket_,tmp,BUFFER_MESSAGE_SIZE) <0){
       this->error("Unable to send");
       exit(1);
    }
    for(int i=0; i<BUFFER_MESSAGE_SIZE;++i){
        qDebug()<<tmp[i];
    }

    //wait for acquittement

    int longueur;
    char bufMessage[BUFFER_MESSAGE_SIZE];
    if((longueur = ::read(socket_, bufMessage, BUFFER_MESSAGE_SIZE)) > 0){
        qDebug()<<bufMessage;
    }



}
