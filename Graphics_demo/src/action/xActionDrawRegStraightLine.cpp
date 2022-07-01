#include "xActionDrawRegStraightLine.h"
#include "xRegStraightLine.h"
#include <QMouseEvent>
#include <QDebug>
xActionDrawRegStraightLine::xActionDrawRegStraightLine(xGraphicView* view)
	: xActionPreviewInterface(view, xDef::AT_DrawRegStraightLine)
{
}

xActionDrawRegStraightLine::~xActionDrawRegStraightLine()
{
	if (!isFinished())
		cancel();
}

void xActionDrawRegStraightLine::mousePressEvent(QMouseEvent* e)
{
	auto spos = viewMapToScene(e);
	if (e->button() == Qt::LeftButton)
	{
		switch (m_status)
		{
		case xDef::AS_Default:
		{
			mp = spos;
			m_status = xDef::AS_DrawEntity1_P1;
			e->accept();
		}
		break;

		case xDef::AS_DrawEntity1_P1:
			if (Distance(mp, spos) > DELTA_DIST_2)
			{
				mp2 = spos;
				m_line->setStyle(xDef::S_RegDrawn);
				// test
				m_line->setSubLine(mp, spos); // 绘画不带范围的直线

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

void xActionDrawRegStraightLine::mouseMoveEvent(QMouseEvent* e)
{
	switch (m_status)
	{
	case xDef::AS_DrawEntity1_P1:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			if (m_line == nullptr)
			{
				m_line = new xRegStraightLine(m_view);
				m_line->setStyle(xDef::S_Drawing);
				m_scene->addItem(m_line);
			}
			m_line->setLine(mp, viewMapToScene(e), 30);
			e->accept();
		}
		break;

	default:
		break;
	}
}

void xActionDrawRegStraightLine::mouseReleaseEvent(QMouseEvent* e)
{
}

void xActionDrawRegStraightLine::cancel()
{
	if (m_line)
	{
		m_scene->removeItem(m_line);
		delete m_line;
		m_line = nullptr;
	}
	m_status = xDef::AS_Default;
}
