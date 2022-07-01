#include "xActionDrawCircle.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>
#include "xGraphicView.h"
#include "xRegCircle.h"
#include "xPoint.h"

xActionDrawCircle::xActionDrawCircle(xGraphicView* view)
	: xActionPreviewInterface(view, xDef::AT_DrawCircle)
{
}

xActionDrawCircle::~xActionDrawCircle()
{
	if (!isFinished())
		cancel();
}

void xActionDrawCircle::mousePressEvent(QMouseEvent* e)
{	
	auto spos = viewMapToScene(e);
	if (e->button() == Qt::LeftButton)
	{
		switch (m_status)
		{
		case xDef::AS_Default:
			if (m_point1 == nullptr)
			{
				m_point1 = new xPoint(m_view);
				m_point1->setStyle(xDef::S_Drawn);
				m_scene->addItem(m_point1);
			}
			mp1 = spos;
			m_status = xDef::AS_DrawEntity1_P1;
			m_point1->setPt(mp1);
			e->accept();
			break;

		case xDef::AS_DrawEntity1_P1:
			if (Distance(mp1, spos) > DELTA_DIST_2)
			{
				mp2 = spos;
				m_point2->setPt(mp2);
				m_status = xDef::AS_DrawEntity1_P2;
				e->accept();
			}
			break;

		case xDef::AS_DrawEntity1_P2:
			if (Distance(mp1, spos) > DELTA_DIST_2 && Distance(mp2, spos) > DELTA_DIST_2)
			{
				m_circle->setCircle(mp1, mp2, viewMapToScene(e));
				m_circle->setStyle(xDef::S_Drawn);

				// 操作完成，设置为AS_ActionFinished
				m_status = xDef::AS_ActionFinished;
				e->accept();
			}
			break;

		default:
			break;
		}
	}
}

void xActionDrawCircle::mouseMoveEvent(QMouseEvent* e)
{
	switch (m_status)
	{
	case xDef::AS_DrawEntity1_P1:
		// 画两个点时显示为画直线
		if (m_point2 == nullptr)
		{
			m_point2 = new xPoint(m_view);
			m_point2->setStyle(xDef::S_Drawn);
			m_scene->addItem(m_point2);
		}
		m_point2->setPt(viewMapToScene(e));
		e->accept();
		break;

	case xDef::AS_DrawEntity1_P2:
		// 画第三个点时删除临时的直线
		if (m_point1)
		{
			m_scene->removeItem(m_point1);
			delete m_point1;
			m_point1 = nullptr;
		}
		if (m_point2)
		{
			m_scene->removeItem(m_point2);
			delete m_point2;
			m_point2 = nullptr;
		}
		if (m_circle == nullptr)
		{
			m_circle = new xCircle(m_view);
			m_circle->setStyle(xDef::S_Drawing);
			m_scene->addItem(m_circle);
		}
		m_circle->setCircle(xCircleData(mp1, mp2, viewMapToScene(e)));
		e->accept();
		break;

	default:
		break;
	}
}

void xActionDrawCircle::mouseReleaseEvent(QMouseEvent* e)
{

}

void xActionDrawCircle::cancel()
{
	if (m_point1)
	{
		m_scene->removeItem(m_point1);
		delete m_point1;
		m_point1 = nullptr;
	}
	if (m_point2)
	{
		m_scene->removeItem(m_point2);
		delete m_point2;
		m_point2 = nullptr;
	}
	if (m_circle)
	{
		m_scene->removeItem(m_circle);
		delete m_circle;
		m_circle = nullptr;
	}
	m_status = xDef::AS_Default;
}
