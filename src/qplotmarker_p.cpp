#include "qplotmarker_p.hpp"

#include <QGraphicsScene>
#include <QValueAxis>

#include "entity/movable_button.hpp"

#include "entity/graphics_coord_item.hpp"

#include "utility/plot_geometry_utils.hpp"

QPlotMarkerPrivate::QPlotMarkerPrivate(QPlotMarker *q)
    : q_ptr(q)
    , m_markerColor(Qt::black)
    , m_intersectionPointSize(3)
    , m_intersectionLineSize(2)
    , m_isVisibleCoords(false)
    , m_isSelectedLock(false)
    , m_buttonSelectedIcon(":/marker_selected_icon")
    , m_buttonsSelectedControl(":/marker_selected_eye")
    , m_buttonIcon(":/marker_icon")
    , m_buttonControl(":/marker_eye")
{}

QPlotMarkerPrivate::~QPlotMarkerPrivate()
{
    for (auto &item : m_intersectionItems) {
        delete item.coord;
        delete item.point;
    }

    m_intersectionItems.clear();
}

void QPlotMarkerPrivate::init(
    QChart *parent, const QColor &color, QPlotMarker::MarkerOrientation orientation)
{
    q_ptr->setFlag(QGraphicsItem::ItemIsSelectable);

    m_parentChart = parent;
    m_markerColor = color;
    m_orientation = orientation;

    m_controlItem = new MovableButton(q_ptr);

    m_controlItem->setColor(m_markerColor);

    m_line = new QGraphicsLineItem(q_ptr);

    m_line->setPen(QPen(color, 2, Qt::DotLine, Qt::RoundCap));

    m_coordInfo = new GraphicsCoordItem(q_ptr);

    m_coordInfo->setItemColor(m_markerColor);

    q_ptr->setSelected(false);

    QObject::connect(m_parentChart, &QChart::plotAreaChanged, q_ptr, &QPlotMarker::update);

    Q_ASSERT_X(
        not m_parentChart->axes().isEmpty(), "QPlotMarkerPrivate::init()", "The axes are missing!");

    for (auto axis : m_parentChart->axes())

        if (auto valueAxis = dynamic_cast<QValueAxis *>(axis))

            QObject::connect(valueAxis, &QValueAxis::rangeChanged, q_ptr, &QPlotMarker::update);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

inline uint qHash(const QPointF &point, uint seed = 0) noexcept
{
    QtPrivate::QHashCombine hash;
    seed = hash(seed, point.x());
    seed = hash(seed, point.y());
    return seed;
}

#else
namespace std {
template<>
struct hash<QPointF>
{
    size_t operator()(const QPointF &key, const size_t seed) const noexcept
    {
        return qHashMulti(seed, key.x(), key.y());
    }
};
} // namespace std
#endif

QPointF QPlotMarkerPrivate::adjustTextItemPosition(
    const QPointF &viewPoint, const GraphicsCoordItem *textItem, const QRectF &plotArea)
{
    const QRectF itemRect = textItem->boundingRect();
    QPointF adjustedPos = viewPoint;

    if (adjustedPos.x() + itemRect.width() > plotArea.right())
        adjustedPos.rx() -= (adjustedPos.x() + itemRect.width() - plotArea.right());
    else if (adjustedPos.x() < plotArea.left())
        adjustedPos.rx() += (plotArea.left() - adjustedPos.x());

    if (adjustedPos.y() + itemRect.height() > plotArea.bottom())
        adjustedPos.ry() -= (adjustedPos.y() + itemRect.height() - plotArea.bottom());
    else if (adjustedPos.y() < plotArea.top())
        adjustedPos.ry() += (plotArea.top() - adjustedPos.y());

    return adjustedPos;
}

void QPlotMarkerPrivate::loadIntersectionPoints(const QPointF &position)
{
    if (not m_intersectionItems.empty())

        clearInterSectionPoints();

    if (not PlotGeometryUtils::isPositionAcceptable(q_ptr, position)
        or not PlotGeometryUtils::isPointIntoSeries(
            m_parentChart, m_parentChart->mapToValue(position)))
        return;

    QSet<QPointF> points;

    auto markerLineOld = m_line->line();

    QLineF markerLine(
        m_parentChart->mapToValue(markerLineOld.p1()),
        m_parentChart->mapToValue(markerLineOld.p2()));

    auto serieses = PlotGeometryUtils::subtractLists<QAbstractSeries *>(
        m_parentChart->series(), m_ignoreSeries);

    for (auto series : serieses) {
        if (not series->isVisible())
            continue;

        const auto lineSeries = dynamic_cast<QXYSeries *>(series);

        if (not lineSeries)
            continue;

        auto point = m_parentChart->mapToValue(position, series);

        auto twoPoint = PlotGeometryUtils::findTwoNearestPoints(point, lineSeries);

        QLineF segment(twoPoint.first, twoPoint.second);

        QPointF intersectPoint;

        auto intersectType = markerLine.intersects(segment, &intersectPoint);

        if (intersectType == QLineF::BoundedIntersection)

            points.insert(
                {intersectPoint.x(), QString::number(intersectPoint.y(), 'f', 3).toDouble()});
        else {
            if (qFuzzyCompare(twoPoint.first.x(), twoPoint.second.x())) {
                if (twoPoint.first.y() < twoPoint.second.y())
                    points.insert(twoPoint.first);

                else
                    points.insert(twoPoint.second);
            }
        }
    }

    const QRectF plotArea = m_parentChart->plotArea();

    for (auto &point : points) {
        auto viewPoint(m_parentChart->mapToPosition(point));

        auto item = new QGraphicsEllipseItem(
            viewPoint.x() - m_intersectionPointSize,
            viewPoint.y() - m_intersectionPointSize,
            2 * m_intersectionPointSize,
            2 * m_intersectionPointSize);

        item->setPen(QPen(m_markerColor, 2));

        item->setBrush(m_markerColor);

        m_parentChart->scene()->addItem(item);

        auto textItem = new GraphicsCoordItem();

        textItem->setFont(m_coordInfo->font());

        textItem->setItemColor(m_markerColor);

        textItem->setCoord(point.y());

        textItem->setLabelFormat(m_labelFormat);

        textItem->setVisible(m_isVisibleCoords);

        textItem->setPos(adjustTextItemPosition(viewPoint, textItem, plotArea));

        QObject::connect(textItem, &GraphicsCoordItem::onActivated, q_ptr, &QPlotMarker::activate);

        m_parentChart->scene()->addItem(textItem);

        m_intersectionItems.append({.coord = textItem, .point = item});
    }
}

void QPlotMarkerPrivate::moveMarkerToPosition(const QPointF &position)
{
    if (not PlotGeometryUtils::isPositionAcceptable(q_ptr, position))
        return;

    m_markerPosition = position;

    m_markerValue = m_parentChart->mapToValue(position);

    QRectF plotArea = m_parentChart->plotArea();
    auto controlRect = m_controlItem->mapToScene(m_controlItem->boundingRect()).boundingRect();

    bool isVertical = this->isVertical();
    bool isInverted = this->isInverted();

    if (isVertical)
        setupVerticalMarker(position, plotArea, controlRect, isInverted);
    else
        setupHorizontalMarker(position, plotArea, controlRect, isInverted);

    emit q_ptr->positionChanged(position);
}

void QPlotMarkerPrivate::setupVerticalMarker(
    const QPointF &position, const QRectF &plotArea, const QRectF &controlRect, bool inverted)
{
    if (inverted)
        m_controlItem->setRotation(180);

    m_line->setLine(position.x(), plotArea.top(), position.x(), plotArea.bottom());

    // Control item positioning
    qreal halfPixmapWidth = controlRect.width() / 2.0;
    qreal pixmapHeight = controlRect.height();
    qreal controlX = inverted ? position.x() + halfPixmapWidth : position.x() - halfPixmapWidth;
    qreal controlY = inverted ? plotArea.bottom() + pixmapHeight : plotArea.top() - pixmapHeight;
    m_controlItem->setPos(controlX, controlY);

    // Coordinate info positioning
    m_coordInfo->setCoord(m_parentChart->mapToValue(position).x());
    qreal coordX = position.x() - m_coordInfo->boundingRect().width() / 2;
    qreal coordY = inverted ? plotArea.top() - m_coordInfo->boundingRect().height()
                            : plotArea.bottom();
    m_coordInfo->setPos(coordX, coordY);

    loadIntersectionPoints(position);
}

void QPlotMarkerPrivate::setupHorizontalMarker(
    const QPointF &position, const QRectF &plotArea, const QRectF &controlRect, bool inverted)
{
    m_controlItem->setRotation(inverted ? 270 : 90);

    m_line->setLine(plotArea.left(), position.y(), plotArea.right(), position.y());

    qreal halfPixmapHeight = controlRect.height() / 2.0;
    qreal controlX = inverted ? plotArea.left() - controlRect.width()
                              : plotArea.right() + controlRect.width();
    qreal controlY = inverted ? position.y() + halfPixmapHeight : position.y() - halfPixmapHeight;
    m_controlItem->setPos(controlX, controlY);

    m_coordInfo->setCoord(m_parentChart->mapToValue(position).y());
    auto valueAxis = dynamic_cast<QValueAxis *>(m_parentChart->axes(Qt::Horizontal).first());
    auto startX = m_parentChart->mapToPosition({valueAxis->min(), 0});

    qreal coordX = inverted ? plotArea.right() : startX.x() - m_coordInfo->boundingRect().width();
    qreal coordY = position.y() - m_coordInfo->boundingRect().height() / 2;
    m_coordInfo->setPos(coordX, coordY);
}

bool isPointIntSeries(QChart *chart, const QPointF &point)
{
    for (QAbstractSeries *series : chart->series()) {
        if (auto *xySeries = qobject_cast<QXYSeries *>(series)) {
            const auto &points = xySeries->points();
            if (std::find(points.begin(), points.end(), point) != points.end()) {
                return true;
            }
        }
    }
    return false;
}

void QPlotMarkerPrivate::updateOnMoveByPoints(const QPointF &targetPoint)
{
    if (const QPointF valueTargetPoint = m_parentChart->mapToValue(targetPoint);
        PlotGeometryUtils::isPositionAcceptable(q_ptr, targetPoint)
        and isPointIntSeries(m_parentChart, valueTargetPoint)) {
        moveMarkerToPosition(m_parentChart->mapToPosition(m_markerValue));
        return;
    }

    auto series = PlotGeometryUtils::subtractLists<QAbstractSeries *>(
        m_parentChart->series(), m_ignoreSeries);

    moveMarkerToPosition(PlotGeometryUtils::findNearestVisiblePoint(q_ptr, series, targetPoint));
}

void QPlotMarkerPrivate::clearInterSectionPoints()
{
    for (auto item : m_intersectionItems) {
        if (item.coord)
            delete item.coord;

        if (item.point)
            delete item.point;
    }

    m_intersectionItems.clear();
}

QPlotMarker::MarkerOrientation QPlotMarkerPrivate::orientation(Qt::Orientation orientation)
{
    return orientation == Qt::Vertical ? QPlotMarker::Vertical : QPlotMarker::Horizontal;
}

Qt::Orientation QPlotMarkerPrivate::orientation() const
{
    switch (m_orientation) {
    case QPlotMarker::Horizontal:
    case QPlotMarker::HorizontalInverted:

        return Qt::Horizontal;

    case QPlotMarker::Vertical:
    case QPlotMarker::VerticalInverted:

        return Qt::Vertical;

    default:
        throw std::logic_error("Unknown orintation");
    }
}

bool QPlotMarkerPrivate::isInverted() const
{
    return m_orientation == QPlotMarker::HorizontalInverted
           or m_orientation == QPlotMarker::VerticalInverted;
}

bool QPlotMarkerPrivate::isVertical() const
{
    return m_orientation == QPlotMarker::Vertical or m_orientation == QPlotMarker::VerticalInverted;
}
