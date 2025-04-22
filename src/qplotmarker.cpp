#include "../include/QPlotMarker/qplotmarker.hpp"


#include <entity/graphics_coord_item.hpp>
#include <entity/movable_button.hpp>

#include <QGraphicsOpacityEffect>
#include <QGraphicsScene>
#include <QValueAxis>


struct ViewCoordItem
{
    GraphicsCoordItem    * coord;

    QGraphicsEllipseItem * point;
};


QPlotMarker::QPlotMarker(QChart * parent, const QColor & color, Qt::Orientation orientation)
    : QGraphicsWidget(),
      m_parent( parent ),
      m_controlItem( new MovableButton( this ) ),
      m_line( new QGraphicsLineItem( this ) ),
      m_orientation( orientation ),
      m_coordInfo( new GraphicsCoordItem( this ) ),
      m_movement( MovementStyle::MOVEMENT_DEFAULT ),
      m_intersectionPointSize( 2 )
{
    setFlag( QGraphicsItem::ItemIsSelectable );

    m_line->setPen( QPen( color, 2, Qt::DotLine, Qt::RoundCap  ) );

    m_controlItem->setColor( color );

    m_coordInfo->setItemColor( color );

    m_markerColor = color;

    setSelect( false );

    connect( parent, &QChart::plotAreaChanged, this, &QPlotMarker::change );
}

QPlotMarker::~QPlotMarker()
{
    delete m_controlItem;

    delete m_line;

    delete m_coordInfo;

    m_items.clear();
}

void QPlotMarker::setColor(const QColor & color)
{
    m_markerColor = color;

    m_line->setPen( QPen( m_markerColor, 2 ) );

    m_controlItem->setColor( m_markerColor );
}

QColor QPlotMarker::color() const
{
    return m_markerColor;
}

void QPlotMarker::setMovementStyle(MovementStyle style)
{
    m_movement = style;

    moveBegin();
}

QPlotMarker::MovementStyle QPlotMarker::movementStyle() const
{
    return m_movement;
}

Qt::Orientation QPlotMarker::orientation() const
{
    return m_orientation;
}

QChart * QPlotMarker::chart() const
{
    return m_parent;
}

void QPlotMarker::setSelect(bool isSelect)
{
    if ( isSelect )
    {
        m_controlItem->setButtonIcon( ":/marker_selected_icon" );

        m_controlItem->setButtonControl( ":/marker_selected_eye" );
    }
    else
    {
        m_controlItem->setButtonIcon( ":/marker_icon" );

        m_controlItem->setButtonControl( ":/marker_eye" );
    }


    if ( m_orientation == Qt::Horizontal )

    m_controlItem->setRotation( 90 );


    m_controlItem->setSize( DEFAULT_PIXMAP_SIZE );

    QGraphicsItem::setSelected( isSelect );
}

void QPlotMarker::move(const QPointF & position)
{
    if ( not isPositionAccept( position ) ) return;

    QGraphicsItem::setPos( position );

    QRectF plotArea = m_parent->plotArea();


    auto controlRect = m_controlItem->mapToScene( m_controlItem->boundingRect() ).boundingRect();

    if ( m_orientation == Qt::Vertical )
    {
        m_line->setLine( position.x(), plotArea.top(), position.x(), plotArea.bottom() );


        qreal halfPixmapWidth = controlRect.width() / 2.0;

        auto pixmapHeight = controlRect.height();


        m_controlItem->setPos( position.x() - halfPixmapWidth, plotArea.top() - pixmapHeight );


        m_coordInfo->setCoord( m_parent->mapToValue( position ).x() );

        m_coordInfo->setPos( position.x() + m_coordInfo->textWidth(), plotArea.bottom() );


        loadPoints( position );
    }
    else
    {
        m_line->setLine( plotArea.left(), position.y(), plotArea.right(), position.y() );

        qreal halfPixmapWidth = controlRect.width() / 2.0;

        m_controlItem->setPos( plotArea.right() + controlRect.width(),
                              position.y() - halfPixmapWidth           );


        m_coordInfo->setCoord( m_parent->mapToValue( position ).y() );

        auto valueAxis = (QValueAxis *) m_parent->axes( Qt::Horizontal ).first();

        auto startX = m_parent->mapToPosition( { valueAxis->min(), 0 } );

        m_coordInfo->setPos( startX.x() - m_coordInfo->boundingRect().width(), position.y() );
    }

    emit onPositionChanged( position );
}


void QPlotMarker::moveBegin()
{
    if ( m_orientation == Qt::Vertical )

        move( { m_parent->plotArea().x(), m_parent->plotArea().y() } );

    else

        move( { m_parent->plotArea().right(), m_parent->plotArea().topRight().y() } );
}

void QPlotMarker::moveEnd()
{
    if ( m_orientation == Qt::Vertical )

        move( { m_parent->plotArea().x(), m_parent->plotArea().y() } );

    else

        move( { m_parent->plotArea().right(), m_parent->plotArea().topRight().y() } );
}

