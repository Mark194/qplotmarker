#pragma once

#include <QPlotMarker/QPlotMarker>

class GraphicsCoordItem;
class MovableButton;
struct ViewCoordItem;

class QPlotMarkerPrivate
{
public:
    struct ViewCoordItem
    {
        GraphicsCoordItem *coord;

        QGraphicsEllipseItem *point;
    };

    explicit QPlotMarkerPrivate(QPlotMarker *q);

    ~QPlotMarkerPrivate();

    void init(QChart *parent, const QColor &color, QPlotMarker::MarkerOrientation orientation);

    bool isPositionAcceptable(const QPointF &position) const;

    static QPointF adjustTextItemPosition(
        const QPointF &viewPoint, const GraphicsCoordItem *textItem, const QRectF &plotArea);

    void loadIntersectionPoints(const QPointF &position);
    void moveMarkerToPosition(const QPointF &position);
    void updateOnMoveByPoints(const QPointF &targetPoint);
    void clearInterSectionPoints();

    QPlotMarker::MarkerOrientation orientation(Qt::Orientation orientation);
    Qt::Orientation orientation() const;

    bool isInverted() const;
    bool isVertical() const;

    QPlotMarker *const q_ptr;

    QChart *m_parentChart = nullptr;
    QColor m_markerColor;
    QPlotMarker::MovementStyle m_movement = QPlotMarker::MOVEMENT_DEFAULT;
    QPlotMarker::MarkerOrientation m_orientation = QPlotMarker::Horizontal;

    MovableButton *m_controlItem = nullptr;
    QGraphicsLineItem *m_line = nullptr;
    GraphicsCoordItem *m_coordInfo = nullptr;
    QList<ViewCoordItem> m_intersectionItems;

    qreal m_intersectionPointSize = 0.0;
    quint8 m_intersectionLineSize = 0;
    QString m_labelFormat;

    QPointF m_markerPosition;

    QPointF m_markerValue;

    QList<QAbstractSeries *> m_ignoreSeries;

    bool m_isVisibleCoords;
    bool m_isSelectedLock;

    QString m_buttonIcon;
    QString m_buttonControl;
    QString m_buttonSelectedIcon;
    QString m_buttonsSelectedControl;

private:
    Q_DISABLE_COPY(QPlotMarkerPrivate)

    void setupVerticalMarker(
        const QPointF &position, const QRectF &plotArea, const QRectF &controlRect, bool inverted);
    void setupHorizontalMarker(
        const QPointF &position, const QRectF &plotArea, const QRectF &controlRect, bool inverted);
};
