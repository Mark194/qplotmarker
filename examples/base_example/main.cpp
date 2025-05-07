#include <QtWidgets/QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>


#include <QPlotMarker/QPlotMarker>


int main(int argc, char *argv[])
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
    chart->legend()->setVisible(false);
    // qDebug() << "X)";


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->setWindowTitle("Пример использования QPlotMarker");
    chartView->show();

    // // Создаем маркер
    QPlotMarker * oneMarker = new QPlotMarker(chart, Qt::red, Qt::Horizontal);

    oneMarker->setLabelFormat("%.2f"); // Формат отображаемых координат


    QPlotMarker *otherMarker = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    otherMarker->setLabelFormat("%.2f"); // Формат отображаемых координат


    chartView->scene()->addItem( oneMarker );

    chartView->scene()->addItem( otherMarker );


    return a.exec();
}
