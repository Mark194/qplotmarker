#include "../include/QPlotMarker/qplotmarker_group.hpp"

#include "QPlotMarker/qplotmarker.hpp"
#include "QPlotMarker/qplotmarker_distance.hpp"
/*!
    \class QPlotMarkerGroup
    \brief A container class that manages groups of QPlotMarker and QPlotMarkerDistance objects.

    The QPlotMarkerGroup enables synchronized movement and management of multiple
    plot markers and their distance measurements.

    \sa QPlotMarker, QPlotMarkerDistance
*/

/*!
    \fn QPlotMarkerGroup::QPlotMarkerGroup(QObject *parent)
    \brief Constructs a QPlotMarkerGroup with the given parent.
    \a parent The parent chart
*/
QPlotMarkerGroup::QPlotMarkerGroup(QObject *parent)
    : QObject(parent)
    , m_isSyncMovement(false)
{}

/*!
    \fn void QPlotMarkerGroup::addMarker(QGraphicsItem *marker)
    \brief Adds a marker to the group (QGraphicsItem overload).
    \a marker is a Graphics item to add (must be a QPlotMarker)
    \note Only QPlotMarker items will be actually added
    \sa addMarker(QPlotMarker*)
*/
void QPlotMarkerGroup::addMarker(QGraphicsItem *marker)
{
    if (const auto plotMarker = dynamic_cast<QPlotMarker *>(marker))

        addMarker(plotMarker);
}

/*!
    \fn void QPlotMarkerGroup::addMarker(QPlotMarker *marker)
    \brief Adds a QPlotMarker to the group.
    \a marker The marker to add to the group
    \sa removeMarker(), markers()
*/
void QPlotMarkerGroup::addMarker(QPlotMarker *marker)
{
    m_markers.append(marker);
}

/*!
    \fn void QPlotMarkerGroup::removeMarker(QPlotMarker *marker)
    \brief Removes a marker from the group.
    \a marker The marker to remove
    \sa addMarker(), clear()
*/
void QPlotMarkerGroup::removeMarker(QPlotMarker *marker)
{
    m_markers.removeOne(marker);
}

/*!
    \fn void QPlotMarkerGroup::addDistance(QGraphicsItem *item)
    \brief Adds a distance measurement to the group (QGraphicsItem overload).
    \a item The distance item to add (must be a QPlotMarkerDistance)
    \note Only QPlotMarkerDistance items will be actually added
    \sa addDistance(QPlotMarkerDistance*)
*/
void QPlotMarkerGroup::addDistance(QGraphicsItem *item)
{
    if (auto distance = dynamic_cast<QPlotMarkerDistance *>(item))

        addDistance(distance);
}

/*!
    \fn void QPlotMarkerGroup::addDistance(QPlotMarkerDistance *distance)
    \brief Adds a QPlotMarkerDistance to the group.
    \a distance The distance measurement to add
    \sa removeDistance()
*/
void QPlotMarkerGroup::addDistance(QPlotMarkerDistance *distance)
{
    m_distances.append(distance);
}

/*!
    \fn void QPlotMarkerGroup::removeDistance(QPlotMarkerDistance *distance)
    \brief Removes a distance measurement from the group.
    \a distance The distance measurement to remove
    \sa addDistance()
*/
void QPlotMarkerGroup::removeDistance(QPlotMarkerDistance *distance)
{
    m_distances.removeOne(distance);
}

/*!
    \fn void QPlotMarkerGroup::clear()
    \brief Removes all markers from the group.
    \note Does not affect distance measurements
    \sa removeMarker()
*/
void QPlotMarkerGroup::clear()
{
    m_markers.clear();
}

/*!
    \fn void QPlotMarkerGroup::setSyncMovement(bool isSync)
    \brief Enables or disables synchronized movement of markers.
    \a isSync If true, markers will move together; if false, they move independently
    \note When enabled, moving any marker will move all others in the group
    \sa markers(), move(const QPointF&)
*/
void QPlotMarkerGroup::setSyncMovement(bool isSync)
{
    if (m_isSyncMovement == isSync)
        return;

    m_isSyncMovement = isSync;

    for (const auto marker : m_markers)

        if (m_isSyncMovement)

            connect(marker, &QPlotMarker::positionChanged, this, &QPlotMarkerGroup::move);

        else

            disconnect(marker, &QPlotMarker::positionChanged, this, &QPlotMarkerGroup::move);
}

/*!
    \fn void QPlotMarkerGroup::move(const QPointF &point)
    \brief Synchronizes movement of all markers in the group.
    \internal
    \a point The new position to move markers to
    \note Called automatically when sync movement is enabled and any marker moves
*/
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

/*!
    \fn QList<QPlotMarker *> QPlotMarkerGroup::markers() const
    \brief Returns all markers in the group.
    Return A list of pointers to markers in this group
    \sa addMarker()
*/
QList<QPlotMarker *> QPlotMarkerGroup::markers() const
{
    return m_markers;
}
