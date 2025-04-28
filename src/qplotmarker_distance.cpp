#include "QPlotMarker/qplotmarker_distance.hpp"


#include <QPlotMarker/QPlotMarker>


QPlotMarkerDistance::QPlotMarkerDistance() {}

qreal QPlotMarkerDistance::markersDistance(QPlotMarker * one, QPlotMarker * other)
{
    return one->markerValue() - other->markerValue();
}
