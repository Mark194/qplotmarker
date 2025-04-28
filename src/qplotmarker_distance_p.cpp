#include "qplotmarker_distance_p.hpp"


#include <QPointF>


#include <QPlotMarker/qplotmarker_distance.hpp>


#include "entity/graphics_coord_item.hpp"


QPlotMarkerDistancePrivate::QPlotMarkerDistancePrivate(QPlotMarkerDistance * q)
    : q_ptr(q)
{

}

QPlotMarkerDistancePrivate::~QPlotMarkerDistancePrivate()
{

}

void QPlotMarkerDistancePrivate::init(QPlotMarker * one, QPlotMarker * other)
{
    Q_ASSERT_X(one and other,
               "QPlotMarkerDistancePrivate::init()",
               "QPlotMarker is null!");

    Q_ASSERT_X(one->chart() == other->chart(),
               "QPlotMarkerDistancePrivate::init()",
               "QPlotMarker's are on different charts!");

    Q_ASSERT_X(one->orientation() == other->orientation(),
               "QPlotMarkerDistancePrivate::init()",
               "QPlotMarker's have different orientations!");

    m_oneMarker     = one;

    m_otherMarker   = other;

    m_line = new QGraphicsLineItem( q_ptr );

    m_line->setPen( QPen( one->color(), 2, Qt::DotLine, Qt::RoundCap  ) );


    m_coordInfo = new GraphicsCoordItem( q_ptr );

    m_coordInfo->setItemColor( one->color() );


    QObject::connect( m_oneMarker, &QPlotMarker::positionChanged,
                      q_ptr,       &QPlotMarkerDistance::update   );

    QObject::connect( m_otherMarker, &QPlotMarker::positionChanged,
                      q_ptr,         &QPlotMarkerDistance::update   );
}

void QPlotMarkerDistancePrivate::paint()
{
    QPointF pointOne   = m_oneMarker->pos();

    QPointF pointOther = m_otherMarker->pos();


    auto plotArea = m_oneMarker->chart()->plotArea();


    m_line->setLine( pointOne.x(), plotArea.y(), pointOther.x(), plotArea.topRight().y() );


    m_coordInfo->setCoord( q_ptr->markersDistance( m_oneMarker, m_otherMarker ) );

    QPointF centerPoint = (m_line->line().p1() + m_line->line().p2()) / 2.0;


    m_coordInfo->setPos( centerPoint - QPointF(m_coordInfo->boundingRect().width() / 2,
                                               m_coordInfo->boundingRect().height() / 2 ));
}
