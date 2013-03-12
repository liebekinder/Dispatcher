#ifndef CONTROLER_H
#define CONTROLER_H

#include <QObject>
#include <QCloseEvent>
#include <QMessageBox>

class Window;

class Controler : public QObject
{
    Q_OBJECT
public:
    Controler(Window *f, QObject *parent = 0);

    //methods
    void close(QCloseEvent * event);
    QString getMode();

private:
    Window * window_;
    QString mode_;

signals:
    
public slots:
    void runServer();
    void runClient();
};

#endif // CONTROLER_H
