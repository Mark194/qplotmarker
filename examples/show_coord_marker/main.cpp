#include <QtWidgets/QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>


#include <QPlotMarker>


#include "chart_event_filter.hpp"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QChart *chart = new QChart();



    QLineSeries *series = new QLineSeries();

    *series << QPointF(1, 1) << QPointF(2, 3) << QPointF(3, 2) << QPointF(4, 4) << QPointF(5, 3);

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->setVisible( false );

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->show();
    chartView->setWindowTitle( "Пример отображения координаты точки пересечения (нажмите V)" );

    QPlotMarker *marker = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    marker->setLabelFormat("%.2f");

    chartView->scene()->addItem( marker );

    marker->move(0.5);


    ChartEventFilter * filter = new ChartEventFilter;

    filter->setViewer( chartView, marker );

    chartView->installEventFilter( filter );

    return a.exec();
}
