#include "../include/QPlotMarker/qplotmarker_distance.hpp"
#include "../include/QPlotMarker/qplotmarker.hpp"

#include "entity/graphics_coord_item.hpp"
#include "qplotmarker_distance_p.hpp"

QPlotMarkerDistance::QPlotMarkerDistance()
    : d_ptr(new QPlotMarkerDistancePrivate(this))
{}

QPlotMarkerDistance::~QPlotMarkerDistance() {}

QPair<QPlotMarker *, QPlotMarker *> QPlotMarkerDistance::markers() const
{
    Q_D(const QPlotMarkerDistance);

    return {d->m_oneMarker, d->m_otherMarker};
}

void QPlotMarkerDistance::setMarker(QPlotMarker *one, QPlotMarker *other)
{
    Q_D(QPlotMarkerDistance);

    d->init(one, other);

    d->paint();
}

void QPlotMarkerDistance::setLabelFormat(const QString &format)
{
    Q_D(QPlotMarkerDistance);

    d->m_coordInfo->setLabelFormat(format);
}

void QPlotMarkerDistance::setCoordFont(const QFont &font)
{
    Q_D(QPlotMarkerDistance);

    d->m_coordInfo->setFont(font);
}

void QPlotMarkerDistance::setCoordPen(const QPen &pen)
{
    Q_D(QPlotMarkerDistance);

    d->m_coordInfo->setPen(pen);
}

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

qreal QPlotMarkerDistance::markersDistance(QPlotMarker *one, QPlotMarker *other)
{
    return std::abs(other->markerValue() - one->markerValue());
}

void QPlotMarkerDistance::update()
{
    Q_D(QPlotMarkerDistance);

    d->paint();
}
