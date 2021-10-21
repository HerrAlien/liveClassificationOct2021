#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QFileDialog>
#include <NeuralNetworkImporter.h>
#include <QPixmap>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myWidget = new MyWidget();
    myWidget->setParent(ui->centralwidget);

    // do not change!!! I kill you!!!
    myWidget->setMouseTracking(false);

    ui->frame->setSizePolicy(QSizePolicy::Fixed,
                             QSizePolicy::Preferred);

    m_hLayout = new QHBoxLayout(ui->centralwidget);
    m_hLayout->addWidget(myWidget);
    m_hLayout->addWidget(ui->frame);

    connect (ui->actionLoad_JSON,
             &QAction::triggered,
             this,
             &MainWindow::onLoadActionActivated);

    connect (ui->actionNew,
             &QAction::triggered,
             this,
             &MainWindow::onNewActionActivated);

    connect (myWidget,
             &MyWidget::handDrawingDone,
             this,
             &MainWindow::onDrawingDone);

    connect (ui->penSizeSpinBox,
             &QSpinBox::valueChanged,
             myWidget,
             &MyWidget::onPenSizeChanged);

    myWidget->onPenSizeChanged( ui->penSizeSpinBox->value()  );

    connect (ui->penSizeSpinBox,
             &QSpinBox::valueChanged,
             this,
             &MainWindow::onDrawingDone);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete myWidget;
    delete m_hLayout;
}

void MainWindow::onLoadActionActivated()
{
    // pop up a file open dialog
    // get the path of the selected file
     QString fileName = QFileDialog::getOpenFileName();

    // TODO: set up the neural network ... ... ?
     std::string path = fileName.toStdString();
     m_myNetwork = NeuralNetworkImporter::FromJson(path);
    // set the status area to list the selected file
     ui->statusbar->showMessage(fileName);
}


void MainWindow::onNewActionActivated()
{
    // TODO: reset the container of points on the custom widgets();
    myWidget->ClearImage();
}


void MainWindow::onDrawingDone()
{
    /*
    - on mouse release =>
        => get the drawing, and make it usable by the label
        => scale it to 28x28
        => preview label shows the image as 28x28
        => pass the image to the classifier
            - take the preview image, and convert it from .... to Image
        => call setNum to tell what the classifier thought of our image
    */

    QPixmap widgetAxPixmap = myWidget->grab();
    auto scaledDown = widgetAxPixmap.scaled(
                28,
                28,
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation
                );

    ui->previewImage->setPixmap(scaledDown);

    // get the data, somehow ...

      QImage convertedToImage = scaledDown.toImage();
      convertedToImage.convertTo(QImage::Format_Grayscale8);

      Image toBeClassified;
      toBeClassified.pixels.insert(toBeClassified.pixels.begin(),
                                   (uint8_t*)convertedToImage.constBits(),
                                   (uint8_t*)convertedToImage.constBits() + 28*28);
      toBeClassified.rows = 28;
      toBeClassified.columns = 28;

      if (m_myNetwork.IsInitialized())
      {
        int valueFound = m_myNetwork.Predict(toBeClassified);
        ui->classificationOutput->setNum(valueFound);
      }
      else
      {
          m_alert.setText("Load net first!");
          m_alert.show();
      }
}

void MyWidget::ClearImage()
{
    m_mousePositions.clear();
    this->update();
}


void MyWidget::paintEvent(QPaintEvent *event)
{


    QPainter painter; // (this);
    painter.begin(this);

    painter.fillRect(0,
                     0,
                     this->width(),
                     this->height(),
                     QColor(0,0,0));

    QPen pen;
    pen.setWidth(m_penSize);
    pen.setColor(QColor(255,255,255));

    painter.setPen(pen);

    for (auto pt : m_mousePositions)
    {
        painter.drawPoint(pt);
    }
}

void MyWidget::onPenSizeChanged(int newSize)
{
    m_penSize = newSize;
    this->update();
}



void MyWidget::mouseMoveEvent(QMouseEvent *event)
{
    // - capture points, in a member data container, as we drag the mouse.
    this->update();
    m_mousePositions.push_back(event->pos());
}

void MyWidget::mouseReleaseEvent(QMouseEvent *event)
{
    emit handDrawingDone();
}
