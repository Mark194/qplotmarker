#pragma once

#include <QGraphicsEffect>
#include <QGraphicsItem>

class GlowEffect : public QGraphicsEffect
{
public:
    explicit GlowEffect(QObject *parent = nullptr);

    // QGraphicsEffect interface
    void setGradient(const QLinearGradient &gradient);

    void setSource(QGraphicsItem *source);

protected:
    void draw(QPainter *painter) override;

private:
    QGraphicsItem *m_source;

    QLinearGradient m_gradient;
};
