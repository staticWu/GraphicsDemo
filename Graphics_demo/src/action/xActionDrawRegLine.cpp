#include "xActionDrawRegLine.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>
#include "engine/xGraphicView.h"
#include "entity/xRegLine.h"

xActionDrawRegLine::xActionDrawRegLine(xGraphicView *view)
	: xActionPreviewInterface(view, xDef::AT_DrawRegLine)
{
}

xActionDrawRegLine::~xActionDrawRegLine()
{
	if (!isFinished())
		cancel();
}

void xActionDrawRegLine::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		auto spos = viewMapToScene(e);
		switch (m_status)
		{
		case xDef::AS_Default:
			mp = spos;
			m_status = xDef::AS_DrawEntity1_P1;
			e->accept();
			break;

		case xDef::AS_DrawEntity1_P1:
			if (Distance(mp, spos) > DELTA_DIST_2)
			{
				m_line->setLine(mp, spos, 30);
				m_line->setStyle(xDef::S_RegDrawn);
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

void xActionDrawRegLine::mouseMoveEvent(QMouseEvent *e)
{
	switch (m_status)
	{
	case xDef::AS_DrawEntity1_P1:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			if (m_line == nullptr)
			{
				m_line = new xRegLine(m_view);
				m_line->setRegWidth(30);
				m_line->setStyle(xDef::S_RegDrawing);
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

void xActionDrawRegLine::mouseReleaseEvent(QMouseEvent *e)
{
}

void xActionDrawRegLine::cancel()
{
	if (m_line)
	{
		m_scene->removeItem(m_line);
		delete m_line;
		m_line = nullptr;
	}
	m_status = xDef::AS_Default;
}
