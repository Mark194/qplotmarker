#pragma once


#include <QPlotMarker/QPlotMarker>
#include <QPlotMarker/QPlotMarkerDistance>


class GraphicsCoordItem;


class QPlotMarkerDistancePrivate
{
public:

    explicit QPlotMarkerDistancePrivate(QPlotMarkerDistance * q);

    ~QPlotMarkerDistancePrivate();

    void init(QPlotMarker * one, QPlotMarker * other);

    void paint();


    QPlotMarkerDistance * const q_ptr;

    QPlotMarker * m_oneMarker       = nullptr;
    QPlotMarker * m_otherMarker     = nullptr;

    QGraphicsLineItem * m_line      = nullptr;
    GraphicsCoordItem * m_coordInfo = nullptr;

private:
    Q_DISABLE_COPY(QPlotMarkerDistancePrivate)
};
