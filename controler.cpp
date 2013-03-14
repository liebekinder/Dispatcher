#include "controler.h"
#include "window.h"
#include "server.h"
#include "client.h"

#include <string>
#include <iostream>

Controler::Controler(Window *f, QObject *parent) :
    QObject(parent)
{
    window_ = f;
    mode_ = "undefined";
    port_ = 0;

}

/**
 * Quit event manager
 * Ask user if they really want to quit
 * if yes, run cleaning subroutine to close network connection
 * @brief Controler::close
 * @param event
 */
void Controler::close(QCloseEvent *event)
{
    if(mode_ == "server"){
        if(QMessageBox::question(window_,
                                 tr("Are you sure?"),
                                 tr("You are about to close the server.\n"
                                    "All connected client will be disconnected and all task will be deleted.\n\n"
                                    "Do you really want to shutdown the server?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
            closingServer();
            event->accept();
        }
        else{
            event->ignore();
        }
    }

    else if(mode_ == "client"){
        if(QMessageBox::question(window_,
                                 tr("Are you sure?"),
                                 tr("You are about to quit.\n"
                                    "All running task will be canceled.\n\n"
                                    "Do you really want to quit?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
            event->accept();
        }
        else{
            event->ignore();
        }
    }
    else{
        event->accept();
    }
}

QString Controler::getMode()
{
    return mode_;
}

int Controler::getPort()
{
    return port_;
}

const char *Controler::getIPaddress()
{
    return ipServer_.toStdString().c_str();
}

QString Controler::getIn()
{
    return clientIn_;
}

QString Controler::getOut()
{
    return clientOut_;
}

/**
 * This method is used when asking to close the server
 * It closes all socket cleanly so that every client knows that the server is down.
 * @brief Controler::closingServer
 */
void Controler::closingServer()
{
    qDebug()<<"Action before shuting down the server";
}

/**
 * Used when the client quit.
 * It closes the socket with server properly.
 * @brief Controler::closingClient
 */
void Controler::closingClient()
{
    qDebug()<<"Action before quiting the client";
}

/**
 * Slot launching the server
 * Actually creating an instance of Server and push it into a thread
 * The thread is then started
 * @brief Controler::runServer
 */
void Controler::runServer()
{
    port_ = window_->getPort();
    mode_ = "server";
    qDebug()<<"launching server on port "+ QString::number(port_)+"...";

    //update GUI
    window_->setServerGui(port_);

    //create a server thread to avoid gui freeze

    server_ = new Server(this);
    serverThread_ = new QThread;
    server_->moveToThread(serverThread_);

    //action listeners

    connect(server_,SIGNAL(error(QString)),this,SLOT(serverError(QString)));
    connect(serverThread_,SIGNAL(started()),server_,SLOT(run()));
    connect(server_,SIGNAL(finished()),serverThread_,SLOT(quit()));
    connect(server_,SIGNAL(finished()),serverThread_,SLOT(deleteLater()));
    connect(serverThread_,SIGNAL(finished()),serverThread_,SLOT(deleteLater()));

    serverThread_->start();

}

/**
 * Managing the client launch
 * This methods stores the port and ask users to enter the server ip adress
 * Use it before "runClient"
 * @brief Controler::askClient
 */
void Controler::askClient()
{
    port_ = window_->getPort();
    mode_ = "client";

    window_->setClientGui(port_);
}

/**
 * Creation of a client into a thread
 * thread then started
 * @brief Controler::runClient
 */
void Controler::runClient()
{
    qDebug()<<"launching client on port "+ QString::number(port_)+"...";
    ipServer_ = window_->getIPAddress();

    //set gui
    window_->setClientGuiFinal(port_);

    //create a client thread to avoid gui freeze

    client_ = new Client(this);
    clientThread_ = new QThread;
    client_->moveToThread(clientThread_);

    connect(client_,SIGNAL(error(QString)),this,SLOT(clientError(QString)));
    connect(clientThread_,SIGNAL(started()),client_,SLOT(run()));
    connect(client_,SIGNAL(finished()),clientThread_,SLOT(quit()));
    connect(client_,SIGNAL(finished()),clientThread_,SLOT(deleteLater()));
    connect(clientThread_,SIGNAL(finished()),clientThread_,SLOT(deleteLater()));


    clientThread_->start();

}

void Controler::clientSendFile()
{
    clientIn_ = window_->getInputFile();
    clientOut_ = window_->getOutputFile();
    if(clientIn_ != "" && clientOut_ != ""){
        client_->sendFile();
    }
}

/**
 * Slot managing the server error through the thread
 * Only display the error
 * @brief Controler::serverError
 * @param s
 */
void Controler::serverError(QString s)
{
    qDebug()<<"server error: "+s;
}

/**
 * Slot managing the client error through the thread
 * Only display the error
 * @brief Controler::serverError
 * @param s
 */
void Controler::clientError(QString s)
{
    qDebug()<<"client error: "+s;
}


