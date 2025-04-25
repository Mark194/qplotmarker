#include "../include/QPlotMarker/qplotmarker.hpp"


#include "qplot_marker_p.hpp"

#include <entity/graphics_coord_item.hpp>
#include <entity/movable_button.hpp>

#include <QGraphicsOpacityEffect>
#include <QGraphicsScene>
#include <QValueAxis>

#include <QDir>
#include <qsvgrenderer.h>

#include <utility/plot_geometry_utils.hpp>


QPlotMarker::QPlotMarker(QChart * parent, const QColor & color, Qt::Orientation orientation)
    : QGraphicsWidget(),
      d_ptr( new QPlotMarkerPrivate( this ) )
{
    Q_INIT_RESOURCE(marker_icons);

    Q_D(QPlotMarker);

    d->init( parent, color, orientation );
}

QPlotMarker::~QPlotMarker()
{

}

void QPlotMarker::setColor(const QColor & color)
{
    Q_D(QPlotMarker);

    d->m_markerColor = color;


    auto pen = d->m_line->pen();

    pen.setColor( color );

    d->m_line->setPen( pen );


    d->m_controlItem->setColor( color );

    emit colorChanged( color );
}

QColor QPlotMarker::color() const
{
    Q_D(const QPlotMarker);

    return d->m_markerColor;
}

void QPlotMarker::setMovementStyle(MovementStyle style)
{
    Q_D(QPlotMarker);

    d->m_movement = style;

    move( QGraphicsItem::pos() );
}

QPlotMarker::MovementStyle QPlotMarker::movementStyle() const
{
    Q_D(const QPlotMarker);

    return d->m_movement;
}

Qt::Orientation QPlotMarker::orientation() const
{
    Q_D(const QPlotMarker);

    return d->m_orientation;
}

QChart * QPlotMarker::chart() const
{
    Q_D(const QPlotMarker);

    return d->m_parentChart;
}

void QPlotMarker::setSelected(bool isSelect)
{
    Q_D(QPlotMarker);

    if ( isSelect )
    {
        d->m_controlItem->setButtonIcon( ":/marker_selected_icon" );

        d->m_controlItem->setButtonControl( ":/marker_selected_eye" );
    }
    else
    {
        d->m_controlItem->setButtonIcon( ":/marker_icon" );

        d->m_controlItem->setButtonControl( ":/marker_eye" );
    }


    if ( d->m_orientation == Qt::Horizontal )

        d->m_controlItem->setRotation( 90 );


    d->m_controlItem->setSize( 25.0 );

    QGraphicsItem::setSelected( isSelect );
}

void QPlotMarker::move(const QPointF & position)
{
    Q_D(QPlotMarker);

    if ( not d->isPositionAcceptable( position ) ) return;


    d->m_markerPosition = position;


    QRectF plotArea = d->m_parentChart->plotArea();

    auto controlRect = d->m_controlItem->mapToScene( d->m_controlItem->boundingRect() ).boundingRect();

    if ( d->m_orientation == Qt::Vertical )
    {
        d->m_line->setLine( position.x(), plotArea.top(), position.x(), plotArea.bottom() );


        qreal halfPixmapWidth = controlRect.width() / 2.0;

        auto pixmapHeight = controlRect.height();


        d->m_controlItem->setPos( position.x() - halfPixmapWidth, plotArea.top() - pixmapHeight );


        d->m_coordInfo->setCoord( d->m_parentChart->mapToValue( position ).x() );

        d->m_coordInfo->setPos( position.x() - d->m_coordInfo->boundingRect().width() / 2,
                                plotArea.bottom()                                         );


        d->loadIntersectionPoints( position );
    }
    else
    {
        d->m_line->setLine( plotArea.left(), position.y(), plotArea.right(), position.y() );

        qreal halfPixmapWidth = controlRect.width() / 2.0;

        d->m_controlItem->setPos( plotArea.right() + controlRect.width(),
                                  position.y() - halfPixmapWidth           );


        d->m_coordInfo->setCoord( d->m_parentChart->mapToValue( position ).y() );

        auto valueAxis = (QValueAxis *) d->m_parentChart->axes( Qt::Horizontal ).first();

        auto startX = d->m_parentChart->mapToPosition( { valueAxis->min(), 0 } );

        d->m_coordInfo->setPos( startX.x() - d->m_coordInfo->boundingRect().width(),
                                position.y() - d->m_coordInfo->boundingRect().height() / 2 );
    }

    emit onPositionChanged( position );
}


