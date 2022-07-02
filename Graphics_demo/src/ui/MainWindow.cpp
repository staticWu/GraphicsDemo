#include "MainWindow.h"
#include "engine/xGraphicView.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "xActionDrawRegLine.h"
#include "xActionDrawRegCircle.h"
#include "xActionDrawRegPoint.h"
#include "xActionDrawRegStraightLine.h"
#include "xActionDrawStraightLine.h"
#include "xActionDrawLine.h"
#include "xActionDrawCircle.h"
#include "xDrawConcentricCircle.h"
#include "xActionDrawVerticalLine.h"
#include "xActionDrawLineToLine.h"
#include "xActionDrawLineToLine2.h"
#include "xActionDrawCircleToLine.h"
#include "xActionDrawCircleDiameter.h"
#include "xActionDrawMyArc.h"
#include "xActionDrawRegArc.h"
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.view_widget->installEventFilter(this);

	m_scene = new QGraphicsScene(this);
	m_view = new xGraphicView(m_scene, ui.view_widget);
	m_scene->setSceneRect(m_view->rect());

	auto hLayout1 = new QHBoxLayout(ui.view_widget);
	hLayout1->setContentsMargins(0, 0, 0, 0);
	hLayout1->setSpacing(0);
	hLayout1->addWidget(m_view);

	connect(ui.action_quit, &QAction::triggered, this, &QWidget::close);
	connect(ui.lineBtn, &QPushButton::clicked, this, &MainWindow::onDrawLine);
	connect(ui.circleBtn, &QPushButton::clicked, this, &MainWindow::onDrawCircle);
	connect(ui.straightLineBtn, &QPushButton::clicked, this, &MainWindow::onDrawStraightLine);
	connect(ui.regLineBtn, &QPushButton::clicked, this, &MainWindow::onDrawRegLine);
	connect(ui.regCircleBtn, &QPushButton::clicked, this, &MainWindow::onDrawRegCircle);
	connect(ui.regPointBtn, &QPushButton::clicked, this, &MainWindow::onDrawRegPoint);
	connect(ui.regStraightLineBtn, &QPushButton::clicked, this, &MainWindow::onDrawRegStraightLine);
	connect(ui.concentricCircleBtn, &QPushButton::clicked, this, &MainWindow::onDrawConcentricCircle);
	connect(ui.verticalBtn, &QPushButton::clicked, this, &MainWindow::onDrawVerticalLine);
	connect(ui.lineToLineBtn, &QPushButton::clicked, this, &MainWindow::onDrawLineToLine);
	connect(ui.LineToLine2, &QPushButton::clicked, this, &MainWindow::onDrawLineToLine2);
	connect(ui.circleToLineBtn, &QPushButton::clicked, this, &MainWindow::onDrawCircleToLine);
	connect(ui.circleDiameterBtn, &QPushButton::clicked, this, &MainWindow::onDrawCircleDiameter);
	connect(ui.arcBtn, &QPushButton::clicked, this, &MainWindow::onDrawArc);
	connect(ui.regArcBtn, &QPushButton::clicked, this, &MainWindow::onDrawRegArc);
}

MainWindow::~MainWindow()
{

}

void MainWindow::onDrawLine()
{
	xActionDrawLine* lineAction = new xActionDrawLine(m_view);
	m_view->setAction(lineAction);
}

void MainWindow::onDrawCircle()
{
	xActionDrawCircle* circleAction = new xActionDrawCircle(m_view);
	m_view->setAction(circleAction);
}

void MainWindow::onDrawRegPoint()
{
	xActionDrawRegPoint* pointAction = new xActionDrawRegPoint(m_view);
	m_view->setAction(pointAction);
}

void MainWindow::onDrawStraightLine()
{
	xActionDrawStraightLine* lineAction = new xActionDrawStraightLine(m_view);
	m_view->setAction(lineAction);
}

void MainWindow::onDrawRegLine()
{
	xActionDrawRegLine* lineAction = new xActionDrawRegLine(m_view);
	m_view->setAction(lineAction);
}

void MainWindow::onDrawRegCircle()
{
	xActionDrawRegCircle* circleAction = new xActionDrawRegCircle(m_view);
	m_view->setAction(circleAction);
}


void MainWindow::onDrawRegStraightLine()
{
	xActionDrawRegStraightLine* lineAction = new xActionDrawRegStraightLine(m_view);
	m_view->setAction(lineAction);
}

void MainWindow::onDrawConcentricCircle()
{
	xDrawConcentricCircle* circleAction = new xDrawConcentricCircle(m_view);
	m_view->setAction(circleAction);
}

void MainWindow::onDrawVerticalLine()
{
	xActionDrawVerticalLine* lineAction = new xActionDrawVerticalLine(m_view);
	m_view->setAction(lineAction);
}

void MainWindow::onDrawLineToLine()
{
	xActionDrawLineToLine* lineToLine = new xActionDrawLineToLine(m_view);
	m_view->setAction(lineToLine);
}

void MainWindow::onDrawLineToLine2()
{
	xActionDrawLineToLine2* lineToLine = new xActionDrawLineToLine2(m_view);
	m_view->setAction(lineToLine);
}

void MainWindow::onDrawCircleToLine()
{
	xActionDrawCircleToLine* circleToLine = new xActionDrawCircleToLine(m_view);
	m_view->setAction(circleToLine);
}

void MainWindow::onDrawCircleDiameter()
{
	xActionDrawCircleDiameter* circleToLine = new xActionDrawCircleDiameter(m_view);
	m_view->setAction(circleToLine);
}

void MainWindow::onDrawArc()
{
	xActionDrawMyArc* arc = new xActionDrawMyArc(m_view);
	m_view->setAction(arc);
}

void MainWindow::onDrawRegArc()
{
	xActionDrawRegArc* regArc = new xActionDrawRegArc(m_view);
	m_view->setAction(regArc);
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
