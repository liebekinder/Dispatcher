#ifndef CONTROLER_H
#define CONTROLER_H

#include <QObject>

class Window;

class Controler : public QObject
{
    Q_OBJECT
public:
    explicit Controler(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // CONTROLER_H
