#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "randomvalue.h"

#include <QChart>
#include <QDateTimeAxis>
#include <QMainWindow>
#include <QMutex>
#include <QPushButton>
#include <QScatterSeries>
#include <QValueAxis>
#include <QWaitCondition>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QChart *chart;
    QScatterSeries *series;

    QThread *ThreadWorker;
    RandomValue *genValue;

    Ui::MainWindow *ui;

private:
    void initChart();

private:
signals:
    void sendPause();
    void sendStop();

private slots:
    void addNewValue(double x, double y);
    void on_pushButtonStart_clicked();
    void on_pushButtonPause_clicked();
    void on_pushButtonStop_clicked();
};
#endif  // MAINWINDOW_H