bool QPlotMarker::hasFocus() const
{
    return m_controlItem->hasFocus();
}

qreal QPlotMarker::markerValue() const
{
    return m_coordInfo->coord();
}

QRectF QPlotMarker::boundingRect() const
{
    return
    {
        m_parent->plotArea().left(),
        x(),
        m_line->boundingRect().width() + m_controlItem->boundingRect().width(),
        m_controlItem->boundingRect().height()
    };
}

void QPlotMarker::showCoord()
{
    for ( auto & item : m_items )

        item.coord->setVisible( not item.coord->isVisible() );
}

void QPlotMarker::activate(bool isActivated)
{
    auto activeItem = dynamic_cast<QGraphicsTextItem *>( sender() );

    for ( auto item : m_items )
    {
        if ( isActivated )
        {
            auto effect = new QGraphicsOpacityEffect();

            effect->setOpacity( 0.2 );

            item.coord->setGraphicsEffect( effect );
        }
        else
        {
            item.coord->setGraphicsEffect( nullptr );
        }

    }
}

void QPlotMarker::setIntersectionPointSize(quint8 size)
{
    m_intersectionPointSize = size;
}

bool QPlotMarker::isPositionAccept(const QPointF & position)
{
    QRectF plotArea = m_parent->plotArea();

    if ( m_orientation == Qt::Vertical )

        return position.x() >= plotArea.left() and position.x() <= plotArea.right();

    else

        return position.y() >= plotArea.top() and position.y() <= plotArea.bottom();
}

void QPlotMarker::change(const QRectF & plotArea)
{
    Q_UNUSED(plotArea)

    moveBegin();
}

qreal distance( const QPointF & pointOne, const QPointF & pointTwo )
{
    return std::sqrt( std::pow( pointTwo.x() - pointOne.x(), 2 ) +
                      std::pow( pointTwo.y() - pointTwo.y(), 2 )   );
}

QPair<QPointF, QPointF> findTwoNearestPoints( const QPointF & targetPoint, QLineSeries * lineSeries )
{
    qreal minDistanceOne = std::numeric_limits<qreal>::max();

    qreal minDistanceTwo = std::numeric_limits<qreal>::max();


    QPointF closestPointOne, closestPointTwo;

    for ( const QPointF & point : lineSeries->points() )
    {
        qreal dist = distance( point, targetPoint );

        if ( dist < minDistanceOne )
        {
            minDistanceTwo = minDistanceOne;

            closestPointTwo = closestPointOne;

            minDistanceOne = dist;

            closestPointOne = point;
        }

        else if ( dist < minDistanceTwo )
        {
            minDistanceTwo = dist;

            closestPointTwo = point;
        }
    }

    if ( minDistanceOne != std::numeric_limits<qreal>::max() and
         minDistanceTwo != std::numeric_limits<qreal>::max()     )

        return qMakePair( closestPointOne, closestPointTwo );

    return {};
}

namespace std
{
template <> struct hash<QPointF>
{
    size_t operator()(const QPointF &key, size_t seed) const
    {
        return qHashMulti( seed, key.x(), key.y() );
    }
};
}

const quint8 RADIUS = 3;

void QPlotMarker::loadPoints(const QPointF & position)
{
    if ( m_items.size() > 0 )

        m_items.clear();


    QSet<QPointF> points;

    auto markerLineOld = m_line->line();

    QLineF markerLine( m_parent->mapToValue( markerLineOld.p1() ),
                      m_parent->mapToValue( markerLineOld.p2() )   );

    for ( auto series : m_parent->series() )
    {
        if ( not series->isVisible() ) continue;

        auto lineSeries = dynamic_cast<QLineSeries *>( series );

        auto twoPoint = findTwoNearestPoints( m_parent->mapToValue( position, series ), lineSeries );

        QLineF segment( twoPoint.first, twoPoint.second );

        QPointF intersectPoint;

        auto intersectType = markerLine.intersects( segment, &intersectPoint );

        if ( intersectType == QLineF::BoundedIntersection )

        points.insert( { intersectPoint.x(),
                       QString::number( intersectPoint.y(), 'g', 3 ).toDouble() } );
    }

    for ( auto & point : points )
    {
        auto viewPoint( m_parent->mapToPosition( point ) );

        auto item = new QGraphicsEllipseItem( viewPoint.x() - RADIUS,
                                              viewPoint.y() - RADIUS,
                                              2 * RADIUS,
                                              2 * RADIUS              );

        item->setPen( QPen( m_markerColor, m_intersectionPointSize ) );

        item->setBrush( m_markerColor );

        m_parent->scene()->addItem( item );


        auto textItem = new GraphicsCoordItem();

        textItem->setItemColor( m_markerColor );

        textItem->setCoord( point.y() );

        textItem->setVisible( false );

        textItem->setPos( viewPoint );

        connect( textItem, &GraphicsCoordItem::onActivated, this, &QPlotMarker::activate );

        m_parent->scene()->addItem( textItem );


        m_items.append( { .coord=textItem, .point=item } );

    }
}
