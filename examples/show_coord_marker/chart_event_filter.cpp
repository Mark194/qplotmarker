#include "chart_event_filter.hpp"

ChartEventFilter::ChartEventFilter(QObject *parent)
    : QObject{parent}, m_isShowCoord(false)
{}

void ChartEventFilter::setViewer(QChartView * chartView, QPlotMarker * marker)
{
    m_marker    = marker;

    m_chartView = chartView;
}

bool ChartEventFilter::eventFilter(QObject * watched, QEvent * event)
{
    if ( event->type() == QEvent::KeyPress )
    {
        auto keyEvent = static_cast<QKeyEvent *>( event );

        if ( keyEvent->key() == Qt::Key_V and m_marker )
        {
            m_isShowCoord = not m_isShowCoord;

            m_marker->showCoordinates( m_isShowCoord );

            return true;
        }
    }

    return QObject::eventFilter( watched, event );
}
