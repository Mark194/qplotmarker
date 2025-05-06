#include "plot_geometry_utils.hpp"
#include "QPlotMarker/qplotmarker.hpp"

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
                                                  points.constBegin() - 1 ) :
                            std::make_pair( points.constBegin(),
                                           points.constEnd()       );

    const int step = findLeft? -1 : 1;

    for ( auto it = begin; it != end; it += step )
    {
        const QPointF & point = *it;

        if (qFuzzyCompare(point.x(), targetPoint.x())) continue;

        if ( findLeft and targetPoint.x() <= point.x() ) continue;

        if ( not findLeft and targetPoint.x() >= point.x() ) continue;

        nearesPoint = point;

        break;
    }

    return nearesPoint;
}

std::optional<QPointF> PlotGeometryUtils::findClosestPoint(
    QPlotMarker * marker,
    const QPointF & position,
    bool isFindLeft)
{
    auto serieses = marker->chart()->series();

    if ( serieses.isEmpty() ) return {};

    QList<QXYSeries *> successSeries;

    auto ignoreSeries = marker->ignoreSeries();

    for ( auto series : serieses )
    {
        auto seriesXY = dynamic_cast<QXYSeries *>( series );

        if ( seriesXY
            and seriesXY->count() != 0
            and not ignoreSeries.contains( series ) )

        successSeries.append( seriesXY );
    }

    if ( successSeries.isEmpty() ) return {};


    QList<QPointF> nearestPoints;

    for ( auto series : successSeries )
    {
        auto valuePoint = marker->chart()->mapToValue( position, series  );

        auto point = PlotGeometryUtils::findNearestPoint( valuePoint, series, isFindLeft );

        if ( not point ) point = isFindLeft? series->points().first() : series->points().last();

        nearestPoints.append( point.value() );
    }

    QPointF closestPoint;

    qreal minDistSq = std::numeric_limits<qreal>::max();

    auto targetValue = marker->chart()->mapToValue( position );

    for ( const auto & point : nearestPoints )
    {
        qreal dist = PlotGeometryUtils::distance( point, targetValue );

        if ( dist < minDistSq )
        {
            minDistSq = dist;

            closestPoint = point;
        }
    }

    return { closestPoint };
}
