#include "movable_button.hpp"


#include <QGraphicsSceneMouseEvent>
#include <QSvgRenderer>


#include <QPlotMarker/qplotmarker.hpp>

#include <effects/fast_colorize_effect.hpp>

#include <utility/plot_geometry_utils.hpp>


MovableButton::MovableButton(QPlotMarker * parent)
    : QGraphicsItem( parent ),
      m_plotMarker(parent),
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
    if ( ( event->modifiers() & Qt::ControlModifier ) != 0 )
    {
        m_plotMarker->setSelected( not m_plotMarker->isSelected() );

        return;
    }


    m_plotMarker->setSelected( true );

    m_plotMarker->move( event->scenePos() );
}

void MovableButton::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if ( m_plotMarker->movementStyle() != QPlotMarker::MOVEMENT_BY_POINTS )
    {
        m_plotMarker->move( event->scenePos() );

        return;
    }

    bool findLeft = event->scenePos().x() < event->lastScenePos().x();

    auto serieses = m_plotMarker->chart()->series();

    if ( serieses.isEmpty() ) return;

    auto series = dynamic_cast<QLineSeries * >( serieses.first() );

    if ( not series ) return;

    auto valuePoint = m_plotMarker->chart()->mapToValue( event->scenePos(),
                                                         series             );

    auto point = PlotGeometryUtils::findNearestPoint( valuePoint, series, findLeft );

    if ( not point )

    point = findLeft? series->points().first() : series->points().last();


    auto scenePoint = m_plotMarker->chart()->mapToPosition( point.value(),
                                                            series          );

    m_plotMarker->move( scenePoint );
}

void MovableButton::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if ( ( event->modifiers() & Qt::ControlModifier ) != 0 )

        return;

    Q_UNUSED(event)

    m_plotMarker->setSelected( false );
}
