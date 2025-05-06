#pragma once


#include <QObject>
#include <QChartView>


#include <QPlotMarker>


class ChartEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit ChartEventFilter(QObject *parent = nullptr);

    void setViewer(QChartView * chartView, QPlotMarker * marker);


protected:
    bool eventFilter(QObject * watched, QEvent * event);

private:

    bool          m_isShowCoord;

    QPlotMarker * m_marker;

    QChartView  * m_chartView;


};
