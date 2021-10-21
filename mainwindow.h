#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPoint>

#include <QMouseEvent>
#include <QHBoxLayout>
#include <NeuralNetwork.h>
#include <vector>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MyWidget : public QWidget
{
Q_OBJECT

public:
    void ClearImage();

protected:
    // the intent is to draw the circle here
    void paintEvent(QPaintEvent *event) override;

    // here be mouse events!
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:

    void handDrawingDone();

public slots:
    void onPenSizeChanged(int newSize);

private:

    std::vector<QPoint> m_mousePositions;
    int m_penSize = 20;

};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onLoadActionActivated();

    void onNewActionActivated();

    void onDrawingDone();

private:
    Ui::MainWindow *ui;
    MyWidget * myWidget;
    QHBoxLayout * m_hLayout;

    NeuralNet m_myNetwork;
    QMessageBox m_alert;


};
#endif // MAINWINDOW_H
