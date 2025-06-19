#pragma once

#include <QObject>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

#include <QtCharts>

using namespace QtCharts;

#endif

#include <QPointF>
#include <QXYSeries>

class QPlotMarker;

class PlotGeometryUtils
{
public:
    PlotGeometryUtils();

    static qreal distance(const QPointF &pointOne, const QPointF &pointTwo);

    static QPair<QPointF, QPointF> findTwoNearestPoints(
        const QPointF &targetPoint, QXYSeries *lineSeries);

    static std::optional<QPointF> findNearestPoint(
        const QPointF &targetPoint, QXYSeries *series, bool findLeft = false);

    static std::optional<QPointF> findClosestPoint(
        QPlotMarker *marker, const QPointF &position, bool isLeft);

    static QPointF findNearestVisiblePoint(
        QPlotMarker *marker, const QList<QAbstractSeries *> &series, const QPointF &position);

    static bool isPositionAcceptable(QPlotMarker *marker, const QPointF &position);

    static bool isPointInSeriesRange(const QList<QAbstractSeries *> &series, const QPointF &point);

    static bool hasPoint(const QList<QAbstractSeries *> &series, const QPointF &point);

    template<typename T>
    static QList<T> subtractLists(const QList<T> &list1, const QList<T> &list2)
    {
        if (list2.empty())
            return list1;

        QSet<T> set1 = QSet<T>(list1.begin(), list1.end());
        QSet<T> set2 = QSet<T>(list2.begin(), list2.end());
        return (set1 - set2).values();
    }
};
