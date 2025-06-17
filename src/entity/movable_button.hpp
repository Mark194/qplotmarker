#pragma once

#include <QGraphicsSvgItem>

class QPlotMarker;

class MovableButton : public QGraphicsItem
{
public:
    explicit MovableButton(QPlotMarker *parent = nullptr);

    void setButtonIcon(const QString &fileName);

    void setButtonControl(const QString &fileName);

    [[nodiscard]] qreal size() const;
    void setSize(qreal size);

    void setColor(const QColor &color);

    // QGraphicsItem interface

    [[nodiscard]] QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void move(const QPointF &position, bool isFindLeft);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPlotMarker *m_plotMarker;

    QGraphicsSvgItem *m_buttonIcon;

    QGraphicsSvgItem *m_buttonControl;

    qreal m_size;
};
