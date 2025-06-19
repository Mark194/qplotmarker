#include "plot_geometry_utils.hpp"
#include "QPlotMarker/qplotmarker.hpp"

PlotGeometryUtils::PlotGeometryUtils() = default;

qreal PlotGeometryUtils::distance(const QPointF &pointOne, const QPointF &pointTwo)
{
    if (pointOne.x() != pointTwo.x())
        return qAbs(pointOne.x() - pointTwo.x());

    return std::hypot(pointOne.x() - pointTwo.x(), pointOne.y() - pointTwo.y());
}

QPair<QPointF, QPointF> PlotGeometryUtils::findTwoNearestPoints(
    const QPointF &targetPoint, QXYSeries *lineSeries)
{
    const auto &points = lineSeries->points();

    if (points.isEmpty())
        return {};

    const auto &first = points.first();
    const auto &last = points.last();

    if (targetPoint.x() < first.x())
        return {first, first};
    if (targetPoint.x() > last.x())
        return {last, last};

    for (qsizetype i = 1; i < points.size(); ++i)

        if (targetPoint.x() <= points[i].x())

            return {points[i - 1], points[i]};

    return {};
}

std::optional<QPointF> PlotGeometryUtils::findNearestPoint(
    const QPointF &targetPoint, QXYSeries *series, bool findLeft)
{
    if (not series or series->count() == 0)
        return {};

    const auto points = series->points();

    std::optional<QPointF> nearestPoint;

    auto [begin, end] = findLeft ? std::make_pair(points.constEnd() - 1, points.constBegin() - 1)
                                 : std::make_pair(points.constBegin(), points.constEnd());

    const int step = findLeft ? -1 : 1;

    for (auto it = begin; it != end; it += step) {
        const QPointF &point = *it;

        if (qFuzzyCompare(point.x(), targetPoint.x()))
            continue;

        if (findLeft and targetPoint.x() <= point.x())
            continue;

        if (not findLeft and targetPoint.x() >= point.x())
            continue;

        nearestPoint = point;

        break;
    }

    return nearestPoint;
}

std::optional<QPointF> PlotGeometryUtils::findClosestPoint(
    QPlotMarker *marker, const QPointF &position, bool isFindLeft)
{
    auto serieses = marker->chart()->series();

    if (serieses.isEmpty())
        return {};

    QList<QXYSeries *> successSeries;

    auto ignoreSeries = marker->ignoreSeries();

    for (auto series : serieses) {
        auto seriesXY = dynamic_cast<QXYSeries *>(series);

        if (seriesXY and seriesXY->count() != 0 and not ignoreSeries.contains(series))

            successSeries.append(seriesXY);
    }

    if (successSeries.isEmpty())
        return {};

    QList<QPointF> nearestPoints;

    for (auto series : successSeries) {
        auto valuePoint = marker->chart()->mapToValue(position, series);

        auto point = findNearestPoint(valuePoint, series, isFindLeft);

        if (not point)
            point = isFindLeft ? series->points().first() : series->points().last();

        if (const auto pixelPoint = marker->chart()->mapToPosition(point.value(), series);
            not isPositionAcceptable(marker, pixelPoint))
            continue;

        nearestPoints.append(point.value());
    }

    QPointF closestPoint;

    bool isFound = false;

    qreal minDistSq = std::numeric_limits<qreal>::max();

    const auto targetValue = marker->chart()->mapToValue(position);

    for (const auto &point : nearestPoints) {
        if (const qreal dist = distance(point, targetValue); dist < minDistSq) {
            minDistSq = dist;

            closestPoint = point;

            isFound = true;
        }
    }

    if (isFound)
        return {closestPoint};

    return {};
}
QPointF PlotGeometryUtils::findNearestVisiblePoint(
    QPlotMarker *marker, const QList<QAbstractSeries *> &series, const QPointF &position)
{
    const auto chart = marker->chart();

    const QPointF valueTargetPoint = chart->mapToValue(position);

    double minDistance = std::numeric_limits<double>::max();
    bool found = false;

    QPointF nearestPoint = position;

    for (auto current : series) {
        if (auto *xySeries = qobject_cast<QXYSeries *>(current)) {
            for (const QPointF &point : xySeries->points()) {
                auto pixelPoint = chart->mapToPosition(point, xySeries);

                if (const double distance = PlotGeometryUtils::distance(point, valueTargetPoint);
                    isPositionAcceptable(marker, pixelPoint) and distance < minDistance) {
                    minDistance = distance;
                    nearestPoint = chart->mapToPosition(point, xySeries);
                    found = true;
                }
            }
        }
    }

    if (found)
        return nearestPoint;

    auto plotArea = chart->plotArea();

    const double centerY = plotArea.center().y();

    if (position.x() < plotArea.center().x())
        return {plotArea.left(), centerY};

    return {plotArea.right(), centerY};
}
bool PlotGeometryUtils::isPositionAcceptable(QPlotMarker *marker, const QPointF &position)
{
    const QRectF plotArea = marker->chart()->plotArea();

    if (marker->orientation() == Qt::Vertical)

        return position.x() >= plotArea.left() and position.x() <= plotArea.right();

    return position.y() >= plotArea.top() and position.y() <= plotArea.bottom();
}
bool PlotGeometryUtils::isPointInSeriesRange(
    const QList<QAbstractSeries *> &series, const QPointF &point)
{
    for (const auto current : series) {
        const auto xySeries = qobject_cast<QXYSeries *>(current);

        if (not xySeries)
            continue;

        auto points = xySeries->points();

        if (points.isEmpty())
            continue;

        auto min = points.first();

        auto max = points.last();

        if (min.x() <= point.x() and point.x() <= max.x())
            return true;
    }

    return false;
}
bool PlotGeometryUtils::hasPoint(const QList<QAbstractSeries *> &series, const QPointF &point)
{
    for (const QAbstractSeries *abstractSeries : series) {
        if (const auto *xySeries = qobject_cast<const QXYSeries *>(abstractSeries)) {
            const auto points = xySeries->points();
            auto it = std::find_if(points.begin(), points.end(), [&point](const QPointF &p) {
                return qFuzzyCompare(p.x(), point.x()) and qFuzzyCompare(p.y(), point.y());
            });
            if (it != points.end()) {
                return true;
            }
        }
    }
    return false;
}
