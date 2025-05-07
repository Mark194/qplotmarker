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

    if ( d->m_isSelectedLock ) return;


    if ( isSelect )
    {
        d->m_controlItem->setButtonIcon( d->m_buttonSelectedIcon );

        d->m_controlItem->setButtonControl( d->m_buttonsSelectedControl );
    }
    else
    {
        d->m_controlItem->setButtonIcon( d->m_buttonIcon );

        d->m_controlItem->setButtonControl( d->m_buttonControl );
    }


    if ( d->m_orientation == Qt::Horizontal )

        d->m_controlItem->setRotation( 90 );


    QGraphicsItem::setSelected( isSelect );
}

bool QPlotMarker::isIgnoreSelected() const
{
    Q_D(const QPlotMarker);

    return d->m_isSelectedLock;
}

void QPlotMarker::setIsIgnoreSelected(bool isIgnore)
{
    Q_D(QPlotMarker);

    d->m_isSelectedLock = isIgnore;
}

void QPlotMarker::move(const QPointF & position)
{
    Q_D(QPlotMarker);

    d->moveMarkerToPosition( position );
}

void QPlotMarker::move(qreal percent)
{
    Q_D(QPlotMarker);

    Q_ASSERT_X(percent <= 1.0 and percent >= -1.0,
               "move(qreal percent)",
               "Incorrect percentage value [-1.0:1.0]!");

    auto plotArea = d->m_parentChart->plotArea();

    auto position = d->m_markerPosition;

    if ( d->m_orientation == Qt::Vertical )
    {
        auto step = plotArea.width() * percent;

        position.setX( position.x() + step );
    }
    else
    {
        auto step = plotArea.height() * percent;

        position.setY( position.y() + step );
    }

    move( position );
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

void QPlotMarker::showCoordinates(bool isVisible)
{
    Q_D(QPlotMarker);

    d->m_isVisibleCoords = isVisible;

    for ( auto & item : d->m_intersectionItems )

        item.coord->setVisible( isVisible );
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

void QPlotMarker::addIgnoreSeries(QAbstractSeries * series)
{
    Q_D(QPlotMarker);

    if ( d->m_ignoreSeries.contains( series ) ) return;

    d->m_ignoreSeries.append( series );
}

void QPlotMarker::removeIgnoreSeries(QAbstractSeries * series)
{
    Q_D(QPlotMarker);

    d->m_ignoreSeries.removeOne( series );
}

void QPlotMarker::setIgnoreSeries(const QList<QAbstractSeries *> & series)
{
    Q_D(QPlotMarker);

    d->m_ignoreSeries = series;
}

QList<QAbstractSeries *> QPlotMarker::ignoreSeries() const
{
    Q_D(const QPlotMarker);

    return d->m_ignoreSeries;
}

qreal QPlotMarker::controlIconSize() const
{
    Q_D(const QPlotMarker);

    return d->m_controlItem->size();
}

void QPlotMarker::setControlIconSize(qreal size)
{
    Q_D(QPlotMarker);

    d->m_controlItem->setSize( size );
}

void QPlotMarker::setCoordFont(const QFont & font)
{
    Q_D(QPlotMarker);

    d->m_coordInfo->setFont( font );

    for ( auto item : d->m_intersectionItems )

        item.coord->setFont( font );
}

void QPlotMarker::setCoordPen(const QPen & pen)
{
    Q_D(QPlotMarker);

    d->m_coordInfo->setPen( pen );

    for ( auto item : d->m_intersectionItems )

        item.coord->setPen( pen );
}

QString QPlotMarker::markerIcon(MarkerButtonIcon typeIcon) const
{
    Q_D(const QPlotMarker);

    switch ( typeIcon )
    {
        case QPlotMarker::MARKER_BUTTON_ICON: return d->m_buttonIcon;

        case QPlotMarker::MARKER_BUTTON_CONTROL: return d->m_buttonControl;

        case QPlotMarker::MARKER_BUTTON_SELECTED_ICON: return d->m_buttonSelectedIcon;

        case QPlotMarker::MARKER_BUTTON_SELECTED_CONTROL: return d->m_buttonsSelectedControl;

        default: return {};
    }
}

void QPlotMarker::setMarkerIcon(const QString & fileName, MarkerButtonIcon typeIcon)
{
    Q_D(QPlotMarker);

    switch ( typeIcon )
    {
        case QPlotMarker::MARKER_BUTTON_ICON:               d->m_buttonIcon = fileName;

        case QPlotMarker::MARKER_BUTTON_CONTROL:            d->m_buttonControl = fileName;

        case QPlotMarker::MARKER_BUTTON_SELECTED_ICON:      d->m_buttonSelectedIcon = fileName;

        case QPlotMarker::MARKER_BUTTON_SELECTED_CONTROL:   d->m_buttonsSelectedControl = fileName;

        default: return;
    }

    update();
}

QRectF QPlotMarker::controlRect() const
{
    Q_D(const QPlotMarker);

    return d->m_controlItem->boundingRect();
}

void QPlotMarker::update()
{
    Q_D(QPlotMarker);

    auto plotArea = d->m_parentChart->plotArea();

    qreal x = qBound( plotArea.left(), d->m_markerPosition.x(), plotArea.right()  );
    qreal y = qBound( plotArea.top(),  d->m_markerPosition.y(), plotArea.bottom() );

    QPointF targetPoint( x, y );

    if ( d->m_movement == QPlotMarker::MOVEMENT_DEFAULT )

        move( targetPoint );

    else

        d->updateOnMoveByPoints( targetPoint );
}

QVariant QPlotMarker::itemChange(GraphicsItemChange change, const QVariant & value)
{
    auto result = QGraphicsWidget::itemChange( change, value );

    if ( change == QGraphicsItem::ItemSceneHasChanged and scene() )

        moveBegin();


    return result;
}
