#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QChartView>
#include <QDateTime>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initChart();

    ThreadWorker = new QThread(nullptr);

    genValue = new RandomValue();

    genValue->moveToThread(ThreadWorker);

    connect(ThreadWorker, &QThread::started, genValue, &RandomValue::start, Qt::QueuedConnection);
    connect(genValue, &RandomValue::sendNewValue, this, &MainWindow::addNewValue, Qt::QueuedConnection);

    connect(this, &MainWindow::sendPause, genValue, &RandomValue::pause, Qt::QueuedConnection);
    connect(this, &MainWindow::sendStop, genValue, &RandomValue::stop, Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete genValue;

    delete ThreadWorker;

    delete ui;
}

void MainWindow::initChart()
{
    chart = new QChart();

    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setAnimationDuration(1000);

    // Customize chart background
    QLinearGradient backgroundGradient;
    backgroundGradient.setStart(QPointF(0, 0));
    backgroundGradient.setFinalStop(QPointF(0, 1));
    backgroundGradient.setColorAt(0.0, QRgb(0xd2d0d1));
    backgroundGradient.setColorAt(1.0, QRgb(0x4c4547));
    backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    chart->setBackgroundBrush(backgroundGradient);

    // Customize plot area background
    QLinearGradient plotAreaGradient;
    plotAreaGradient.setStart(QPointF(0, 1));
    plotAreaGradient.setFinalStop(QPointF(1, 0));
    plotAreaGradient.setColorAt(0.0, QRgb(0x555555));
    plotAreaGradient.setColorAt(1.0, QRgb(0x55aa55));
    plotAreaGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    chart->setPlotAreaBackgroundBrush(plotAreaGradient);
    chart->setPlotAreaBackgroundVisible(true);

    chart->legend()->hide();

    //----------------------
    QValueAxis *axisX = new QValueAxis;
    axisX->setLabelFormat("%i");
    axisX->setTitleText("Ox");

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Oy");

    // Customize axis label font
    QFont labelsFont;
    labelsFont.setPixelSize(12);
    axisX->setLabelsFont(labelsFont);
    axisY->setLabelsFont(labelsFont);

    // Customize axis colors
    QPen axisPen(QRgb(0xd18952));
    axisPen.setWidth(3);
    axisX->setLinePen(axisPen);
    axisY->setLinePen(axisPen);

    // Customize axis label colors
    QBrush axisBrush(Qt::black);
    axisX->setLabelsBrush(axisBrush);
    axisY->setLabelsBrush(axisBrush);

    // Customize grid lines and shades
    axisX->setGridLineVisible(false);
    axisY->setGridLineVisible(false);
    axisY->setShadesPen(Qt::NoPen);
    axisY->setShadesBrush(QBrush(QColor(0x99, 0xcc, 0xcc, 0x55)));
    axisY->setShadesVisible(true);

    axisX->setRange(0, 100);

    axisY->setRange(0, 100);

    chart->addAxis(axisY, Qt::AlignLeft);
    chart->addAxis(axisX, Qt::AlignBottom);
    //----------

    series = new QScatterSeries(chart);
    series->setUseOpenGL(true);

    chart->addSeries(series);

    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series->setMarkerSize(8);
    series->setColor(QColor("blue"));

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    series->setName("Test");
    series->setPointsVisible(true);

    series->setPointLabelsColor(Qt::red);
    series->setPointLabelsFormat("@yPoint");

    //-------
    ui->graphicsView->setChart(chart);
    ui->graphicsView->setRenderHint(QPainter::QPainter::Antialiasing);
}

void MainWindow::addNewValue(double x, double y)
{
    series->append(x, y);
}

void MainWindow::on_pushButtonStart_clicked()
{
    if (ThreadWorker->isRunning())
    {
        genValue->wakeUp();

        return;
    }

    ThreadWorker->start();
}

void MainWindow::on_pushButtonPause_clicked()
{
    emit sendPause();
}

void MainWindow::on_pushButtonStop_clicked()
{
    /// *Remark. В задании ничего не сказанно, какое поведение должно быть после нажатия стоп у остальных кнопок

    emit sendStop();

    // не совсем понял, что означает очищает граф. сцену
    series->removePoints(0, series->count());

    ui->graphicsView->setChart(new QChart());

    delete chart;
}
