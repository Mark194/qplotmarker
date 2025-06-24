#include "../include/QPlotMarker/qplotmarker.hpp"

#include "qplotmarker_p.hpp"

#include <entity/graphics_coord_item.hpp>
#include <entity/movable_button.hpp>

#include <QGraphicsOpacityEffect>
#include <QGraphicsScene>

#include <QDir>
#include <QSvgRenderer>

#include <utility/plot_geometry_utils.hpp>

/*!
    \class QPlotMarker
    \inmodule QPlotMarker
    \brief A customizable marker that can be placed on QChart plots.

    The QPlotMarker class provides interactive markers that can be placed on Qt charts.
    It supports dragging, snapping to data points, and various visual customizations.

    For general library usage, see \l{overview} page.

    \sa QPlotMarkerGroup, QPlotMarkerDistance
*/

/*!
    \enum QPlotMarker::MovementStyle

    \brief Defines how the marker moves on the chart.

    This enumeration specifies different movement modes for the plot marker.

    \value MOVEMENT_DEFAULT
           Marker moves freely within chart bounds (default mode)
    \value MOVEMENT_BY_POINTS
           Marker snaps to actual data points in the series

    \sa setMovementStyle(), movementStyle()
*/

/*!
    \enum QPlotMarker::MarkerButtonIcon
    \brief Types of marker icons.

    \value MARKER_BUTTON_ICON
           Default icon
    \value MARKER_BUTTON_CONTROL
           Control icon
    \value MARKER_BUTTON_SELECTED_ICON
           Icon when selected
    \value MARKER_BUTTON_SELECTED_CONTROL
           Control icon when selected
*/

/*!
    \enum QPlotMarker::MarkerOrientation
    \brief Marker orientation options.

    \value Horizontal
           Standard horizontal orientation
    \value Vertical
           Standard vertical orientation
    \value HorizontalInverted
           Horizontally inverted
    \value VerticalInverted
           Vertically inverted
*/

/*!
    \fn QPlotMarker::QPlotMarker(QChart *parent, const QColor &color, MarkerOrientation orientation)

    \brief Constructs a QPlotMarker with the given parent chart, color and orientation.

    \a parent
    The parent chart to which this marker will be added

    \a color
    The color of the marker

    \a orientation
    The orientation of the marker (horizontal or vertical)
*/
QPlotMarker::QPlotMarker(QChart *parent, const QColor &color, MarkerOrientation orientation)
    : d_ptr(new QPlotMarkerPrivate(this))
{
    Q_INIT_RESOURCE(marker_icons);

    Q_D(QPlotMarker);

    d->init(parent, color, orientation);
}

/*!
    \fn QPlotMarker::QPlotMarker(QChart *parent, const QColor &color, Qt::Orientation)

    \brief Constructs a QPlotMarker with Qt standard orientation.
*/
QPlotMarker::QPlotMarker(QChart *parent, const QColor &color, Qt::Orientation orientation)
    : QPlotMarker(parent, color, static_cast<MarkerOrientation>(orientation))
{}

/*!
    \fn QPlotMarker::~QPlotMarker()

    \brief Destroys the marker.
*/
QPlotMarker::~QPlotMarker() = default;

/*!
    \fn void QPlotMarker::setColor(const QColor &color)

    \brief Sets the marker color.

    \a color
    The new color to apply to the marker. This should be a valid QColor.

    \note This function will emit the \c colorChanged() signal after updating all components.

    \sa color()
*/
void QPlotMarker::setColor(const QColor &color)
{
    Q_D(QPlotMarker);

    d->m_markerColor = color;

    auto pen = d->m_line->pen();

    pen.setColor(color);

    d->m_line->setPen(pen);

    d->m_controlItem->setColor(color);

    emit colorChanged(color);
}

/*!
    \property QPlotMarker::markerColor
    \brief The color of the marker.

    This property controls the color used for all visual components of the marker.
    When changed, all marker elements are updated and the colorChanged() signal is emitted.

    \sa color(), setColor()
*/

/*!
    \fn QPlotMarker::color() const

    \brief Returns the current color of the marker.

    Return The QColor representing the marker's current color

    \sa setColor(), colorChanged()
*/
QColor QPlotMarker::color() const
{
    Q_D(const QPlotMarker);

    return d->m_markerColor;
}

