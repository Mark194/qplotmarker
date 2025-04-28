#pragma once


#include <QObject>


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

    #include <QtCharts>

    using namespace QtCharts;

#endif


#include <QChart>
#include <QGraphicsSvgItem>


#include "qplotmarker_global.hpp"


class QPlotMarkerPrivate;


class QPLOTMARKER_EXPORT QPlotMarker : public QGraphicsWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPlotMarker)
    Q_PROPERTY(QColor markerColor READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(MovementStyle movement READ movementStyle WRITE setMovementStyle NOTIFY movementStyleChanged)
    Q_PROPERTY(Qt::Orientation orientation READ orientation)

public:

    enum MovementStyle
    {
        MOVEMENT_DEFAULT,
        MOVEMENT_BY_POINTS
    };
    Q_ENUM(MovementStyle)


    explicit QPlotMarker(QChart * parent,
                         const QColor & color = "black",
                         Qt::Orientation orientation = Qt::Horizontal);

    virtual ~QPlotMarker();


    QColor color() const;
    void setColor(const QColor & color);

    MovementStyle movementStyle() const;
    void setMovementStyle( MovementStyle style );


    Qt::Orientation orientation() const;


    void setSelected(bool isSelect);
    QChart * chart() const;


    void move(const QPointF & position);
    void move(qreal percent);
    void moveBegin();
    void moveEnd();
    void moveToNextPoint();
    void moveToPreviousPoint();

    QPointF pos() const;


    bool hasFocus() const;
    qreal markerValue() const;


    void setIntersectionPointSize(qreal size);
    void setIntersectionLineSize(quint8 size);
    quint8 intersectionLineSize() const;

    void setLabelFormat(const QString & format);


    void addIgnoreSeries( QAbstractSeries * series );
    void removeIgnoreSeries( QAbstractSeries * series );
    void setIgnoreSeries(const QList<QAbstractSeries *> & series );

    QList<QAbstractSeries *> ignoreSeries() const;

    qreal controlIconSize() const;
    void setControlIconSize(qreal size);

    // QGraphicsItem interface
    QRectF boundingRect() const;

Q_SIGNALS:
    void colorChanged(const QColor & color);
    void movementStyleChanged(MovementStyle style);
    void positionChanged(const QPointF & position);
<<<<<<< HEAD
    void clicked(Qt::MouseButton button);
    void doubleClicked(Qt::MouseButton button);
=======
    void clicked();
    void doubleClicked();
>>>>>>> origin/main

public slots:

    void showCoordinates();

    void activate( bool isActivated );

    void update();

protected:

    QScopedPointer<QPlotMarkerPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QPlotMarker)
};
