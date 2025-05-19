#include "plot_geometry_utils.hpp"
#include "QPlotMarker/qplotmarker.hpp"

PlotGeometryUtils::PlotGeometryUtils() = default;

qreal PlotGeometryUtils::distance(
    const QPointF & pointOne,
    const QPointF & pointTwo
)
{
    return abs(pointOne.x() - pointTwo.x());
}

QPair<QPointF, QPointF> PlotGeometryUtils::findTwoNearestPoints(
    const QPointF & targetPoint,
    QXYSeries * lineSeries
)
{
    const auto & points = lineSeries->points();

    if ( points.isEmpty() ) return {};

    const auto& first = points.first();
    const auto& last = points.last();

    if ( targetPoint.x() < first.x() ) return { first, first };
    if ( targetPoint.x() > last.x()  ) return { last,  last  };


    for (qsizetype i = 1; i < points.size(); ++i)

        if (targetPoint.x() <= points[i].x())

            return {points[i-1], points[i]};


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

    std::optional<QPointF> nearestPoint;

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

        nearestPoint = point;

        break;
    }

    return nearestPoint;
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
