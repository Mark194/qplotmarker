#pragma once

#include <QGraphicsEffect>

class FastColorizeEffect : public QGraphicsEffect
{
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    explicit FastColorizeEffect(QObject *parent = nullptr);

    [[nodiscard]] QColor color() const;

    void setColor(const QColor &color);

protected:
    void draw(QPainter *painter) override;

    void sourceChanged(ChangeFlags flags) override;

private:
    QColor m_color;

    QPixmap m_cachedPixmap;
};
