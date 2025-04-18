#pragma once


#include <QChart>
#include <QGraphicsSvgItem>
#include <QLineSeries>


#include "qplotmarker_global.hpp"


class GraphicsCoordItem;
class MovableButton;


class QPLOTMARKER_EXPORT QPlotMarker : public QGraphicsWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor markerColor READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(MovementStyle movement READ movementStyle WRITE setMovementStyle NOTIFY movementStyleChanged)
    Q_PROPERTY(Qt::Orientation orientation READ orientation)
    Q_ENUMS(MovementStyle)

public:

    const qreal DEFAULT_PIXMAP_SIZE = 25.0;

    enum MovementStyle
    {
        MOVEMENT_DEFAULT,
        MOVEMENT_BY_POINTS
    };


    QPlotMarker(QChart * parent,
                const QColor & color = "black",
                Qt::Orientation orientation = Qt::Horizontal);

    virtual ~QPlotMarker();

    void setColor(const QColor & color);
    QColor color() const;

    void setMovementStyle( MovementStyle style );
    MovementStyle movementStyle() const;

    Qt::Orientation orientation() const;


    void setSelect(bool isSelect);

    QChart * chart() const;


    void move(const QPointF & position);

    void moveBegin();

    void moveEnd();


    bool hasFocus() const;

    // QGraphicsItem interface

    QRectF boundingRect() const;

Q_SIGNALS:
    void colorChanged(const QColor & color);
    void movementStyleChanged(MovementStyle style);
    void onPositionChanged(const QPointF & position);

public slots:

    void showCoord();

    void activate( bool isActivated );

private:

    QChart        * m_parent;

    QColor          m_markerColor;

    MovementStyle   m_movement;

    Qt::Orientation m_orientation;

    MovableButton * m_controlItem;

    QGraphicsLineItem * m_line;

    GraphicsCoordItem * m_coordInfo;

    QList<QGraphicsItem *> m_items;

    bool isPositionAccept(const QPointF & position);

    void change(const QRectF & plotArea);

    void loadPoints(const QPointF & position);
};
