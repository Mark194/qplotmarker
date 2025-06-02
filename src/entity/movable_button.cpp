#include "movable_button.hpp"


#include <limits>


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
      m_size( 25.0 )
{
    setFlag( ItemIsFocusable );

    setFlag( ItemIsMovable );

    setFlag( ItemIsSelectable );

    setFlag( ItemSendsGeometryChanges );
}

void MovableButton::setButtonIcon(const QString & fileName)
{
    m_buttonIcon->setSharedRenderer( new QSvgRenderer( fileName ) );

    setSize( m_size );
}

void MovableButton::setButtonControl(const QString & fileName)
{
    m_buttonControl->setSharedRenderer( new QSvgRenderer( fileName ) );

    setSize( m_size );
}

qreal MovableButton::size() const
{
    return m_size;
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

QPointF MovableButton::value() const
{
    return m_value;
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

void MovableButton::move(const QPointF & position, bool isFindLeft)
{
    auto closestPoint = PlotGeometryUtils::findClosestPoint( m_plotMarker, position, isFindLeft );

    if ( not closestPoint ) return;

    m_value = closestPoint.value();

    m_plotMarker->move( m_plotMarker->chart()->mapToPosition( closestPoint.value() ) );
}

void MovableButton::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    emit m_plotMarker->pressed( event->button() );

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

    bool isFindLeft = event->scenePos().x() < event->lastScenePos().x();

    move( event->scenePos(), isFindLeft );
}

void MovableButton::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    emit m_plotMarker->released( event->button() );

    if ( ( event->modifiers() & Qt::ControlModifier ) != 0 )

        return;

    Q_UNUSED(event)

    m_plotMarker->setSelected( false );
}

void MovableButton::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    emit m_plotMarker->doubleClicked( event->button() );
}
