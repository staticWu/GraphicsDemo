#include "xActionDrawRegArc.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>
#include "xGraphicView.h"
#include "xRegArc.h"
#include "xPoint.h"
xActionDrawRegArc::xActionDrawRegArc(xGraphicView* view)
	: xActionPreviewInterface(view, xDef::AT_DrawRegArc)
{
}

xActionDrawRegArc::~xActionDrawRegArc()
{
	if (!isFinished())
		cancel();
}

void xActionDrawRegArc::mousePressEvent(QMouseEvent * e)
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
				m_arc->setArc(mp1, mp2, viewMapToScene(e),30);

				m_arc->setStyle(xDef::S_RegDrawn);
				// TEST
				m_arc->setSubArc(mp1, mp2, viewMapToScene(e));

				// ������ɣ�����ΪAS_ActionFinished
				m_status = xDef::AS_ActionFinished;
				e->accept();
			}
			break;

		default:
			break;
		}
	}
}

void xActionDrawRegArc::mouseMoveEvent(QMouseEvent* e)
{
	switch (m_status)
	{
	case xDef::AS_DrawEntity1_P1:
		// ��������ʱ��ʾΪ��ֱ��
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
		// ����������ʱɾ����ʱ��ֱ��
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
		if (m_arc == nullptr)
		{
			m_arc = new xRegArc(m_view);
			m_arc->setStyle(xDef::S_Drawing);
			m_scene->addItem(m_arc);
		}
		m_arc->setArc(mp1, mp2, viewMapToScene(e), 30);
		e->accept();
		break;

	default:
		break;
	}
}

void xActionDrawRegArc::mouseReleaseEvent(QMouseEvent* e)
{
}

void xActionDrawRegArc::cancel()
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
	if (m_arc)
	{
		m_scene->removeItem(m_arc);
		delete m_arc;
		m_arc = nullptr;
	}
	m_status = xDef::AS_Default;
}
