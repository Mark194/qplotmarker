#include "glow_effect.hpp"

#include <QGraphicsLineItem>
#include <QPainter>

GlowEffect::GlowEffect(QObject *parent)
    : QGraphicsEffect(parent)
    , m_source(nullptr)
{}

void GlowEffect::draw(QPainter *painter)
{
    drawSource(painter);

    if (const auto *lineItem = dynamic_cast<QGraphicsLineItem *>(m_source)) {
        painter->save();

        const QLineF line = lineItem->line();
        m_gradient.setStart(line.p1());
        m_gradient.setFinalStop(line.p2());

        QPen glowPen = lineItem->pen();
        glowPen.setWidth(glowPen.width());
        glowPen.setBrush(QBrush(m_gradient));

        painter->setPen(glowPen);
        painter->drawLine(line);

        painter->restore();
    }
}

void GlowEffect::setSource(QGraphicsItem *source)
{
    m_source = source;
}

void GlowEffect::setGradient(const QLinearGradient &gradient)
{
    m_gradient = gradient;

    update();
}
