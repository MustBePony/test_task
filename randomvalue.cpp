#include "randomvalue.h"

#include <QAbstractEventDispatcher>
#include <QRandomGenerator>
#include <QThread>

RandomValue::RandomValue()
{
}

RandomValue::~RandomValue()
{
}

void RandomValue::start()
{
    const int max = 100;

    while (1)
    {
        this->thread()->eventDispatcher()->processEvents(QEventLoop::AllEvents);

        if (needStop)
            break;

        emit sendNewValue(QRandomGenerator::global()->generateDouble() * max, QRandomGenerator::global()->generateDouble() * max);

        this->thread()->msleep(TICK_MS);
    }

    this->thread()->quit();
}

void RandomValue::pause()
{
    QMutexLocker l(&mutex);
    wait.wait(&mutex);
}

void RandomValue::stop()
{
    needStop = true;
}

void RandomValue::wakeUp()
{
    wait.wakeAll();
}
