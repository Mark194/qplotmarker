#include "controlwindow.hpp"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineSeries>

ControlWindow::ControlWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_groupMarker(new QPlotMarkerGroup)
{
    setWindowTitle("Grouping markers");

    resize(800, 600);

    createForm();
}

ControlWindow::~ControlWindow() = default;

void ControlWindow::createForm()
{
    auto *centralWidget = new QWidget;

    setCentralWidget(centralWidget);

    auto *centralLayout = new QVBoxLayout;

    centralWidget->setLayout(centralLayout);

    const auto layoutControls = createControls();

    centralLayout->addLayout(layoutControls);

    const auto viewsLayout = createViews();

    centralLayout->addLayout(viewsLayout);
}

void ControlWindow::loadData(
    QChart *chart, const double ampl, const double freq, const double phase, const int count)
{
    auto *series = new QLineSeries;

    for (int i = 0; i <= count; ++i) {
        double x = 2 * M_PI * i / count;

        double y = ampl * std::sin(freq * x + phase);

        series->append(x, y);
    }

    chart->addSeries(series);
}

QChartView *ControlWindow::createView()
{
    auto *chart = new QChart();

    chart->legend()->setVisible(false);

    auto *view = new QChartView(chart);

    view->setRenderHint(QPainter::Antialiasing);

    return view;
}
//![1]
QPlotMarker *ControlWindow::createMarker(QChart *chart)
{
    auto *marker = new QPlotMarker(chart, Qt::red, Qt::Vertical);

    marker->setMovementStyle(QPlotMarker::MOVEMENT_BY_POINTS);

    marker->setLabelFormat("%.2f");

    chart->scene()->addItem(marker);

    return marker;
}

//![1]

QLayout *ControlWindow::createViews()
{
    auto *layout = new QHBoxLayout;

    const auto viewOne = createView();

    const auto viewOther = createView();

    layout->addWidget(viewOne);

    layout->addWidget(viewOther);

    loadData(viewOne->chart());

    loadData(viewOther->chart());

    viewOne->chart()->createDefaultAxes();

    viewOther->chart()->createDefaultAxes();

    //![2]

    m_oneMarker = createMarker(viewOne->chart());

    m_otherMarker = createMarker(viewOther->chart());

    m_groupMarker->addMarker(m_oneMarker);

    m_groupMarker->addMarker(m_otherMarker);

    //![2]

    return layout;
}

QLayout *ControlWindow::createControls()
{
    auto *controlLayout = new QHBoxLayout;

    auto *syncEditor = new QCheckBox("Synchronize");

    connect(syncEditor, &QCheckBox::clicked, m_groupMarker, &QPlotMarkerGroup::setSyncMovement);

    controlLayout->addWidget(syncEditor);

    return controlLayout;
}