/*!
    \fn QPlotMarker::setMovementStyle(MovementStyle style)

    \brief Sets the movement style for the marker.

    \a style
    The movement behavior (see also QPlotMarker::MovementStyle)

    \sa movementStyle()
*/
void QPlotMarker::setMovementStyle(MovementStyle style)
{
    Q_D(QPlotMarker);

    d->m_movement = style;

    update();
}

/*!
    \fn QPlotMarker::MovementStyle QPlotMarker::movementStyle() const

    \brief Returns the current movement style.

    \sa setMovementStyle()
*/
QPlotMarker::MovementStyle QPlotMarker::movementStyle() const
{
    Q_D(const QPlotMarker);

    return d->m_movement;
}

/*!
    \property QPlotMarker::movement
    \brief The movement style of the marker.

    This property determines how the marker can be moved:
    \list
        \li MOVEMENT_DEFAULT
        \li MOVEMENT_BY_POINTS
    \endlist

    \sa movementStyle(), setMovementStyle()
*/

/*!
    \fn Qt::Orientation QPlotMarker::orientation() const
    \brief Returns the orientation of the plot marker.

    This function returns whether the marker is horizontally or vertically oriented.
    The orientation of the marker as \c Qt::Horizontal or \c Qt::Vertical

    \sa markerOrientation()
*/
Qt::Orientation QPlotMarker::orientation() const
{
    Q_D(const QPlotMarker);

    return d->orientation();
}

/*!
    \fn QPlotMarker::MarkerOrientation QPlotMarker::markerOrientation() const
    \brief Returns the extended orientation including inversion state.

    Full orientation information

    \sa orientation()
*/
QPlotMarker::MarkerOrientation QPlotMarker::markerOrientation() const
{
    Q_D(const QPlotMarker);

    return d->m_orientation;
}

/*!
    \fn bool QPlotMarker::isInverted() const
    \brief Checks if the marker coordinates are inverted.

    Return true if coordinates are inverted
*/
bool QPlotMarker::isInverted() const
{
    Q_D(const QPlotMarker);

    return d->isInverted();
}

/*!
    \fn QChart * QPlotMarker::chart() const
    \brief Returns pointer to parent QChart
*/
QChart *QPlotMarker::chart() const
{
    Q_D(const QPlotMarker);

    return d->m_parentChart;
}

/*!
    \fn void QPlotMarker::setSelected(bool isSelect)
    \brief Sets the selection state of the marker.

    \a isSelect
    True to select, false to deselect

    \sa isIgnoreSelected()
*/
void QPlotMarker::setSelected(bool isSelect)
{
    Q_D(QPlotMarker);

    if (d->m_isSelectedLock)
        return;

    if (isSelect) {
        d->m_controlItem->setButtonIcon(d->m_buttonSelectedIcon);

        d->m_controlItem->setButtonControl(d->m_buttonsSelectedControl);
    } else {
        d->m_controlItem->setButtonIcon(d->m_buttonIcon);

        d->m_controlItem->setButtonControl(d->m_buttonControl);
    }

    QGraphicsItem::setSelected(isSelect);
}

/*!
    \fn bool QPlotMarker::isIgnoreSelected() const
    \brief Checks if selection is ignored for this marker.

    Return true if selection is disabled

    \sa setIsIgnoreSelected()
*/
bool QPlotMarker::isIgnoreSelected() const
{
    Q_D(const QPlotMarker);

    return d->m_isSelectedLock;
}

/*!
    \fn void QPlotMarker::setIsIgnoreSelected(bool isIgnore)
    \brief Sets whether selection should be ignored.

    \a isIgnore
    If true, selected markers will be excluded from calculations

    \sa isIgnoreSelected()
*/
void QPlotMarker::setIsIgnoreSelected(bool isIgnore)
{
    Q_D(QPlotMarker);

    d->m_isSelectedLock = isIgnore;
}

/*!
    \fn void QPlotMarker::move(const QPointF &position)
    \brief Moves the marker to the specified position.

    \a position
    The new position in chart coordinates
*/
void QPlotMarker::move(const QPointF &position)
{
    Q_D(QPlotMarker);

    d->moveMarkerToPosition(position);
}

/*!
    \fn void QPlotMarker::move(qreal percent)
    \brief Moves the marker by percentage of plot area.

    \a percent
    Value between -1.0 and 1.0 representing percentage
*/
void QPlotMarker::move(qreal percent)
{
    Q_D(QPlotMarker);

    Q_ASSERT_X(
        percent <= 1.0 and percent >= -1.0,
        "move(qreal percent)",
        "Incorrect percentage value [-1.0:1.0]!");

    auto plotArea = d->m_parentChart->plotArea();

    auto position = d->m_markerPosition;

    if (d->orientation() == Qt::Vertical) {
        auto step = plotArea.width() * percent;

        position.setX(position.x() + step);
    } else {
        auto step = plotArea.height() * percent;

        position.setY(position.y() + step);
    }

    move(position);
}

