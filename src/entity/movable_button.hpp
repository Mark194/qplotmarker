#pragma once


#include <QGraphicsItem>
#include <QGraphicsSvgItem>


class QPlotMarker;


class MovableButton : public QGraphicsItem
{
public:

    MovableButton(QPlotMarker * parent = nullptr);

    void setButtonIcon( const QString & fileName );

    void setButtonControl( const QString & fileName );

    void setSize(qreal size);

    void setColor(const QColor & color);

    // QGraphicsItem interface

    QRectF boundingRect() const override;

    void paint(QPainter * painter,
               const QStyleOptionGraphicsItem * option,
               QWidget * widget) override;

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

private:

    QPlotMarker      * m_plotMarker;

    QGraphicsSvgItem * m_buttonIcon;

    QGraphicsSvgItem * m_buttonControl;

    qreal m_size;
};
