#pragma once


#include <QObject>
#include <QGraphicsItem>


#include "qplotmarker_global.hpp"


class QPlotMarker;
class QPlotMarkerDistance;


class QPLOTMARKER_EXPORT QPlotMarkerGroup : public QObject
{
    Q_OBJECT
public:
    explicit QPlotMarkerGroup(QObject *parent = nullptr);

    void addMarker(QGraphicsItem * marker);
    void addMarker(QPlotMarker * marker);
    void removeMarker(QPlotMarker * marker);


    void addDistance(QGraphicsItem * distance);
    void addDistance(QPlotMarkerDistance * distance);
    void removeDistance(QPlotMarkerDistance * distance);

    void clear();

    void setSyncMovement(bool isSync);


    QList<QPlotMarker *> markers() const;

public slots:

    void move(const QPointF & point);

private:

    bool m_isSyncMovement;

    QList<QPlotMarker *> m_markers;

    QList<QPlotMarkerDistance *> m_distances;
};
