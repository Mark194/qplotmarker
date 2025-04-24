#pragma once


#include <QLineSeries>
#include <QPointF>


class PlotGeometryUtils
{
public:
    PlotGeometryUtils();

    static qreal distance( const QPointF & pointOne, const QPointF & pointTwo );

    static QPair<QPointF, QPointF> findTwoNearestPoints( const QPointF & targetPoint,
                                                         QLineSeries * lineSeries     );

    static std::optional<QPointF> findNearestPoint( const QPointF & targetPoint,
                                                    QLineSeries * series,
                                                    bool findLeft = false );
};
