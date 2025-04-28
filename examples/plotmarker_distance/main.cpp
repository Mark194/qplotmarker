#include <QtWidgets/QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>


#include <QPlotMarker/QPlotMarker>
#include <QPlotMarker/QPlotMarkerDistance>


int main(
    int argc, char *argv[])
{
    QApplication a(argc, argv);

    // // Создаем график и серию данных
    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries();

    // // Добавляем данные в серию
    *series << QPointF(1, 1) << QPointF(2, 3) << QPointF(3, 2) << QPointF(4, 4) << QPointF(5, 3);

    // // Настраиваем график
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Пример использования QPlotMarkerDistance");

    // qDebug() << "X)";


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->show();

    QPlotMarker *one = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    chartView->scene()->addItem( one );

    one->moveBegin();


    QPlotMarker *other = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    chartView->scene()->addItem( other );

    other->moveEnd();


    QPlotMarkerDistance * distance = new QPlotMarkerDistance();

    distance->setMarker( one, other );

    distance->setLabelFormat("%.2f");

    chartView->scene()->addItem( distance );

    return a.exec();
}
