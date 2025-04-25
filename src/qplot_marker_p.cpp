#include "qplot_marker_p.hpp"


#include <QGraphicsScene>


#include "entity/movable_button.hpp"

#include "entity/graphics_coord_item.hpp"

#include "utility/plot_geometry_utils.hpp"


QPlotMarkerPrivate::QPlotMarkerPrivate(QPlotMarker * q)
    : q_ptr( q ),
      m_parentChart(nullptr),
      m_markerColor(Qt::black),
      m_movement(QPlotMarker::MOVEMENT_DEFAULT),
      m_orientation(Qt::Horizontal),
      m_controlItem(nullptr),
      m_line(nullptr),
      m_coordInfo(nullptr),
      m_intersectionPointSize(3),
      m_intersectionLineSize(2)
{}

QPlotMarkerPrivate::~QPlotMarkerPrivate()
{

}

void QPlotMarkerPrivate::init(QChart * parent, const QColor & color, Qt::Orientation orientation)
{
    q_ptr->setFlag( QGraphicsItem::ItemIsSelectable );

    m_parentChart = parent;
    m_markerColor = color;
    m_orientation = orientation;


    m_line = new QGraphicsLineItem( q_ptr );

    m_line->setPen( QPen( color, 2, Qt::DotLine, Qt::RoundCap  ) );


    m_controlItem = new MovableButton( q_ptr );

    m_controlItem->setColor( m_markerColor );


    m_coordInfo = new GraphicsCoordItem( q_ptr );

    m_coordInfo->setItemColor( m_markerColor );


    q_ptr->setSelected( false );

    QObject::connect( m_parentChart, &QChart::plotAreaChanged,
                      q_ptr, [this](const QRectF & plotArea) {
                          this->handlePositionChange( plotArea );
                      } );
}

bool QPlotMarkerPrivate::isPositionAcceptable(const QPointF & position) const
{
    QRectF plotArea = m_parentChart->plotArea();

    if ( m_orientation == Qt::Vertical )

        return position.x() >= plotArea.left() and position.x() <= plotArea.right();

    else

        return position.y() >= plotArea.top() and position.y() <= plotArea.bottom();
}

void QPlotMarkerPrivate::handlePositionChange(const QRectF & plotArea)
{
    q_ptr->moveBegin();
}


#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

inline uint qHash(const QPointF &point, uint seed = 0) noexcept {
    QtPrivate::QHashCombine hash;
    seed = hash(seed, point.x());
    seed = hash(seed, point.y());
    return seed;
}

#else
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
#endif


void QPlotMarkerPrivate::loadIntersectionPoints(const QPointF & position)
{
    if ( m_intersectionItems.size() > 0 )

        clearInterSectionPoints();


    QSet<QPointF> points;

    auto markerLineOld = m_line->line();

    QLineF markerLine( m_parentChart->mapToValue( markerLineOld.p1() ),
                       m_parentChart->mapToValue( markerLineOld.p2() )   );

    for ( auto series : m_parentChart->series() )
    {
        if ( not series->isVisible() ) continue;

        auto lineSeries = dynamic_cast<QLineSeries *>( series );


        auto point = m_parentChart->mapToValue( position, series );

        auto twoPoint = PlotGeometryUtils::findTwoNearestPoints( point, lineSeries );

        QLineF segment( twoPoint.first, twoPoint.second );

        QPointF intersectPoint;

        auto intersectType = markerLine.intersects( segment, &intersectPoint );

        if ( intersectType == QLineF::BoundedIntersection )

        points.insert( { intersectPoint.x(),
                       QString::number( intersectPoint.y(), 'g', 3 ).toDouble() } );
    }

    for ( auto & point : points )
    {
        auto viewPoint( m_parentChart->mapToPosition( point ) );

        auto item = new QGraphicsEllipseItem( viewPoint.x() - m_intersectionPointSize,
                                              viewPoint.y() - m_intersectionPointSize,
                                              2 * m_intersectionPointSize,
                                              2 * m_intersectionPointSize              );

        item->setPen( QPen( m_markerColor, 2 ) );

        item->setBrush( m_markerColor );

        m_parentChart->scene()->addItem( item );


        auto textItem = new GraphicsCoordItem();

        textItem->setItemColor( m_markerColor );

        textItem->setCoord( point.y() );

        textItem->setVisible( false );

        textItem->setPos( viewPoint );

        QObject::connect( textItem, &GraphicsCoordItem::onActivated, q_ptr, &QPlotMarker::activate );

        m_parentChart->scene()->addItem( textItem );


        m_intersectionItems.append( { .coord=textItem, .point=item } );

    }
}

void QPlotMarkerPrivate::clearInterSectionPoints()
{
    for ( auto item : m_intersectionItems )
    {
        if ( item.coord ) delete item.coord;

        if ( item.point ) delete item.point;
    }

    m_intersectionItems.clear();
}
