#pragma once

#include <QBrush>
#include <QGraphicsTextItem>
#include <QPen>

class GraphicsCoordItem final : public QGraphicsTextItem
{
    Q_OBJECT

    Q_PROPERTY(qreal coord READ coord WRITE setCoord)

public:
    explicit GraphicsCoordItem(QGraphicsItem *parent = nullptr);

    [[nodiscard]] qreal coord() const;
    void setCoord(qreal value);

    [[nodiscard]] QPen pen() const;
    void setPen(const QPen &pen);

    void setItemColor(const QColor &itemColor);

    void setLabelFormat(const QString &format);

    // QGraphicsItem interface

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

Q_SIGNALS:

    void onActivated(bool);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    QColor m_itemColor;

    qreal m_coord{};

    QString m_labelFormat;

    QPen m_coordPen;
};
