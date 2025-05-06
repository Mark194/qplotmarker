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
    chart->legend()->setVisible( false );

    // qDebug() << "X)";


    QString markerFormat = "%.2f";

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->setWindowTitle("Пример использования QPlotMarkerDistance");
    chartView->show();

    QPlotMarker *one = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    one->setLabelFormat( markerFormat );

    chartView->scene()->addItem( one );

    one->moveBegin();


    QPlotMarker *other = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    other->setLabelFormat( markerFormat );

    chartView->scene()->addItem( other );

    other->moveEnd();


    QPlotMarkerDistance * distance = new QPlotMarkerDistance();

    distance->setMarker( one, other );

    distance->setLabelFormat( markerFormat );

    chartView->scene()->addItem( distance );

    distance->update();

    return a.exec();
}
