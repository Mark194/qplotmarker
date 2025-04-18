#include "QPlotMarker/qplot_marker_group.hpp"


#include "QPlotMarker/qplotmarker.hpp"


QPlotMarkerGroup::QPlotMarkerGroup(QObject * parent)
{

}

void QPlotMarkerGroup::addMarker(QGraphicsItem * marker)
{
    if ( auto plotMarker = dynamic_cast<QPlotMarker *>( marker ) )

        addMarker( plotMarker );
}

void QPlotMarkerGroup::addMarker(QPlotMarker * marker)
{
    m_markers.append( marker );

    connect( marker, &QPlotMarker::onPositionChanged,
             this,   &QPlotMarkerGroup::move          );
}

void QPlotMarkerGroup::removeMarker(QPlotMarker * marker)
{
    m_markers.removeOne( marker );
}

void QPlotMarkerGroup::clear()
{
    m_markers.clear();
}

void QPlotMarkerGroup::move(const QPointF & point)
{
    auto senderMarker = dynamic_cast<QPlotMarker *>( sender() );

    for ( auto marker : m_markers )
    {
        if ( marker == senderMarker ) continue;

        marker->blockSignals( true );

        marker->move( point );

        marker->blockSignals( false );
    }
}
