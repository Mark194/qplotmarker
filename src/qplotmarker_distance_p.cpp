#include "qplotmarker_distance_p.hpp"

#include <QGraphicsScene>
#include <QPointF>

#include <QPlotMarker/qplotmarker_distance.hpp>

#include "effects/glow_effect.hpp"

#include "entity/graphics_coord_item.hpp"

QPlotMarkerDistancePrivate::QPlotMarkerDistancePrivate(QPlotMarkerDistance *q)
    : q_ptr(q), m_line(new QGraphicsLineItem(q_ptr)), m_coordInfo(new GraphicsCoordItem(q_ptr)),
      m_alignment(Qt::AlignBottom), m_precentAlignment(0.0) {}

QPlotMarkerDistancePrivate::~QPlotMarkerDistancePrivate() = default;

void QPlotMarkerDistancePrivate::init(QPlotMarker *one, QPlotMarker *other)
{
    Q_ASSERT_X(one and other, "QPlotMarkerDistancePrivate::init()", "QPlotMarker is null!");

    Q_ASSERT_X(
        one->chart() == other->chart(),
        "QPlotMarkerDistancePrivate::init()",
        "QPlotMarker's are on different charts!");

    Q_ASSERT_X(
        one->orientation() == other->orientation(),
        "QPlotMarkerDistancePrivate::init()",
        "QPlotMarker's have different orientations!");

    m_oneMarker = one;

    m_otherMarker = other;

    m_line->setPen(QPen(one->color(), 2, Qt::DotLine, Qt::RoundCap));

    if ( m_oneMarker->color() != m_otherMarker->color() ) {
        auto *effect = new GlowEffect();

        effect->setSource(m_line);

        QLinearGradient gradient;

        gradient.setColorAt(0, m_oneMarker->color());

        gradient.setColorAt(1, m_otherMarker->color());

        effect->setGradient(gradient);

        m_line->setGraphicsEffect(effect);
    }

    m_coordInfo->setItemColor(one->color());

    m_oneMarker->setZValue(1.0);

    m_otherMarker->setZValue(1.0);

    QObject::connect(
        m_oneMarker,
        &QPlotMarker::positionChanged,
        q_ptr,
        &QPlotMarkerDistance::update);

    QObject::connect(
        m_otherMarker,
        &QPlotMarker::positionChanged,
        q_ptr,
        &QPlotMarkerDistance::update);
}

void QPlotMarkerDistancePrivate::paint()
{
    const QPointF pointOne = m_oneMarker->pos();

    const QPointF pointOther = m_otherMarker->pos();

    const auto plotArea = m_oneMarker->chart()->plotArea();

    const auto diff = controlDifference();

    const auto orientation = m_oneMarker->orientation();

    if ( orientation == Qt::Vertical )

        m_line->setLine(
            pointOne.x(),
            plotArea.y() - diff,
            pointOther.x(),
            plotArea.topRight().y() - diff);

    else

        m_line->setLine(
            plotArea.topRight().x() + diff,
            pointOne.y(),
            plotArea.topRight().x() + diff,
            pointOther.y());

    m_coordInfo->setCoord(QPlotMarkerDistance::markersDistance(m_oneMarker, m_otherMarker));

    changeVisibleCoordItem();

    QPointF centerPoint = (m_line->line().p1() + m_line->line().p2()) / 2.0;

    QPointF coordPoint = centerPoint - QPointF(
                             m_coordInfo->boundingRect().width() / 2.0,
                             m_coordInfo->boundingRect().height() / 2.0);

    if ( orientation == Qt::Horizontal )

        coordPoint.setX(coordPoint.x() - diff / 2 - m_coordInfo->pen().widthF());

    normalizeCoordItem(coordPoint, m_oneMarker->chart()->rect());
}

void QPlotMarkerDistancePrivate::setAlignment(Qt::AlignmentFlag alignment)
{
    m_alignment = alignment;

    paint();
}

qreal QPlotMarkerDistancePrivate::controlDifference()
{
    auto orientation = m_oneMarker->orientation();

    auto controlRect = m_oneMarker->controlRect();

    switch ( m_alignment ) {
    case Qt::AlignTop:

        return orientation == Qt::Vertical ? controlRect.height() : controlRect.width();

    case Qt::AlignCenter:

        return orientation == Qt::Vertical ? controlRect.height() / 2 : controlRect.width() / 2;

    case Qt::AlignBottom:

        return 0;

    case Qt::AlignJustify:

        return orientation == Qt::Vertical
                   ? controlRect.height() * m_precentAlignment
                   : controlRect.width() * m_precentAlignment;

    default:
        return 0;
    }
}

void QPlotMarkerDistancePrivate::changeVisibleCoordItem()
{
    auto orientation = m_oneMarker->orientation();

    auto controlRect = m_oneMarker->isSelected()
                           ? m_otherMarker->controlRect()
                           : m_oneMarker->controlRect();

    if ( orientation == Qt::Vertical ) {
        m_coordInfo->setVisible(
            m_line->line().length() > m_coordInfo->boundingRect().width() + controlRect.width());
    } else {
        m_coordInfo->setVisible(
            m_line->line().length() > m_coordInfo->boundingRect().height() + controlRect.height());
    }
}

void QPlotMarkerDistancePrivate::normalizeCoordItem
(const QPointF &coordPoint, const QRectF &sceneRect)
{
    QRectF textRect = m_coordInfo->boundingRect();

    auto chart = m_oneMarker->chart()->margins();

    qreal boundedX = qBound(
        sceneRect.left(),
        coordPoint.x(),
        sceneRect.right() - textRect.width() - m_coordInfo->pen().widthF());

    qreal boundedY = qBound(
        sceneRect.top(),
        coordPoint.y(),
        sceneRect.bottom() - textRect.height() - m_coordInfo->pen().widthF());

    m_coordInfo->setPos(QPointF(boundedX, boundedY));
}
