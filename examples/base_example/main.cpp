#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtWidgets/QApplication>

#include <QPlotMarker/QPlotMarker>

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
    chartView->setWindowTitle("Example of using QPlotMarker");
    chartView->show();

    //![1]

    //![2]

    auto *oneMarker = new QPlotMarker(chart, Qt::red, Qt::Horizontal);
    oneMarker->setLabelFormat("%.2f");

    auto *otherMarker = new QPlotMarker(chart, Qt::red, Qt::Vertical);
    otherMarker->setLabelFormat("%.2f");

    //![2]

    //![3]

    chartView->scene()->addItem(oneMarker);
    chartView->scene()->addItem(otherMarker);

    //![3]

    return a.exec();
}
