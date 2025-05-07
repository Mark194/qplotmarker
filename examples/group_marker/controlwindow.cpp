#include "controlwindow.hpp"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineSeries>

ControlWindow::ControlWindow(QWidget *parent)
    : QMainWindow(parent),
      m_groupMarker( new QPlotMarkerGroup )
{
    setWindowTitle( "Группировка маркеров" );

    resize( 800, 600 );

    createForm();
}

ControlWindow::~ControlWindow() {}


void ControlWindow::createForm()
{
    QWidget * centralWidget = new QWidget;

    setCentralWidget( centralWidget );


    QVBoxLayout * centralLayout = new QVBoxLayout;

    centralWidget->setLayout( centralLayout );


    auto layoutControls = createControls();

    centralLayout->addLayout( layoutControls );


    auto viewsLayout = createViews();

    centralLayout->addLayout( viewsLayout );
}

void ControlWindow::loadData(QChart * chart, double ampl, double freq, double phase, int count)
{
    QLineSeries * series = new QLineSeries;

    for ( int i = 0; i <= count; ++i )
    {
        double x = 2 * M_PI * i / count;

        double y = ampl * std::sin(freq * x + phase);

        series->append(x, y);
    }

    chart->addSeries( series );
}

QChartView * ControlWindow::createView()
{
    QChart *chart = new QChart();

    chart->legend()->setVisible( false );


    QChartView * view = new QChartView( chart );

    view->setRenderHint(QPainter::Antialiasing);

    return view;
}

QPlotMarker * ControlWindow::createMarker(QChart * chart)
{
    QPlotMarker * marker = new QPlotMarker( chart, Qt::red, Qt::Vertical );

    marker->setMovementStyle( QPlotMarker::MOVEMENT_BY_POINTS );

    marker->setLabelFormat( "%.2f" );

    chart->scene()->addItem( marker );

    return marker;
}

QLayout * ControlWindow::createViews()
{
    QHBoxLayout * layout = new QHBoxLayout;


    auto viewOne   = createView();

    auto viewOther = createView();

    layout->addWidget( viewOne );

    layout->addWidget( viewOther );


    loadData( viewOne->chart() );

    loadData( viewOther->chart() );


    viewOne->chart()->createDefaultAxes();

    viewOther->chart()->createDefaultAxes();


    m_oneMarker = createMarker( viewOne->chart() );

    m_otherMarker = createMarker( viewOther->chart() );

    m_groupMarker->addMarker( m_oneMarker );

    m_groupMarker->addMarker( m_otherMarker );

    return layout;
}

QLayout * ControlWindow::createControls()
{
    QHBoxLayout * controlLayout = new QHBoxLayout;


    QCheckBox * syncEditor = new QCheckBox( "Синхронизировать" );

    connect(syncEditor, &QCheckBox::clicked, m_groupMarker, &QPlotMarkerGroup::setSyncMovement );


    controlLayout->addWidget( syncEditor );


    return controlLayout;
}



