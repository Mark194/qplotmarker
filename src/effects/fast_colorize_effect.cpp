#include "fast_colorize_effect.hpp"


#include <QPainter>


FastColorizeEffect::FastColorizeEffect(QObject * parent)
    : QGraphicsEffect(parent), m_color(Qt::red)
{}

QColor FastColorizeEffect::color() const
{
    return m_color;
}

void FastColorizeEffect::setColor(const QColor & color)
{
    if ( m_color == color ) return;

    m_color = color;

    m_cachedPixmap = QPixmap();

    update();
}

void FastColorizeEffect::draw(QPainter * painter)
{
    if (m_cachedPixmap.isNull())
    {
        QPixmap source = sourcePixmap(Qt::LogicalCoordinates, nullptr, NoPad);

        if (source.isNull()) return;

        m_cachedPixmap = QPixmap(source.size());

        m_cachedPixmap.fill(Qt::transparent);

        QPainter pixmapPainter(&m_cachedPixmap);

        pixmapPainter.setCompositionMode(QPainter::CompositionMode_Source);

        pixmapPainter.drawPixmap(0, 0, source);

        pixmapPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);

        pixmapPainter.fillRect(source.rect(), m_color);

        pixmapPainter.end();
    }

    painter->drawPixmap(0, 0, m_cachedPixmap);
}

void FastColorizeEffect::sourceChanged(ChangeFlags flags)
{
    QGraphicsEffect::sourceChanged(flags);

    m_cachedPixmap = QPixmap();
}
