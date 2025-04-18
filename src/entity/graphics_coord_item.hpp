#pragma once


#include <QGraphicsTextItem>


class GraphicsCoordItem : public QGraphicsTextItem
{
public:
    GraphicsCoordItem(QGraphicsItem * parent = nullptr);

    void setCoord( qreal value );

    void setItemColor(const QColor & itemColor);

    // QGraphicsItem interface

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

Q_SIGNALS:

    void onActivated(bool);

private:

    const qreal RECT_MARGIN = -3;

    QColor      m_itemColor;
};
