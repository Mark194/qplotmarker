#include "chart_event_filter.hpp"

ChartEventFilter::ChartEventFilter(QObject *parent)
    : QObject{parent}
    , m_isShowCoord(false)
    , m_marker(nullptr)
    , m_chartView(nullptr)
{}

void ChartEventFilter::setViewer(QChartView *chartView, QPlotMarker *marker)
{
    m_marker = marker;

    m_chartView = chartView;
}
//![1]
bool ChartEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        if (const auto keyEvent = dynamic_cast<QKeyEvent *>(event);
            keyEvent->key() == Qt::Key_V and m_marker) {
            m_isShowCoord = not m_isShowCoord;

            m_marker->showCoordinates(m_isShowCoord);

            return true;
        }
    }

    return QObject::eventFilter(watched, event);
}
//![1]
