#include "xActionDrawVerticalLine.h"
#include "xVerticalLine.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>

xActionDrawVerticalLine::xActionDrawVerticalLine(xGraphicView* view)
	:xActionPreviewInterface(view, xDef::AT_DrawVerticalLine)
{
}

xActionDrawVerticalLine::~xActionDrawVerticalLine()
{
	if (!isFinished())
		cancel();
}

void xActionDrawVerticalLine::mousePressEvent(QMouseEvent* e)
{
	auto spos = viewMapToScene(e);
	if (e->button() == Qt::LeftButton)
	{
		switch (m_status)
		{
		case xDef::S_Default:
			mp = spos;
			m_status = xDef::S_DrawEntity1_P1;
			e->accept();
			break;

		case xDef::S_DrawEntity1_P1:
			if (Distance(mp, spos) > DELTA_DIST_2)
			{
				mp2 = spos;
				m_line->setStraightLine(mp, spos);
				m_line->setStyle(xStyle::Drawing);

				m_status = xDef::S_DrawEntity1_P2;
				e->accept();
			}
			break;
		case xDef::S_DrawEntity1_P2:

				m_line->setPt3(spos);
				m_line->setStyle(xStyle::Drawn);

				// 操作完成，设置为S_ActionFinished
				m_status = xDef::S_ActionFinished;
				e->accept();
			break;
		default:
			break;
		}
	}
}

void xActionDrawVerticalLine::mouseMoveEvent(QMouseEvent* e)
{
	switch (m_status)
	{
	case xDef::S_DrawEntity1_P1:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			if (m_line == nullptr)
			{
				m_line = new xVerticalLine(m_view);
				m_line->setStyle(xStyle::Drawing);
				m_scene->addItem(m_line);
			}
			m_line->setStraightLine(mp, viewMapToScene(e));
			e->accept();
		}
		break;
	case xDef::S_DrawEntity1_P2:
		if (Distance(mp2, viewMapToScene(e)) > DELTA_DIST_2)
		{
			m_line->setPt3(viewMapToScene(e));
			e->accept();
		}
		break;
	default:
		break;
	}
}

void xActionDrawVerticalLine::mouseReleaseEvent(QMouseEvent* e)
{
}

void xActionDrawVerticalLine::cancel()
{
	if (m_line)
	{
		m_scene->removeItem(m_line);
		delete m_line;
		m_line = nullptr;
	}
	m_status = xDef::S_Default;
}
