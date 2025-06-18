#include <QMouseEvent>
#include <QtCharts>

class ChartViewPanner : public QObject
{
    Q_OBJECT
public:
    explicit ChartViewPanner(QChartView *chartView, QObject *parent = nullptr)
        : QObject(parent)
        , m_chartView(chartView)
        , m_isPanning(false)
        , m_wasPanned(false)
    {
        if (!m_chartView) {
            qWarning() << "ChartViewPanner: chartView is null";
            return;
        }

        m_chartView->setMouseTracking(true);
        m_chartView->viewport()->installEventFilter(this);
    }

    ~ChartViewPanner() override = default;

signals:
    // Сигнал при перемещении графика
    void chartPanned(const QPointF &delta);
    // Сигнал при начале перемещения
    void panningStarted();
    // Сигнал при завершении перемещения
    void panningFinished(bool wasActuallyPanned);

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
        if (event->button() == Qt::RightButton) { // Изменили на правую кнопку
            m_isPanning = true;
            m_wasPanned = false;
            m_lastMousePos = event->pos();
            m_chartView->setCursor(Qt::ClosedHandCursor);
            emit panningStarted();
            return true;
        }
        return false;
    }

    bool handleMouseMove(QMouseEvent *event)
    {
        if (m_isPanning) {
            QPoint delta = event->pos() - m_lastMousePos;
            if (!delta.isNull()) {
                m_chartView->chart()->scroll(-delta.x(), delta.y());
                m_lastMousePos = event->pos();
                m_wasPanned = true;
                emit chartPanned(QPointF(delta));
                return true;
            }
        }
        return false;
    }

    bool handleMouseRelease(QMouseEvent *event)
    {
        if (event->button() == Qt::RightButton && m_isPanning) {
            m_isPanning = false;
            m_chartView->setCursor(Qt::ArrowCursor);
            emit panningFinished(m_wasPanned);
            return true;
        }
        return false;
    }

    QChartView *m_chartView;
    bool m_isPanning;
    bool m_wasPanned;
    QPoint m_lastMousePos;
};