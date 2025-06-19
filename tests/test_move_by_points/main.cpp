#include <QPlotMarker/QPlotMarker>

#include "chart_view_panner.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto *chart = new QChart();

    auto *series = new QLineSeries();

    *series << QPointF(1, 1) << QPointF(2, 3) << QPointF(2, 5) << QPointF(3, 2) << QPointF(4, 4)
            << QPointF(5, 3) << QPointF(6, 2);

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 7);
    chart->legend()->setVisible(false);

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->setWindowTitle("Test using marker");
    chartView->show();

    new ChartViewPanner(chartView);

    auto *oneMarker = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    oneMarker->setLabelFormat("%.2f");

    chartView->scene()->addItem(oneMarker);

    oneMarker->moveEnd();

    oneMarker->setMovementStyle(QPlotMarker::MOVEMENT_BY_POINTS);

    return a.exec();
}
