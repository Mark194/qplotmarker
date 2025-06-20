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
    Q_PROPERTY(
        MovementStyle movement READ movementStyle WRITE setMovementStyle NOTIFY movementStyleChanged)
    Q_PROPERTY(Qt::Orientation orientation READ orientation)

public:
    enum MovementStyle { MOVEMENT_DEFAULT, MOVEMENT_BY_POINTS };
    Q_ENUM(MovementStyle)

    enum MarkerButtonIcon {
        MARKER_BUTTON_ICON,
        MARKER_BUTTON_CONTROL,
        MARKER_BUTTON_SELECTED_ICON,
        MARKER_BUTTON_SELECTED_CONTROL
    };
    Q_ENUM(MarkerButtonIcon)

    enum MarkerOrientation {
        Horizontal = Qt::Horizontal,
        Vertical = Qt::Vertical,
        HorizontalInverted,
        VerticalInverted
    };
    Q_ENUM(MarkerOrientation)

    explicit QPlotMarker(
        QChart *parent,
        const QColor &color = "black",
        QPlotMarker::MarkerOrientation orientation = QPlotMarker::Horizontal);

    explicit QPlotMarker(
        QChart *parent, const QColor &color = "black", Qt::Orientation orientation = Qt::Horizontal);

    ~QPlotMarker() override;

    QColor color() const;
    void setColor(const QColor &color);

    MovementStyle movementStyle() const;
    void setMovementStyle(MovementStyle style);

    Qt::Orientation orientation() const;
    MarkerOrientation markerOrientation() const;
    bool isInverted() const;

    void setSelected(bool isSelect);

    bool isIgnoreSelected() const;
    void setIsIgnoreSelected(bool isIgnore);

    QChart *chart() const;

    void move(const QPointF &position);
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

    void setLabelFormat(const QString &format);

    void addIgnoreSeries(QAbstractSeries *series);
    void removeIgnoreSeries(QAbstractSeries *series);
    void setIgnoreSeries(const QList<QAbstractSeries *> &series);

    QList<QAbstractSeries *> ignoreSeries() const;

    qreal controlIconSize() const;
    void setControlIconSize(qreal size);

    void setCoordFont(const QFont &font);
    void setCoordPen(const QPen &pen);

    QString markerIcon(MarkerButtonIcon typeIcon) const;
    void setMarkerIcon(const QString &fileName, MarkerButtonIcon typeIcon);

    QRectF controlRect() const;

    // QGraphicsItem interface
    QRectF boundingRect() const override;

Q_SIGNALS:
    void colorChanged(const QColor &color);
    void movementStyleChanged(MovementStyle style);
    void positionChanged(const QPointF &position);

    void pressed(Qt::MouseButton button);
    void released(Qt::MouseButton button);

    void doubleClicked(Qt::MouseButton button);

public slots:

    void showCoordinates(bool isVisible);

    void activate(bool isActivated);

    void update();

protected:
    QScopedPointer<QPlotMarkerPrivate> d_ptr;

    // QGraphicsItem interface

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    Q_DISABLE_COPY(QPlotMarker)
};
