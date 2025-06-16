#pragma once


#include <QChartView>
#include <QMainWindow>


#include <QPlotMarker>
#include <QPlotMarkerGroup>


class ControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    ControlWindow(QWidget *parent = nullptr);
    ~ControlWindow();

private:

    QPlotMarkerGroup * m_groupMarker;

    QPlotMarker      * m_oneMarker;

    QPlotMarker      * m_otherMarker;


    void createForm();

    static QChartView * createView();

    QLayout * createViews();

    QLayout * createControls();

    static void loadData(QChart * chart,
                         double ampl = 1.0,
                         double freq = 1.0,
                         double phase = 0.0,
                         int count = 100);

    static QPlotMarker * createMarker(QChart * chart);
};
