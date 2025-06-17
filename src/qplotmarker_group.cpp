#include "QPlotMarker/qplotmarker_group.hpp"

#include "QPlotMarker/QPlotMarkerDistance"
#include "QPlotMarker/qplotmarker.hpp"

QPlotMarkerGroup::QPlotMarkerGroup(QObject *parent)
    : QObject(parent)
    , m_isSyncMovement(false)
{}

void QPlotMarkerGroup::addMarker(QGraphicsItem *marker)
{
    if (const auto plotMarker = dynamic_cast<QPlotMarker *>(marker))

        addMarker(plotMarker);
}

void QPlotMarkerGroup::addMarker(QPlotMarker *marker)
{
    m_markers.append(marker);
}

void QPlotMarkerGroup::removeMarker(QPlotMarker *marker)
{
    m_markers.removeOne(marker);
}

void QPlotMarkerGroup::addDistance(QGraphicsItem *item)
{
    if (auto distance = dynamic_cast<QPlotMarkerDistance *>(item))

        addDistance(distance);
}

void QPlotMarkerGroup::addDistance(QPlotMarkerDistance *distance)
{
    m_distances.append(distance);
}

void QPlotMarkerGroup::removeDistance(QPlotMarkerDistance *distance)
{
    m_distances.removeOne(distance);
}

void QPlotMarkerGroup::clear()
{
    m_markers.clear();
}

void QPlotMarkerGroup::setSyncMovement(bool isSync)
{
    if (m_isSyncMovement == isSync)
        return;

    m_isSyncMovement = isSync;

    for (auto marker : m_markers)

        if (m_isSyncMovement)

            connect(marker, &QPlotMarker::positionChanged, this, &QPlotMarkerGroup::move);

        else

            disconnect(marker, &QPlotMarker::positionChanged, this, &QPlotMarkerGroup::move);
}

void QPlotMarkerGroup::move(const QPointF &point)
{
    const auto senderMarker = dynamic_cast<QPlotMarker *>(sender());

    for (const auto marker : m_markers) {
        if (marker == senderMarker)
            continue;

        marker->blockSignals(true);

        marker->move(point);

        marker->blockSignals(false);
    }

    for (const auto distance : m_distances)

        distance->update();
}

QList<QPlotMarker *> QPlotMarkerGroup::markers() const
{
    return m_markers;
}
