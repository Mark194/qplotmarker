#pragma once


#include <QGraphicsLineItem>
#include <QGraphicsSvgItem>
#include <QLineSeries>


#include <QPlotMarker/QPlotMarker>


class GraphicsCoordItem;
class MovableButton;
struct ViewCoordItem;


class QPlotMarkerPrivate
{
public:

    struct ViewCoordItem
    {
        GraphicsCoordItem    * coord;

        QGraphicsEllipseItem * point;
    };

    explicit QPlotMarkerPrivate(QPlotMarker * q);

    ~QPlotMarkerPrivate();

    void init(QChart * parent, const QColor & color, Qt::Orientation orientation);

    bool isPositionAcceptable(const QPointF& position) const;
    void loadIntersectionPoints(const QPointF& position);

    void clearInterSectionPoints();

    QPlotMarker * const q_ptr;

    QChart * m_parentChart = nullptr;
    QColor m_markerColor;
    QPlotMarker::MovementStyle m_movement = QPlotMarker::MOVEMENT_DEFAULT;
    Qt::Orientation m_orientation = Qt::Horizontal;

    MovableButton           * m_controlItem = nullptr;
    QGraphicsLineItem       * m_line        = nullptr;
    GraphicsCoordItem       * m_coordInfo   = nullptr;
    QList<ViewCoordItem>      m_intersectionItems;

    qreal       m_intersectionPointSize = 0.0;
    quint8      m_intersectionLineSize = 0;
    QString     m_labelFormat;

    QPointF     m_markerPosition;


    QList<QAbstractSeries *> m_ignoreSeries;

private:
    Q_DISABLE_COPY(QPlotMarkerPrivate)
};
