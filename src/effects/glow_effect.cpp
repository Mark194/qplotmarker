#include "glow_effect.hpp"


#include <QGraphicsLineItem>
#include <QPainter>


GlowEffect::GlowEffect(QObject * parent)
    : QGraphicsEffect(parent)
{

}

void GlowEffect::draw(QPainter * painter)
{
    // 1. Рисуем исходный элемент (включая все его стандартные эффекты)
    drawSource(painter);

    // 2. Добавляем свечение поверх
    if (auto* lineItem = dynamic_cast<QGraphicsLineItem*>( m_source ))
    {
        painter->save();

        // Настраиваем градиент вдоль линии
        QLineF line = lineItem->line();
        m_gradient.setStart(line.p1());
        m_gradient.setFinalStop(line.p2());

        // Создаем перо с градиентом (толще оригинального на 5px)
        QPen glowPen = lineItem->pen();
        glowPen.setWidth(glowPen.width());
        glowPen.setBrush(QBrush(m_gradient));

        // Рисуем линию свечения
        painter->setPen(glowPen);
        painter->drawLine(line);

        painter->restore();
    }
}

void GlowEffect::setSource(QGraphicsItem * source)
{
    m_source = source;
}

void GlowEffect::setGradient(const QLinearGradient & gradient)
{
    m_gradient = gradient;

    update();
}
