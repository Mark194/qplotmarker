#include "movable_button.hpp"


#include <QGraphicsSceneMouseEvent>
#include <QSvgRenderer>


#include <QPlotMarker/qplotmarker.hpp>

#include <effects/fast_colorize_effect.hpp>


MovableButton::MovableButton(QGraphicsItem * parent)
    : QGraphicsItem( parent ),
      m_buttonIcon( new QGraphicsSvgItem( this ) ),
      m_buttonControl( new QGraphicsSvgItem( this ) ),
      m_size( 100 )
{
    setFlag( QGraphicsItem::ItemIsFocusable );

    setFlag( QGraphicsItem::ItemIsMovable );

    setFlag( QGraphicsItem::ItemIsSelectable );

    setFlag( QGraphicsItem::ItemSendsGeometryChanges );
}

void MovableButton::setButtonIcon(const QString & fileName)
{
    m_buttonIcon->setSharedRenderer( new QSvgRenderer( fileName ) );
}

void MovableButton::setButtonControl(const QString & fileName)
{
    m_buttonControl->setSharedRenderer( new QSvgRenderer( fileName ) );
}

void MovableButton::setSize(qreal size)
{
    m_size = size;


    auto originalSize = m_buttonIcon->renderer()->defaultSize();

    auto scaleFactor = size / originalSize.width();

    m_buttonIcon->setScale( scaleFactor );


    originalSize = m_buttonControl->renderer()->defaultSize();

    scaleFactor = size / originalSize.width();

    m_buttonControl->setScale( scaleFactor );
}

void MovableButton::setColor(const QColor & color)
{
    auto effect = new FastColorizeEffect( m_buttonIcon );

    effect->setColor( color );

    m_buttonIcon->setGraphicsEffect( effect );
}

QRectF MovableButton::boundingRect() const
{
    QRectF bgRect = m_buttonControl->boundingRect();

    bgRect.setWidth( m_size );

    bgRect.setHeight( m_size );

    return bgRect;
}

void MovableButton::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED( painter )

    Q_UNUSED(option)

    Q_UNUSED(widget)
}

void MovableButton::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    auto plotMarker = dynamic_cast<QPlotMarker *>( parentItem() );


    if ( ( event->modifiers() & Qt::ControlModifier ) != 0 )
    {
        plotMarker->setSelect( not plotMarker->isSelected() );

        return;
    }


    plotMarker->setSelect( true );

    plotMarker->move( event->scenePos() );
}

std::optional<QPointF> findNearestPoint(
    const QPointF & targetPoint,
    QLineSeries * series,
    bool findLeft = false         )
{
    if ( not series or series->count() == 0 ) return {};

    const auto points = series->points();

    std::optional<QPointF> nearesPoint;

    auto [begin, end] = findLeft? std::make_pair( points.constEnd() - 1, points.constBegin() - 1) :
                            std::make_pair( points.constBegin(),   points.constEnd() );

    const int step = findLeft? -1 : 1;

    for ( auto it = begin; it != end; it += step )
    {
        const QPointF & point = *it;

        if ( findLeft and targetPoint.x() < point.x() ) continue;

        if ( not findLeft and targetPoint.x() > point.x() ) continue;

        nearesPoint = point;

        break;
    }

    return nearesPoint;
}

void MovableButton::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    auto plotMarker = dynamic_cast<QPlotMarker *>( parentItem() );

    if ( plotMarker->movementStyle() != QPlotMarker::MOVEMENT_BY_POINTS )
    {
        plotMarker->move( event->scenePos() );

        return;
    }

    bool findLeft = event->scenePos().x() < event->lastScenePos().x();

    auto series = dynamic_cast<QLineSeries * >( plotMarker->chart()->series().first() );

    if ( not series ) return;

    auto valuePoint = plotMarker->chart()->mapToValue( event->scenePos(), series );

    auto point = findNearestPoint( valuePoint, series, findLeft );

    if ( not point )

    point = findLeft? series->points().first() : series->points().last();


    auto scenePoint = plotMarker->chart()->mapToPosition( point.value(), series );

    plotMarker->move( scenePoint );
}

void MovableButton::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if ( ( event->modifiers() & Qt::ControlModifier ) != 0 )

        return;

    Q_UNUSED(event)

    auto plotMarker = dynamic_cast<QPlotMarker *>( parentItem() );

    plotMarker->setSelect( false );
}
