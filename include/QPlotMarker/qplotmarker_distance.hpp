#pragma once


#include <QGraphicsWidget>


#include "qplotmarker_global.hpp"


class QPlotMarker;
class QPlotMarkerDistancePrivate;


class QPLOTMARKER_EXPORT QPlotMarkerDistance : public QGraphicsWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QPlotMarkerDistance)

public:

    explicit QPlotMarkerDistance();

    ~QPlotMarkerDistance();

    QPair<QPlotMarker *, QPlotMarker *> markers() const;
    void setMarker(QPlotMarker * one, QPlotMarker * other);


    void setLabelFormat(const QString & format);

    void setCoordFont(const QFont & font);
    void setCoordPen(const QPen & pen);


    void setAlignment(Qt::AlignmentFlag alignment);

    void setAlignment(qreal precentAlignment);

    static qreal markersDistance(QPlotMarker * one, QPlotMarker * other);


public slots:

    void update();

protected:

    QScopedPointer<QPlotMarkerDistancePrivate> d_ptr;

private:
    Q_DISABLE_COPY(QPlotMarkerDistance)
};
