#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"

class xGraphicView;
class QGraphicsScene;
class OperationWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

public slots:
	void onDrawLine();
	void onDrawCircle();
	void onDrawStraightLine();
	void onDrawRegLine();
	void onDrawRegCircle();
	void onDrawRegPoint();
	void onDrawRegStraightLine();
	void onDrawConcentricCircle();
	void onDrawVerticalLine();
	void onDrawLineToLine();// 画线与线
	void onDrawLineToLine2();// 画线与线
	void onDrawCircleToLine();
	void onDrawCircleDiameter();
	void onDrawArc();
	void onDrawRegArc();
protected:
	void paintEvent(QPaintEvent *e) override;
	bool eventFilter(QObject *obj, QEvent *e) override;

private slots:

private:
	Ui::MainWindow ui;
	xGraphicView *m_view = nullptr;
	QGraphicsScene *m_scene = nullptr;
	QVBoxLayout *m_vLayout = nullptr;
};
