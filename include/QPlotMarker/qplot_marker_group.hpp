#pragma once


#include <QObject>
#include <QGraphicsItem>


#include "qplotmarker_global.hpp"


class QPlotMarker;


class QPLOTMARKER_EXPORT QPlotMarkerGroup : public QObject
{
    Q_OBJECT
public:
    explicit QPlotMarkerGroup(QObject *parent = nullptr);

    void addMarker(QGraphicsItem * marker);

    void addMarker(QPlotMarker * marker);

    void removeMarker(QPlotMarker * marker);

    void clear();

public slots:

    void move(const QPointF & point);

private:

    QList<QPlotMarker *> m_markers;
};
