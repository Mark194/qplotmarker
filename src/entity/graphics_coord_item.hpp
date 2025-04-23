#pragma once


#include <QGraphicsTextItem>


class GraphicsCoordItem : public QGraphicsTextItem
{
    Q_OBJECT

    Q_PROPERTY(qreal coord READ coord WRITE setCoord)

public:
    GraphicsCoordItem(QGraphicsItem * parent = nullptr);

    void setCoord( qreal value );
    qreal coord() const;


    void setItemColor(const QColor & itemColor);

    void setLabelFormat(const QString & format);

    // QGraphicsItem interface

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

Q_SIGNALS:

    void onActivated(bool);

private:

    QColor      m_itemColor;

    qreal       m_coord;

    QString     m_labelFormat;
};
