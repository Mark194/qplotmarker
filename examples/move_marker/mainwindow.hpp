#pragma once


#include <QChartView>
#include <QMainWindow>
#include <QPlotMarker>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadData(QChart * chart);

private:

    QPlotMarker * m_marker;

    void createForm();

    static QChartView * createView();

    QLayout * createControls();

    void createMarker(QChart * chart);
};
