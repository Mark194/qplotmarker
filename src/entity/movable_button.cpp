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
    setFlag( QGraphicsItem::ItemIsFocusable );

    setFlag( QGraphicsItem::ItemIsMovable );

    setFlag( QGraphicsItem::ItemIsSelectable );

    setFlag( QGraphicsItem::ItemSendsGeometryChanges );
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
    auto serieses = m_plotMarker->chart()->series();

    if ( serieses.isEmpty() ) return;

    QList<QXYSeries *> successSeries;

    auto ignoreSeries = m_plotMarker->ignoreSeries();

    for ( auto series : serieses )
    {
        auto seriesXY = dynamic_cast<QXYSeries *>( series );

        if ( seriesXY
             and seriesXY->count() != 0
             and not ignoreSeries.contains( series ) )

            successSeries.append( seriesXY );
    }

    if ( successSeries.isEmpty() ) return;


    QList<QPointF> nearestPoints;

    for ( auto series : successSeries )
    {
        auto valuePoint = m_plotMarker->chart()->mapToValue( position, series  );

        auto point = PlotGeometryUtils::findNearestPoint( valuePoint, series, isFindLeft );

        if ( not point ) point = isFindLeft? series->points().first() : series->points().last();

        nearestPoints.append( point.value() );
    }

    QPointF closestPoint;

    qreal minDistSq = std::numeric_limits<qreal>::max();

    auto targetValue = m_plotMarker->chart()->mapToValue( position );

    bool isFind = false;

    for ( const auto & point : nearestPoints )
    {
        qreal dist = PlotGeometryUtils::distance( point, targetValue );

        if ( dist < minDistSq )
        {
            minDistSq = dist;

            closestPoint = point;

            isFind = true;
        }
    }

    m_plotMarker->move( closestPoint );
}

void MovableButton::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    emit m_plotMarker->clicked( event->button() );

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
    if ( ( event->modifiers() & Qt::ControlModifier ) != 0 )

        return;

    Q_UNUSED(event)

    m_plotMarker->setSelected( false );
}

void MovableButton::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    emit m_plotMarker->doubleClicked( event->button() );
}
