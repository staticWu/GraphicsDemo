#include "MainWindow.h"
#include "engine/xGraphicView.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "xActionDrawLine.h"
#include "xActionDrawCircle.h"
#include "xActionDrawPoint.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.view_widget->installEventFilter(this);

	m_scene = new QGraphicsScene(this);
	m_view = new xGraphicView(m_scene, ui.view_widget);
	
	auto hLayout1 = new QHBoxLayout(ui.view_widget);
	hLayout1->setContentsMargins(0, 0, 0, 0);
	hLayout1->setSpacing(0);
	hLayout1->addWidget(m_view);

	connect(ui.action_quit, &QAction::triggered, this, &QWidget::close);
	connect(ui.lineBtn, &QPushButton::clicked, this, &MainWindow::onDrawLine);
	connect(ui.circleBtn, &QPushButton::clicked, this, &MainWindow::onDrawCircle);
	connect(ui.pointBtn, &QPushButton::clicked, this, &MainWindow::onDrawPoint);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onDrawLine()
{
	xActionDrawLine *lineAction = new xActionDrawLine(m_view);
	m_view->setAction(lineAction);
}

void MainWindow::onDrawCircle()
{
	xActionDrawCircle *circleAction = new xActionDrawCircle(m_view);
	m_view->setAction(circleAction);
}

void MainWindow::onDrawPoint()
{
	xActionDrawPoint* pointAction = new xActionDrawPoint(m_view);
	m_view->setAction(pointAction);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
	QStyleOption op;
	op.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &op, &p, this);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
	if (obj == ui.view_widget)
	{
		if (e->type() == QEvent::Resize)
		{
			// 窗口大小改变时调整视图场景大小
			m_scene->setSceneRect(ui.view_widget->rect());
			return true;
		}
		return false;
	}
	else
	{
		return QMainWindow::eventFilter(obj, e);
	}
}
