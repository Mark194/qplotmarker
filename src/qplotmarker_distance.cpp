#include "../include/QPlotMarker/qplotmarker_distance.hpp"
#include "qplotmarker_distance_p.hpp"

#include "../include/QPlotMarker/qplotmarker.hpp"

#include "entity/graphics_coord_item.hpp"

/*!
    \class QPlotMarkerDistance
    \inmodule QPlotMarker
    \brief A distance measurement widget between two QPlotMarker instances.

    The QPlotMarkerDistance displays and manages the visual representation
    of the distance between two QPlotMarker objects on a QChart.
*/

/*!
    \fn QPlotMarkerDistance::QPlotMarkerDistance()
    \brief Constructs a QPlotMarkerDistance with no parent.
*/
QPlotMarkerDistance::QPlotMarkerDistance()
    : d_ptr(new QPlotMarkerDistancePrivate(this))
{}

/*!
    \fn QPlotMarkerDistance::~QPlotMarkerDistance()
    \brief Destroys the distance marker.
*/
QPlotMarkerDistance::~QPlotMarkerDistance() {}

/*!
    \fn QPair<QPlotMarker *, QPlotMarker *> QPlotMarkerDistance::markers() const
    \brief Returns the pair of markers being measured.

    Return QPair containing the two markers (maybe null)

    \sa setMarkers()
*/
QPair<QPlotMarker *, QPlotMarker *> QPlotMarkerDistance::markers() const
{
    Q_D(const QPlotMarkerDistance);

    return {d->m_oneMarker, d->m_otherMarker};
}

/*!
    \fn void QPlotMarkerDistance::setMarker(QPlotMarker *one, QPlotMarker *other)
    \brief Sets the two markers to measure distance between.

    \a one
    First marker

    \a other
    Second marker

    \warning Both markers must belong to the same QChart
    \sa markers()
*/
void QPlotMarkerDistance::setMarkers(QPlotMarker *one, QPlotMarker *other)
{
    Q_D(QPlotMarkerDistance);

    d->init(one, other);

    d->paint();
}

void QPlotMarkerDistance::clearMarkers()
{
    Q_D(QPlotMarkerDistance);
}
/*!
    \fn void QPlotMarkerDistance::setLabelFormat(const QString &format)
    \brief Sets the format string for distance display.

    \a format
    printf-style format string (e.g., "%.2f units")
*/
void QPlotMarkerDistance::setLabelFormat(const QString &format)
{
    Q_D(QPlotMarkerDistance);

    d->m_coordInfo->setLabelFormat(format);
}

/*!
    \fn void QPlotMarkerDistance::setCoordFont(const QFont &font)
    \brief Sets the font for distance display.

    \a font
    The QFont to use for coordinate text display. The font will be applied

*/
void QPlotMarkerDistance::setCoordFont(const QFont &font)
{
    Q_D(QPlotMarkerDistance);

    d->m_coordInfo->setFont(font);
}

/*!
    \fn void QPlotMarkerDistance::setCoordPen(const QPen &pen)
    \brief Sets the pen for distance display.

    \a pen
    The QPen to use for coordinate styling. This controls:
    \list
        \li Text outline (if enabled)
        \li Border styling (if applicable)
        \li General stroke properties for coordinate element
    \endlist
*/
void QPlotMarkerDistance::setCoordPen(const QPen &pen)
{
    Q_D(QPlotMarkerDistance);

    d->m_coordInfo->setPen(pen);
}
void QPlotMarkerDistance::setCoordBackgroundColor(const QColor &color)
{
    Q_D(QPlotMarkerDistance);

    d->m_coordInfo->setBackgroundColor(color);
}
void QPlotMarkerDistance::setCoordItemColor(const QColor &color)
{
    Q_D(QPlotMarkerDistance);

    d->m_coordInfo->setItemColor(color);
}

/*!
    \fn void QPlotMarkerDistance::setAlignment(Qt::AlignmentFlag alignment)
    \brief Sets the label alignment using Qt flags.

    \a alignment
    Qt alignment flag (AlignTop, AlignBottom, or AlignCenter)

    \sa setAlignment(qreal)
*/
void QPlotMarkerDistance::setAlignment(Qt::AlignmentFlag alignment)
{
    Q_D(QPlotMarkerDistance);

    switch (alignment) {
    case Qt::AlignTop:
    case Qt::AlignBottom:
    case Qt::AlignCenter:

        d->setAlignment(alignment);

        break;

    default:
        qWarning("Invalid flag! Use AlignTop, AlignBottom, or AlignCenter.");
        break;
    }
}

/*!
    \fn void QPlotMarkerDistance::setAlignment(qreal precentAlignment)
    \brief Sets precise label alignment using percentage.

    \a precentAlignment
    Assertion failure if value outside [0.0, 1.0]

    \sa setAlignment(Qt::AlignmentFlag)
*/
void QPlotMarkerDistance::setAlignment(qreal precentAlignment)
{
    Q_D(QPlotMarkerDistance);

    Q_ASSERT_X(
        precentAlignment >= 0.0 and precentAlignment <= 1.0,
        "QPlotMarkerDistance::setAlignment(qreal precentAlignment)",
        "PrecentAlignment must be in range [0.0;1.0]!");

    d->m_precentAlignment = precentAlignment;

    d->setAlignment(Qt::AlignJustify);
}

/*!
    \fn qreal QPlotMarkerDistance::markersDistance(QPlotMarker *one, QPlotMarker *other)
    \brief Static method to calculate distance between any two markers.

    \a one
    The first marker

    \a other
    The second marker

    Return absolute distance between marker values

    \note This is a pure calculation - no visual representation
*/
qreal QPlotMarkerDistance::markersDistance(QPlotMarker *one, QPlotMarker *other)
{
    Q_ASSERT_X(one and other, "QPlotMarkerDistance::markersDistance()", "QPlotMarker is null!");

    return std::abs(other->markerValue() - one->markerValue());
}

/*!
    \fn void QPlotMarkerDistance::update()
    \brief Updates the visual representation of the distance measurement.
    \note Call this when marker positions change externally
*/
void QPlotMarkerDistance::update()
{
    Q_D(QPlotMarkerDistance);

    d->paint();
}
