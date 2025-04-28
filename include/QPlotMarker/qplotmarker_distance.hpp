#pragma once


#include <QGraphicsItem>


#include "qplotmarker_global.hpp"


class QPlotMarker;


class QPLOTMARKER_EXPORT QPlotMarkerDistance : public QGraphicsItem
{
public:

    QPlotMarkerDistance();

    static qreal markersDistance(QPlotMarker * one, QPlotMarker * other);
};
