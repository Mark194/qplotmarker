#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QApplication>

#include <QPlotMarker/QPlotMarker>
#include <QPlotMarker/QPlotMarkerDistance>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setWindowIcon(QIcon(":/logo"));
    //![1]
    auto *chart = new QChart();
    auto *series = new QLineSeries();

    *series << QPointF(1, 1) << QPointF(2, 3) << QPointF(3, 2) << QPointF(4, 4) << QPointF(5, 3);

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->legend()->setVisible(false);

    const QString markerFormat = "%.2f";

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->setWindowTitle("Example of using QPlotMarkerDistance");
    chartView->show();

    //![1]

    //![2]
    auto *one = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    one->setLabelFormat(markerFormat);

    chartView->scene()->addItem(one);

    auto *other = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    other->setLabelFormat(markerFormat);

    chartView->scene()->addItem(other);

    other->moveEnd();
    //![2]

    //![3]
    auto *distance = new QPlotMarkerDistance();

    distance->setMarker(one, other);

    distance->setLabelFormat(markerFormat);

    chartView->scene()->addItem(distance);

    distance->update();
    //![3]

    return a.exec();
}
