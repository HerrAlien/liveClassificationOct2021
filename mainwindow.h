#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPoint>

#include <QMouseEvent>
#include <QHBoxLayout>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MyWidget : public QWidget
{
Q_OBJECT

protected:
    // the intent is to draw the circle here
    void paintEvent(QPaintEvent *event) override;

    // here be mouse events!
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void mouseMoved (QPoint theNewCoordinates);

private:

    QPoint m_mousePosition;

};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onMouseMoved(QPoint coordinates);

private:
    Ui::MainWindow *ui;
    MyWidget * myWidget;
    QHBoxLayout * m_hLayout;

};
#endif // MAINWINDOW_H
