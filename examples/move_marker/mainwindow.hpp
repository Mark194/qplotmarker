#pragma once


#include <QChartView>
#include <QMainWindow>
#include <QPlotMarker>


class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    static void loadData(QChart * chart);

private:

    QPlotMarker * m_marker;

    void createForm();

    static QChartView * createView();

    QLayout * createControls();

    void createMarker(QChart * chart);
};
