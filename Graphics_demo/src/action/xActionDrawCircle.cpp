#include "xActionDrawCircle.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>
#include "engine/xGraphicView.h"
#include "entity/xCircle.h"
#include "entity/xLine.h"

xActionDrawCircle::xActionDrawCircle(xGraphicView *view)
	: xActionPreviewInterface(view, xDef::AT_DrawCircle)
{
}

xActionDrawCircle::~xActionDrawCircle()
{
}

void xActionDrawCircle::mousePressEvent(QMouseEvent *e)
{
	auto spos = pointMapToScene(e);
	if (e->button() == Qt::LeftButton)
	{
		switch (m_status)
		{
		case xDef::S_Default:
			p1 = spos;
			m_status = xDef::S_DrawEntity1_P1;
			e->accept();
			break;

		case xDef::S_DrawEntity1_P1:
			if (Distance(p1, spos) > 10)
			{
				p2 = spos;
				m_status = xDef::S_DrawEntity1_P2;
				e->accept();
			}
			break;

		case xDef::S_DrawEntity1_P2:
			if (Distance(p1, spos) > 10 && Distance(p2, spos) > 10)
			{
				m_circle->setStyle(xStyle::Drawn);
				m_status = xDef::S_ActionFinished;
				e->accept();
			}
			break;

		case xDef::S_DrawEntity1_P3:
			//p1 = spos;
			//if (m_circle == nullptr)
			//{
			//	m_circle = new xCircle();
			//	m_scene->addItem(m_circle);
			//}
			//if (QLineF(p1, m_line->pt1()).length() < 6)
			//{
			//	m_isGrabCtrlPoint = true;
			//	m_moveP1 = true;
			//	m_processed = true;
			//}
			//else if (QLineF(p1, m_line->pt2()).length() < 6)
			//{
			//	m_isGrabCtrlPoint = true;
			//	m_moveP2 = true;
			//	m_processed = true;
			//}
			break;

		case xDef::S_DrawFinished:
		case xDef::S_ActionFinished:
		default:
			break;
		}
	}
}

void xActionDrawCircle::mouseMoveEvent(QMouseEvent *e)
{
	switch (m_status)
	{
	case xDef::S_DrawEntity1_P1:
		if (m_line == nullptr)
		{
			m_line = new xLine(m_view);
			m_line->setStyle(xStyle::Drawing);
			m_scene->addItem(m_line);
		}
		m_line->setLine(p1, pointMapToScene(e));
		e->accept();
		break;

	case xDef::S_DrawEntity1_P2:
		if (m_line)
		{
			m_scene->removeItem(m_line);
			delete m_line;
			m_line = nullptr;
		}
		if (m_circle == nullptr)
		{
			m_circle = new xCircle(m_view);
			m_circle->setStyle(xStyle::Drawing);
			m_scene->addItem(m_circle);
		}
		m_circle->setCircle(xCircleData(p1, p2, pointMapToScene(e)));
		e->accept();
		break;

	case xDef::S_DrawEntity1_P3:
		break;
	case xDef::S_DrawFinished:
		break;
	case xDef::S_ActionFinished:
		break;
	default:
		break;
	}
}

void xActionDrawCircle::mouseReleaseEvent(QMouseEvent *e)
{
	m_isGrabCtrlPoint = false;
}

void xActionDrawCircle::cancel()
{
	if (m_line)
	{
		m_scene->removeItem(m_line);
		delete m_line;
		m_line = nullptr;
	}
	if (m_circle)
	{
		m_scene->removeItem(m_circle);
		delete m_circle;
		m_circle = nullptr;
	}
	m_status = xDef::S_Default;
}
