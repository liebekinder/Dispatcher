#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>

class Controler;

class Window : public QMainWindow
{
    Q_OBJECT
    
public:
    Window(QWidget *parent = 0);
    ~Window();

private:
    //initial selction window
    QWidget* centralWidget;
    QVBoxLayout * centralLayout;
    QPushButton * serverChoice;
    QPushButton * clientChoice;

};

#endif // WINDOW_H
