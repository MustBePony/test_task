#ifndef RANDOMVALUE_H
#define RANDOMVALUE_H

#include <QMutex>
#include <QObject>
#include <QWaitCondition>

static const int TICK_MS = 1000;

class RandomValue : public QObject
{
    Q_OBJECT

private:
    QMutex mutex;
    QWaitCondition wait;

    bool needStop = false;

public:
    RandomValue();
    ~RandomValue();

    void wakeUp();

public:
signals:
    void sendNewValue(double x, double y);

public slots:
    void start();
    void pause();
    void stop();
};

#endif  // RANDOMVALUE_H
