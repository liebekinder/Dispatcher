#include "window.h"
#include "controler.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    //controler set up

    controler_ = new Controler(this);

    //initialization of the selection window
    setWindowTitle(tr("Task Dispatcher"));

    centralWidget = new QWidget;
    centralLayout = new QVBoxLayout;
        serverChoice = new QPushButton(tr("Server: start"));
        clientChoice = new QPushButton(tr("Client: start"));
        portLayout = new QHBoxLayout;
            numPort = new QSpinBox;
            //need to look at port specification for the following lines
            numPort->setMaximum(60000);
            numPort->setMinimum(1000);
            numPort->setValue(5000);
            numPortLabel = new QLabel(tr("port number"));
        portLayout->addWidget(numPort);
        portLayout->addWidget(numPortLabel);
    centralLayout->addWidget(serverChoice);
    centralLayout->addWidget(clientChoice);
    centralLayout->addLayout(portLayout);
    centralWidget->setLayout(centralLayout);

    setCentralWidget(centralWidget);

    //action listener

    connect(serverChoice,SIGNAL(pressed()),controler_,SLOT(runServer()));
    connect(clientChoice,SIGNAL(pressed()),controler_,SLOT(runClient()));

}

Window::~Window()
{    
}

int Window::getPort()
{
    return numPort->value();
}

void Window::setServerGui(int port)
{

    centralServer = new QWidget;
    centralServerLayout = new QVBoxLayout;
        serverStartInfo = new QLabel(tr("Server is started on port ") + QString::number(port));
        serverStartInfo->setAlignment(Qt::AlignCenter);
        listClientInfo = new QListWidget();
        listClientInfo->setMinimumSize(480,320);
    centralServerLayout->addWidget(serverStartInfo);
    centralServerLayout->addWidget(listClientInfo);
    centralServer->setLayout(centralServerLayout);

    setCentralWidget(centralServer);

}

void Window::setClientGui(int port)
{

}

void Window::closeEvent(QCloseEvent *event)
{
    controler_->close(event);
}
