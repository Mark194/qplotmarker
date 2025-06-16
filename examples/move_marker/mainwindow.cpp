#include "mainwindow.hpp"


#include <QLineSeries>
#include <QPushButton>
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle( "Moving the marker by points" );

    resize( 800, 600 );

    createForm();
}

MainWindow::~MainWindow() = default;

void MainWindow::loadData(QChart * chart)
{
    constexpr int points = 10;


    auto * series = new QLineSeries;

    for ( int i = 0; i <= points; ++i )
    {
        constexpr double phase = 0.0;
        constexpr double frequency = 1.0;
        constexpr double amplitude = 1.0;
        const double x = 2 * M_PI * i / points;

        const double y = amplitude * std::sin(frequency * x + phase);

        series->append(x, y);
    }

    chart->addSeries( series );
}

void MainWindow::createForm()
{
    auto * centralWidget = new QWidget;

    setCentralWidget( centralWidget );


    auto * centralLayout = new QVBoxLayout;

    centralWidget->setLayout( centralLayout );


    const auto view = createView();


    loadData( view->chart() );

    view->chart()->createDefaultAxes();

    createMarker( view->chart() );


    const auto controls = createControls();


    centralLayout->addWidget( view, 7 );

    centralLayout->addLayout( controls, 1 );
}

QChartView * MainWindow::createView()
{
    const auto chart = new QChart();

    chart->legend()->setVisible( false );


    const auto view = new QChartView( chart );

    view->setRenderHint(QPainter::Antialiasing);

    return view;
}

QLayout * MainWindow::createControls()
{
    auto * controlLayout = new QHBoxLayout;


    auto * moveNextButton = new QPushButton( "Next" );

    connect( moveNextButton, &QPushButton::clicked, m_marker, &QPlotMarker::moveToNextPoint );


    auto * movePrevButton = new QPushButton( "Back"  );

    connect( movePrevButton, &QPushButton::clicked, m_marker, &QPlotMarker::moveToPreviousPoint );


    controlLayout->addWidget( movePrevButton );

    controlLayout->addWidget( moveNextButton );


    return controlLayout;
}

void MainWindow::createMarker(QChart * chart)
{
    m_marker = new QPlotMarker( chart, Qt::red, Qt::Vertical );

    m_marker->setMovementStyle( QPlotMarker::MOVEMENT_BY_POINTS );

    m_marker->setLabelFormat( "%.2f" );

    chart->scene()->addItem( m_marker );
}
