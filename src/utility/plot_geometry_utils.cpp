#include "plot_geometry_utils.hpp"

PlotGeometryUtils::PlotGeometryUtils() {}

qreal PlotGeometryUtils::distance(
    const QPointF & pointOne,
    const QPointF & pointTwo
)
{
    return std::sqrt( std::pow( pointTwo.x() - pointOne.x(), 2 ) +
                      std::pow( pointTwo.y() - pointTwo.y(), 2 )   );
}

QPair<QPointF, QPointF> PlotGeometryUtils::findTwoNearestPoints(
    const QPointF & targetPoint,
    QXYSeries * lineSeries
)
{
    qreal minDistanceOne = std::numeric_limits<qreal>::max();

    qreal minDistanceTwo = std::numeric_limits<qreal>::max();


    QPointF closestPointOne, closestPointTwo;

    for ( const QPointF & point : lineSeries->points() )
    {
        qreal dist = distance( point, targetPoint );

        if ( dist < minDistanceOne )
        {
            minDistanceTwo = minDistanceOne;

            closestPointTwo = closestPointOne;

            minDistanceOne = dist;

            closestPointOne = point;
        }

        else if ( dist < minDistanceTwo )
        {
            minDistanceTwo = dist;

            closestPointTwo = point;
        }
    }

    if ( minDistanceOne != std::numeric_limits<qreal>::max() and
        minDistanceTwo != std::numeric_limits<qreal>::max()     )

    return qMakePair( closestPointOne, closestPointTwo );

    return {};
}

std::optional<QPointF> PlotGeometryUtils::findNearestPoint(
    const QPointF & targetPoint,
    QXYSeries * series,
    bool findLeft
    )
{
    if ( not series or series->count() == 0 ) return {};

    const auto points = series->points();

    std::optional<QPointF> nearesPoint;

    auto [begin, end] = findLeft? std::make_pair( points.constEnd() - 1,
                                                  points.constBegin() - 1) :
                            std::make_pair( points.constBegin(),
                                           points.constEnd()         );

    const int step = findLeft? -1 : 1;

    for ( auto it = begin; it != end; it += step )
    {
        const QPointF & point = *it;

        if ( findLeft and targetPoint.x() < point.x() ) continue;

        if ( not findLeft and targetPoint.x() > point.x() ) continue;

        nearesPoint = point;

        break;
    }

    return nearesPoint;
}
