#include "mainwindow.hpp"


#include <QLineSeries>
#include <QPushButton>
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle( "Перемещение маркера по точкам" );

    resize( 800, 600 );

    createForm();
}

MainWindow::~MainWindow() {}

void MainWindow::loadData(QChart * chart)
{
    double amplitude = 1.0;

    double frequency = 1.0;

    double phase = 0.0;

    int points = 10;


    QLineSeries * series = new QLineSeries;

    for ( int i = 0; i <= points; ++i )
    {
        double x = 2 * M_PI * i / points;

        double y = amplitude * std::sin(frequency * x + phase);

        series->append(x, y);
    }

    chart->addSeries( series );
}

void MainWindow::createForm()
{
    QWidget * centralWidget = new QWidget;

    setCentralWidget( centralWidget );


    QVBoxLayout * centralLayout = new QVBoxLayout;

    centralWidget->setLayout( centralLayout );


    auto view = createView();


    loadData( view->chart() );

    view->chart()->createDefaultAxes();

    createMarker( view->chart() );


    auto controls = createControls();


    centralLayout->addLayout( controls, 1 );

    centralLayout->addWidget( view, 7 );
}

QChartView * MainWindow::createView()
{
    QChart *chart = new QChart();

    chart->legend()->setVisible( false );


    QChartView * view = new QChartView( chart );

    view->setRenderHint(QPainter::Antialiasing);

    return view;
}

QLayout * MainWindow::createControls()
{
    QHBoxLayout * controlLayout = new QHBoxLayout;


    QPushButton * moveNextButton = new QPushButton( "Вперёд" );

    connect( moveNextButton, &QPushButton::clicked, m_marker, &QPlotMarker::moveToNextPoint );


    QPushButton * movePrevButton = new QPushButton( "Назад"  );

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

    m_marker->moveBegin();
}
