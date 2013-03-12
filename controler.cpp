#include "controler.h"
#include "window.h"

Controler::Controler(Window *f, QObject *parent) :
    QObject(parent)
{
    window_ = f;
    mode_ = "undefined";

}

void Controler::close(QCloseEvent *event)
{
    if(mode_ == "server"){
        if(QMessageBox::question(0,
                                 tr("Are you sure?"),
                                 tr("You are about to close the server.\n"
                                    "All connected client will be disconnected and all task will be deleted.\n\n"
                                    "Do you really want to shutdown the server?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
            event->accept();
        }
        else{
            event->ignore();
        }
    }
    else if(mode_ == "client"){
        if(QMessageBox::question(0,
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

void Controler::runServer()
{
    int port = window_->getPort();
    mode_ = "server";
    qDebug()<<"launching server on port "+ QString::number(port)+"...";

    //update GUI
    window_->setServerGui(port);

    //create a server thread to avoid gui freeze

}

void Controler::runClient()
{
    int port = window_->getPort();
    mode_ = "client";
    qDebug()<<"launching client on port "+ QString::number(port)+"...";

    //update GUI
    window_->setClientGui(port);

    //create a server thread to avoid gui freeze
}