/*!
    \fn void QPlotMarker::moveBegin()
    \brief Moves the marker to the beginning of the plot area.
*/
void QPlotMarker::moveBegin()
{
    Q_D(QPlotMarker);

    auto plotArea = d->m_parentChart->plotArea();
    if (d->orientation() == Qt::Vertical)

        move({plotArea.x(), plotArea.y()});

    else

        move({plotArea.right(), plotArea.topRight().y()});
}

/*!
    \fn QPlotMarker::moveEnd()
    \brief Moves the marker to the end of the plot area.
*/
void QPlotMarker::moveEnd()
{
    Q_D(QPlotMarker);

    auto plotArea = d->m_parentChart->plotArea();
    if (d->orientation() == Qt::Vertical)

        move({plotArea.topRight().x(), plotArea.y()});

    else

        move({plotArea.right(), plotArea.bottomRight().y()});
}

/*!
    \fn QPlotMarker::moveToNextPoint()
    \brief Moves to next data point (only in MOVEMENT_BY_POINTS mode).
    \sa setMovementStyle()
*/
void QPlotMarker::moveToNextPoint()
{
    Q_D(QPlotMarker);

    Q_ASSERT_X(
        d->m_movement == QPlotMarker::MOVEMENT_BY_POINTS,
        "moveToNextPoint()",
        "The mode of moving by points is not set!");

    d->m_controlItem->move(d->m_markerPosition, false);
}

/*!
    \fn QPlotMarker::moveToPreviousPoint()
    \brief Moves to previous data point (only in MOVEMENT_BY_POINTS mode).
    \sa setMovementStyle()
*/
void QPlotMarker::moveToPreviousPoint()
{
    Q_D(QPlotMarker);

    Q_ASSERT_X(
        d->m_movement == QPlotMarker::MOVEMENT_BY_POINTS,
        "moveToPreviousPoint()",
        "The mode of moving by points is not set!");

    d->m_controlItem->move(d->m_markerPosition, true);
}

/*!
    \fn QPointF QPlotMarker::pos() const
    \brief Returns the current marker position.

    Current position in chart coordinates
*/
QPointF QPlotMarker::pos() const
{
    Q_D(const QPlotMarker);

    return d->m_markerPosition;
}

/*!
    \fn bool QPlotMarker::hasFocus() const
    \brief Checks if the marker has keyboard focus.

    Return \c true if the marker currently has keyboard focus, \c false otherwise
*/
bool QPlotMarker::hasFocus() const
{
    Q_D(const QPlotMarker);

    return d->m_controlItem->hasFocus();
}

/*!
    \fn qreal QPlotMarker::markerValue() const
    \brief Returns the current marker value.

    Current value on the primary axis
*/
qreal QPlotMarker::markerValue() const
{
    Q_D(const QPlotMarker);

    return d->m_coordInfo->coord();
}

/*!
    \fn QRectF QPlotMarker::boundingRect() const
    \brief Returns the bounding rectangle of the marker.

    Bounding rectangle in scene coordinates
*/
QRectF QPlotMarker::boundingRect() const
{
    Q_D(const QPlotMarker);

    return {
        d->m_parentChart->plotArea().left(),
        x(),
        d->m_line->boundingRect().width() + d->m_controlItem->boundingRect().width(),
        d->m_controlItem->boundingRect().height()};
}

/*!
    \fn void QPlotMarker::showCoordinates(bool isVisible)
    \brief Shows or hides coordinate labels.
    \ingroup slots

    \a isVisible
    true to show coordinates
*/
void QPlotMarker::showCoordinates(bool isVisible)
{
    Q_D(QPlotMarker);

    d->m_isVisibleCoords = isVisible;

    for (auto &item : d->m_intersectionItems)

        item.coord->setVisible(isVisible);
}

