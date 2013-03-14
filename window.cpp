#include "window.h"
#include "controler.h"

/**
 * Initial window
 * @brief Window::Window
 * @param parent
 */
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
    connect(clientChoice,SIGNAL(pressed()),controler_,SLOT(askClient()));

}

Window::~Window()
{    
}

int Window::getPort()
{
    return numPort->value();
}

QString Window::getIPAddress()
{
    return IPType->text();
}

QString Window::getInputFile()
{
    return clientLineEditInput->text();
}

QString Window::getOutputFile()
{
    return clientLineEditOutput->text();
}

/**
 * Setting the server display
 * @brief Window::setServerGui
 * @param port
 */
void Window::setServerGui(int port)
{

    centralServer = new QWidget;
    centralServerLayout = new QVBoxLayout;
        serverStartInfo = new QLabel(tr("Server is started on port: ") + QString::number(port));
        serverStartInfo->setAlignment(Qt::AlignCenter);
        listClientInfo = new QListWidget();
        listClientInfo->setMinimumSize(480,320);
    centralServerLayout->addWidget(serverStartInfo);
    centralServerLayout->addWidget(listClientInfo);
    centralServer->setLayout(centralServerLayout);

    setCentralWidget(centralServer);

}

/**
 * Setting the client display
 * @brief Window::setClientGui
 * @param port
 */
void Window::setClientGui(int port)
{
    centralClientTemp = new QWidget;
    centralClientTempLayout = new QVBoxLayout;
    centralClientInfo = new QLabel(tr("Please enter the server IP adress"));
    centralClientInfo->setAlignment(Qt::AlignCenter);
    centralClientPortInfo = new QLabel(tr("Client will start on port: ") + QString::number(port));
    centralClientPortInfo->setAlignment(Qt::AlignCenter);
        centralClientTempFormLayout = new QHBoxLayout;
            IPType = new QLineEdit("127.0.0.1");
            IPTypeOk = new QPushButton(tr("Connect"));
        centralClientTempFormLayout->addWidget(IPType);
        centralClientTempFormLayout->addWidget(IPTypeOk);
    centralClientTempLayout->addWidget(centralClientInfo);
    centralClientTempLayout->addWidget(centralClientPortInfo);
    centralClientTempLayout->addLayout(centralClientTempFormLayout);
    centralClientTemp->setLayout(centralClientTempLayout);

    setCentralWidget(centralClientTemp);

    //action listener

    connect(IPTypeOk,SIGNAL(pressed()),controler_,SLOT(runClient()));

}

void Window::setClientGuiFinal(int port)
{
    centralClient = new QWidget;
    centralClientLayout = new QVBoxLayout;
        clientLabelGeneral = new QLabel(tr("Client connected on port: ")+ QString::number(port));
        clientLabelGeneral->setAlignment(Qt::AlignCenter);
        clientLabelInput = new QLabel(tr("Input file:"));
        clientlayoutInput = new QHBoxLayout;
            clientLineEditInput = new QLineEdit;
            clientButtonInput = new QPushButton(tr("..."));
            clientButtonInput->setMaximumWidth(30);
        clientlayoutInput->addWidget(clientLineEditInput);
        clientlayoutInput->addWidget(clientButtonInput);
        clientLabelOutput = new QLabel(tr("Output file:"));
        clientlayoutOutput = new QHBoxLayout;
            clientLineEditOutput = new QLineEdit;
            clientButtonOutput = new QPushButton(tr("..."));
            clientButtonOutput->setMaximumWidth(30);
        clientlayoutOutput->addWidget(clientLineEditOutput);
        clientlayoutOutput->addWidget(clientButtonOutput);
        clientSend = new QPushButton(tr("Send"));
    centralClientLayout->addWidget(clientLabelGeneral);
    centralClientLayout->addWidget(clientLabelInput);
    centralClientLayout->addLayout(clientlayoutInput);
    centralClientLayout->addWidget(clientLabelOutput);
    centralClientLayout->addLayout(clientlayoutOutput);
    centralClientLayout->addWidget(clientSend);
    centralClient->setLayout(centralClientLayout);

    setCentralWidget(centralClient);

    //action listener
    connect(clientButtonInput,SIGNAL(pressed()),this,SLOT(askFileIn()));
    connect(clientButtonOutput,SIGNAL(pressed()),this,SLOT(askFileOut()));
    connect(clientSend,SIGNAL(pressed()),controler_,SLOT(clientSendFile()));
}
/**
 * overloading of QWidget method closeEvent
 * Intercept the quit event
 * @brief Window::closeEvent
 * @param event
 */
void Window::closeEvent(QCloseEvent *event)
{
    controler_->close(event);
}

void Window::askFileIn()
{
    clientLineEditInput->setText( QFileDialog::getOpenFileName(this,
                                        tr("select the file to work on"),
                                        "",
                                        tr("All files (*.*)")) );
}

void Window::askFileOut()
{
    clientLineEditOutput->setText( QFileDialog::getSaveFileName(this,
                                     tr("select the file to work on"),
                                        "",
                                     tr("All files (*.*)")) );
}
