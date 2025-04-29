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

    void setAlignment(Qt::AlignmentFlag alignment);

    QPlotMarkerDistance * const q_ptr;

    QPlotMarker * m_oneMarker       = nullptr;
    QPlotMarker * m_otherMarker     = nullptr;

    QGraphicsLineItem * m_line      = nullptr;
    GraphicsCoordItem * m_coordInfo = nullptr;

    Qt::AlignmentFlag m_alignment;

    qreal m_precentAlignment;

private:
    Q_DISABLE_COPY(QPlotMarkerDistancePrivate)

    qreal controlDifference();

    void changeVisibleCoordItem();

    void normalizeCoordItem(const QPointF & coordPoint, const QRectF & plotAreaRect);
};