/*!
    \fn void QPlotMarker::activate(bool isActivated)
    \brief Activates or deactivates the marker appearance.

    \a isActivated
    true for active state
*/
void QPlotMarker::activate(bool isActivated)
{
    Q_D(QPlotMarker);

    auto activeItem = dynamic_cast<QGraphicsTextItem *>(sender());

    for (auto item : d->m_intersectionItems) {
        if (not item.coord or item.coord == activeItem)

            continue;

        if (isActivated) {
            auto effect = new QGraphicsOpacityEffect();

            effect->setOpacity(0.2);

            item.coord->setGraphicsEffect(effect);
        } else {
            item.coord->setGraphicsEffect(nullptr);
        }
    }
}

/*!
    \fn void QPlotMarker::setIntersectionPointSize(qreal size)
    \brief Sets the size of intersection points.

    \a size
    New size in pixels
*/
void QPlotMarker::setIntersectionPointSize(qreal size)
{
    Q_D(QPlotMarker);

    d->m_intersectionPointSize = size;
}

/*!
    \fn void QPlotMarker::setIntersectionLineSize(quint8 size)
    \brief Sets the line width for intersection lines.

    \a size
    New width in pixels

    \sa intersectionLineSize()
*/
void QPlotMarker::setIntersectionLineSize(quint8 size)
{
    Q_D(QPlotMarker);

    auto pen = d->m_line->pen();

    pen.setWidth(size);

    d->m_line->setPen(pen);
}

/*!
    \fn quint8 QPlotMarker::intersectionLineSize() const
    \brief Returns the current intersection line width.

    Return current width in pixels

    \sa setIntersectionLineSize()
*/
quint8 QPlotMarker::intersectionLineSize() const
{
    Q_D(const QPlotMarker);

    return d->m_line->pen().width();
}

/*!
    \fn void QPlotMarker::setLabelFormat(const QString &format)
    \brief Sets the format string for coordinate labels.

    \a format
    A printf-style format string (e.g. "%.2f, %.2f")
*/
void QPlotMarker::setLabelFormat(const QString &format)
{
    Q_D(QPlotMarker);

    d->m_coordInfo->setLabelFormat(format);

    for (auto &item : d->m_intersectionItems)

        item.coord->setLabelFormat(format);
}

/*!
    \fn void QPlotMarker::addIgnoreSeries(QAbstractSeries *series)
    \brief Adds a series to ignore during intersection calculations.
    \a series
    The QAbstractSeries-derived object (e.g., QLineSeries, QScatterSeries)
    that should be ignored by this marker. If the series is already in the
    ignore list, this function does nothing.
    \sa removeIgnoreSeries()
*/
void QPlotMarker::addIgnoreSeries(QAbstractSeries *series)
{
    Q_D(QPlotMarker);

    if (d->m_ignoreSeries.contains(series))
        return;

    d->m_ignoreSeries.append(series);
}

/*!
    \fn void QPlotMarker::removeIgnoreSeries(QAbstractSeries *series)
    \brief Removes a series from ignore list.
    \a series
    The series to remove from the ignore list. If the series is not
    currently in the ignore list, this function has no effect.
    \sa addIgnoreSeries()
*/
void QPlotMarker::removeIgnoreSeries(QAbstractSeries *series)
{
    Q_D(QPlotMarker);

    d->m_ignoreSeries.removeOne(series);
}

/*!
    \fn void QPlotMarker::setIgnoreSeries(const QList<QAbstractSeries *> &series)
    \brief Sets the complete list of series to ignore.

    \a series
    A list of QAbstractSeries pointers representing all series that should be ignored.
    The list can be empty to clear all ignores. The marker does not take ownership
    of the series objects.

    \sa ignoreSeries()
*/
void QPlotMarker::setIgnoreSeries(const QList<QAbstractSeries *> &series)
{
    Q_D(QPlotMarker);

    d->m_ignoreSeries = series;
}

/*!
    \fn QList<QAbstractSeries *> QPlotMarker::ignoreSeries() const
    \brief Returns the list of ignored series.

    \sa setIgnoreSeries()
*/
QList<QAbstractSeries *> QPlotMarker::ignoreSeries() const
{
    Q_D(const QPlotMarker);

    return d->m_ignoreSeries;
}

/*!
    \fn qreal QPlotMarker::controlIconSize() const
    \brief Returns the current control icon size.

    \sa setControlIconSize()
*/
qreal QPlotMarker::controlIconSize() const
{
    Q_D(const QPlotMarker);

    return d->m_controlItem->size();
}

/*!
    \fn void QPlotMarker::setControlIconSize(qreal size)
    \brief Sets the control icon size.
    \a size
    The new size for control icons in pixels. Must be a positive value.
    \sa controlIconSize()
*/
void QPlotMarker::setControlIconSize(qreal size)
{
    Q_D(QPlotMarker);

    d->m_controlItem->setSize(size);
}

