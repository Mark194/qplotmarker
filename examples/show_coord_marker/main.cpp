#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QApplication>

#include <QPlotMarker>

#include "chart_event_filter.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //![1]
    auto *chart = new QChart();

    auto *series = new QLineSeries();

    *series << QPointF(1, 1) << QPointF(2, 3) << QPointF(3, 2) << QPointF(4, 4) << QPointF(5, 3);

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->setVisible(false);

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->show();
    chartView->setWindowTitle("Example of displaying the intersection point coordinate (press V)");
    //![1]

    //![2]
    auto *marker = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    marker->setLabelFormat("%.2f");

    chartView->scene()->addItem(marker);

    marker->move(0.5);

    //![2]

    //![3]

    auto *filter = new ChartEventFilter;

    filter->setViewer(chartView, marker);

    chartView->installEventFilter(filter);

    //![3]

    return a.exec();
}
