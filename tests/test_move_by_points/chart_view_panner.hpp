#include <QMouseEvent>
#include <QtCharts>

class ChartViewPanner : public QObject
{
public:
    explicit ChartViewPanner(QChartView *chartView, QObject *parent = nullptr)
        : QObject(parent)
        , m_chartView(chartView)
        , m_isPanning(false)
    {
        if (!m_chartView) {
            qWarning() << "ChartViewPanner: chartView is null";
            return;
        }

        m_chartView->setMouseTracking(true);
        m_chartView->viewport()->installEventFilter(this);
    }

    ~ChartViewPanner() override = default;

protected:
    bool eventFilter(QObject *object, QEvent *event) override
    {
        if (!m_chartView || object != m_chartView->viewport()) {
            return QObject::eventFilter(object, event);
        }

        switch (event->type()) {
        case QEvent::MouseButtonPress:
            return handleMousePress(static_cast<QMouseEvent *>(event));
        case QEvent::MouseMove:
            return handleMouseMove(static_cast<QMouseEvent *>(event));
        case QEvent::MouseButtonRelease:
            return handleMouseRelease(static_cast<QMouseEvent *>(event));
        default:
            return QObject::eventFilter(object, event);
        }
    }

private:
    bool handleMousePress(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton) {
            m_isPanning = true;
            m_lastMousePos = event->pos();
            m_chartView->setCursor(Qt::ClosedHandCursor);
            return true;
        }
        return false;
    }

    bool handleMouseMove(QMouseEvent *event)
    {
        if (m_isPanning) {
            QPoint delta = event->pos() - m_lastMousePos;
            m_chartView->chart()->scroll(-delta.x(), delta.y());
            m_lastMousePos = event->pos();
            return true;
        }
        return false;
    }

    bool handleMouseRelease(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton && m_isPanning) {
            m_isPanning = false;
            m_chartView->setCursor(Qt::ArrowCursor);
            return true;
        }
        return false;
    }

    QChartView *m_chartView;
    bool m_isPanning;
    QPoint m_lastMousePos;
};