/*!
    \fn void QPlotMarker::setCoordFont(const QFont &font)
    \brief Sets the font for coordinate labels.
    \a font
    The QFont to use for coordinate text display. The font will be applied
    immediately to all existing coordinate labels.
*/
void QPlotMarker::setCoordFont(const QFont &font)
{
    Q_D(QPlotMarker);

    d->m_coordInfo->setFont(font);

    for (auto item : d->m_intersectionItems)

        item.coord->setFont(font);
}

/*!
    \fn void QPlotMarker::setCoordPen(const QPen &pen)
    \brief Sets the pen for coordinate labels.
    \a pen
    The QPen to use for coordinate styling. This controls:
        \list
            \li Text outline (if enabled)
            \li Border styling (if applicable)
            \li General stroke properties for coordinate element
        \endlist
*/
void QPlotMarker::setCoordPen(const QPen &pen)
{
    Q_D(QPlotMarker);

    d->m_coordInfo->setPen(pen);

    for (auto item : d->m_intersectionItems)

        item.coord->setPen(pen);
}

/*!
    \fn QString QPlotMarker::markerIcon(MarkerButtonIcon typeIcon) const
    \brief Returns the specified marker icon.
    \a typeIcon
    The type of marker icon to retrieve.
    \sa setMarkerIcon()
*/
QString QPlotMarker::markerIcon(MarkerButtonIcon typeIcon) const
{
    Q_D(const QPlotMarker);

    switch (typeIcon) {
    case MARKER_BUTTON_ICON:
        return d->m_buttonIcon;

    case MARKER_BUTTON_CONTROL:
        return d->m_buttonControl;

    case MARKER_BUTTON_SELECTED_ICON:
        return d->m_buttonSelectedIcon;

    case MARKER_BUTTON_SELECTED_CONTROL:
        return d->m_buttonsSelectedControl;

    default:
        return {};
    }
}

/*!
    \fn void QPlotMarker::setMarkerIcon(const QString &fileName, MarkerButtonIcon typeIcon)
    \brief Sets a marker icon.

    \a typeIcon
    Which icon to replace (MARKER_BUTTON_ICON etc.)

    \a fileName
    Path to image file (empty to reset to default)

    \sa markerIcon()
*/
void QPlotMarker::setMarkerIcon(const QString &fileName, MarkerButtonIcon typeIcon)
{
    Q_D(QPlotMarker);

    switch (typeIcon) {
    case MARKER_BUTTON_ICON:
        d->m_buttonIcon = fileName;
        break;

    case MARKER_BUTTON_CONTROL:
        d->m_buttonControl = fileName;
        break;

    case MARKER_BUTTON_SELECTED_ICON:
        d->m_buttonSelectedIcon = fileName;
        break;

    case MARKER_BUTTON_SELECTED_CONTROL:
        d->m_buttonsSelectedControl = fileName;
        break;

    default:
        return;
    }

    update();
}

/*!
    \fn QRectF QPlotMarker::controlRect() const
    \brief Returns the control item's rectangle.
*/
QRectF QPlotMarker::controlRect() const
{
    Q_D(const QPlotMarker);

    return d->m_controlItem->boundingRect();
}

/*!
    \fn void QPlotMarker::update()
    \brief Updates the marker's visual representation.
*/
void QPlotMarker::update()
{
    Q_D(QPlotMarker);

    auto plotArea = d->m_parentChart->plotArea();

    qreal x = qBound(plotArea.left(), d->m_markerPosition.x(), plotArea.right());
    qreal y = qBound(plotArea.top(), d->m_markerPosition.y(), plotArea.bottom());

    const QPointF targetPoint(x, y);

    if (d->m_movement == MOVEMENT_DEFAULT)

        move(targetPoint);

    else

        d->updateOnMoveByPoints(d->m_parentChart->mapToPosition(d->m_markerValue));
}

/*!
    \fn QVariant QPlotMarker::itemChange(GraphicsItemChange change, const QVariant &value)
    \internal
    \brief Handles item change events.
*/
QVariant QPlotMarker::itemChange(GraphicsItemChange change, const QVariant &value)
{
    auto result = QGraphicsWidget::itemChange(change, value);

    if (change == ItemSceneHasChanged and scene())

        moveBegin();

    return result;
}