void QPlotMarker::moveBegin()
{
    Q_D(QPlotMarker);

    if ( d->m_orientation == Qt::Vertical )

        move( { d->m_parentChart->plotArea().x(),
                d->m_parentChart->plotArea().y() } );

    else

        move( { d->m_parentChart->plotArea().right(),
                d->m_parentChart->plotArea().topRight().y() } );
}

void QPlotMarker::moveEnd()
{
    Q_D(QPlotMarker);

    if ( d->m_orientation == Qt::Vertical )

        move( { d->m_parentChart->plotArea().topRight().x(),
                d->m_parentChart->plotArea().y() } );

    else

        move( { d->m_parentChart->plotArea().right(),
                d->m_parentChart->plotArea().bottomRight().y() } );
}

void QPlotMarker::moveToNextPoint()
{
    Q_D(QPlotMarker);

    Q_ASSERT_X(d->m_movement == QPlotMarker::MOVEMENT_BY_POINTS,
               "moveToNextPoint()",
               "The mode of moving by points is not set!");

    d->m_controlItem->move( d->m_markerPosition, false );
}

void QPlotMarker::moveToPreviousPoint()
{
    Q_D(QPlotMarker);

    Q_ASSERT_X(d->m_movement == QPlotMarker::MOVEMENT_BY_POINTS,
               "moveToPreviousPoint()",
               "The mode of moving by points is not set!");

    d->m_controlItem->move( d->m_markerPosition, true );
}

QPointF QPlotMarker::pos() const
{
    Q_D(const QPlotMarker);

    return d->m_markerPosition;
}

bool QPlotMarker::hasFocus() const
{
    Q_D(const QPlotMarker);

    return d->m_controlItem->hasFocus();
}

qreal QPlotMarker::markerValue() const
{
    Q_D(const QPlotMarker);

    return d->m_coordInfo->coord();
}

QRectF QPlotMarker::boundingRect() const
{
    Q_D(const QPlotMarker);

    return
    {
        d->m_parentChart->plotArea().left(),
        x(),
        d->m_line->boundingRect().width() + d->m_controlItem->boundingRect().width(),
        d->m_controlItem->boundingRect().height()
    };
}

void QPlotMarker::showCoordinates()
{
    Q_D(QPlotMarker);

    for ( auto & item : d->m_intersectionItems )

        item.coord->setVisible( not item.coord->isVisible() );
}

void QPlotMarker::activate(bool isActivated)
{
    Q_D(QPlotMarker);

    auto activeItem = dynamic_cast<QGraphicsTextItem *>( sender() );

    for ( auto item : d->m_intersectionItems )
    {
        if ( not item.coord or item.coord == activeItem )

            continue;

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

void QPlotMarker::setIntersectionPointSize(qreal size)
{
    Q_D(QPlotMarker);

    d->m_intersectionPointSize = size;
}

void QPlotMarker::setIntersectionLineSize(quint8 size)
{
    Q_D(QPlotMarker);

    auto pen = d->m_line->pen();

    pen.setWidth( size );

    d->m_line->setPen( pen );
}

quint8 QPlotMarker::intersectionLineSize() const
{
    Q_D(const QPlotMarker);

    return d->m_line->pen().width();
}

void QPlotMarker::setLabelFormat(const QString & format)
{
    Q_D(QPlotMarker);

    d->m_coordInfo->setLabelFormat( format );

    for ( auto & item : d->m_intersectionItems )

        item.coord->setLabelFormat( format );
}

void QPlotMarker::addIgnoreSeries(const QList<QAbstractSeries *> & series)
{
    Q_D(QPlotMarker);

    d->m_ignoreSeries = series;
}

QList<QAbstractSeries *> QPlotMarker::ignoreSeries() const
{
    Q_D(const QPlotMarker);

    return d->m_ignoreSeries;
}
