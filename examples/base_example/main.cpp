#include <QtWidgets/QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>


#include <QPlotMarker/QPlotMarker>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // qDebug() << "X)";

    // // Создаем график и серию данных
    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries();

    // // Добавляем данные в серию
    *series << QPointF(1, 1) << QPointF(2, 3) << QPointF(3, 2) << QPointF(4, 4) << QPointF(5, 3);

    // // Настраиваем график
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Пример использования QPlotMarker");

    // qDebug() << "X)";


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->show();

    // // Создаем маркер
    QPlotMarker *marker = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    // qDebug() << "X)";

    // // Настраиваем маркер
    // // marker->setMovementStyle(QPlotMarker::MOVEMENT_BY_POINTS);
    marker->setLabelFormat("%.2f"); // Формат отображаемых координат

    // // Подключаем сигнал изменения позиции
    QObject::connect(marker, &QPlotMarker::onPositionChanged, [](const QPointF &pos) {
        qDebug() << "Маркер перемещен в позицию:" << pos;
    });

    // // Позиционируем маркер
    marker->moveBegin(); // Устанавливаем маркер на x=3

    // // Создаем view для отображения графика


    return a.exec();
}
