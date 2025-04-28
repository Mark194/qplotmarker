#include "graphics_coord_item.hpp"


#include <QApplication>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QStyle>
#include <QStyleFactory>


GraphicsCoordItem::GraphicsCoordItem(QGraphicsItem * parent)
    : QGraphicsTextItem( parent )
{
    QPalette vistaPalette = QApplication::palette();

    m_coordPen = vistaPalette.color( QPalette::WindowText );
}

void GraphicsCoordItem::setCoord(qreal value)
{
    m_coord = value;

    if ( m_labelFormat.isEmpty() )

        setPlainText( QString::number( value ) );
    else

        setPlainText( QString::asprintf( m_labelFormat.toUtf8().constData(),
                                         value)                              );


    update();
}

QPen GraphicsCoordItem::pen() const
{
    return m_coordPen;
}

void GraphicsCoordItem::setPen(const QPen & pen)
{
    m_coordPen = pen;
}

qreal GraphicsCoordItem::coord() const
{
    return m_coord;
}

void GraphicsCoordItem::setItemColor(const QColor & itemColor)
{
    m_itemColor = itemColor;

    update();
}

void GraphicsCoordItem::setLabelFormat(const QString & format)
{
    m_labelFormat = format;

    setCoord( m_coord );
}

void GraphicsCoordItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option)

    Q_UNUSED(widget)


    QPalette vistaPalette = QApplication::palette();

    painter->setBrush( vistaPalette.color( QPalette::Base ) );

    painter->setPen( QPen( m_itemColor, 2 ) );

    painter->drawRect( boundingRect() );


    painter->setPen( m_coordPen );

    auto rect = boundingRect();

    painter->setFont( font() );

    painter->drawText( rect, Qt::AlignCenter, toPlainText() );
}

void GraphicsCoordItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    Q_UNUSED( event )

    emit onActivated( true );
}

void GraphicsCoordItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    Q_UNUSED( event )

    emit onActivated( false );
}

