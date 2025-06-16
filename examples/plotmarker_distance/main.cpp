#include <QtWidgets/QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>


#include <QPlotMarker/QPlotMarker>
#include <QPlotMarker/QPlotMarkerDistance>


int main(
    int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto *chart = new QChart();
    auto *series = new QLineSeries();

    *series << QPointF(1, 1) << QPointF(2, 3) << QPointF(3, 2) << QPointF(4, 4) << QPointF(5, 3);

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->setVisible( false );


    const QString markerFormat = "%.2f";

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->setWindowTitle( "Example of using QPlotMarkerDistance");
    chartView->show();

    auto *one = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    one->setLabelFormat( markerFormat );

    chartView->scene()->addItem( one );


    auto *other = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    other->setLabelFormat( markerFormat );

    chartView->scene()->addItem( other );

    other->moveEnd();

    auto * distance = new QPlotMarkerDistance();

    distance->setMarker( one, other );

    distance->setLabelFormat( markerFormat );

    chartView->scene()->addItem( distance );

    distance->update();

    return a.exec();
}
