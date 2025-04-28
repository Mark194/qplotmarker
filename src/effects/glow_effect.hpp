#pragma once


#include <QGraphicsItem>
#include <QGraphicsEffect>


class GlowEffect : public QGraphicsEffect
{
public:
    GlowEffect(QObject *parent = nullptr);

    // QGraphicsEffect interface
    void setGradient(const QLinearGradient & gradient);

    void setSource(QGraphicsItem * source);

protected:
    void draw(QPainter * painter);

private:

    QGraphicsItem * m_source;

    QLinearGradient m_gradient;
};
