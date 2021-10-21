#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myWidget = new MyWidget();
    myWidget->setParent(ui->centralwidget);

    myWidget->setMouseTracking(true);

    ui->frame->setSizePolicy(QSizePolicy::Fixed,
                             QSizePolicy::Preferred);

    m_hLayout = new QHBoxLayout(ui->centralwidget);
    m_hLayout->addWidget(myWidget);
    m_hLayout->addWidget(ui->frame);

/*
    WHEN myWidget->mouseMoved
    THEN this->onMouseMoved
*/

    connect (myWidget,
             &MyWidget::mouseMoved,
             this,
             &MainWindow::onMouseMoved);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete myWidget;
    delete m_hLayout;
}


void MainWindow::onMouseMoved(QPoint coordinates)
{
    ui->m_xLabel->setNum(coordinates.x());
    ui->m_yLabel->setNum(coordinates.y());
}


void MyWidget::paintEvent(QPaintEvent *event)
{
 // paint at the mouse coordinates (m_mousePosition)
    // draw a circle
    QPainter painter; // (this);
    painter.begin(this);

    painter.drawEllipse(m_mousePosition, 20, 20);
}

void MyWidget::mouseMoveEvent(QMouseEvent *event)
{
 // get the mouse coordinates
    m_mousePosition = event->pos();
    // the coordinates have changed! Update the drawing!
    this->update();
    // pass the coordinates to the main windoW!
    // => we must emit a signal,
    // and (hopefylly!) the main window is listening for it!
    emit mouseMoved(m_mousePosition);
}
