#include "window.h"
#include "controler.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    //initialization of the selection window
    setWindowTitle(tr("Task Dispatcher"));

    centralWidget = new QWidget;
    centralLayout = new QVBoxLayout;
        serverChoice = new QPushButton(tr("Launch server"));



}

Window::~Window()
{
    
}
