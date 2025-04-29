#include "../include/QPlotMarker/qplotmarker_distance.hpp"
#include "../include/QPlotMarker/qplotmarker.hpp"


#include "qplotmarker_distance_p.hpp"
#include "entity/graphics_coord_item.hpp"


QPlotMarkerDistance::QPlotMarkerDistance()
    : d_ptr(new QPlotMarkerDistancePrivate( this ))
{}

QPlotMarkerDistance::~QPlotMarkerDistance()
{

}

QPair<QPlotMarker *, QPlotMarker *> QPlotMarkerDistance::markers() const
{
    Q_D(const QPlotMarkerDistance);

    return { d->m_oneMarker, d->m_otherMarker };
}

void QPlotMarkerDistance::setMarker(QPlotMarker * one, QPlotMarker * other)
{
    Q_D(QPlotMarkerDistance);

    d->init( one, other );

    d->paint();
}

void QPlotMarkerDistance::setLabelFormat(const QString & format)
{
    Q_D(QPlotMarkerDistance);

    d->m_coordInfo->setLabelFormat( format );
}

void QPlotMarkerDistance::setCoordFont(const QFont & font)
{
    Q_D(QPlotMarkerDistance);

    d->m_coordInfo->setFont( font );
}

void QPlotMarkerDistance::setCoordPen(const QPen & pen)
{
    Q_D(QPlotMarkerDistance);

    d->m_coordInfo->setPen( pen );
}

qreal QPlotMarkerDistance::markersDistance(QPlotMarker * one, QPlotMarker * other)
{
    return other->markerValue() - one->markerValue();
}

void QPlotMarkerDistance::update()
{
    Q_D(QPlotMarkerDistance);

    d->paint();
}
