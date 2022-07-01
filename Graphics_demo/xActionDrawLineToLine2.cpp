#include "xActionDrawLineToLine2.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>
#include "xLineToLine.h"

xActionDrawLineToLine2::xActionDrawLineToLine2(xGraphicView* view)
	: xActionPreviewInterface(view, xDef::AT_DrawLineToLine)
{
}

xActionDrawLineToLine2::~xActionDrawLineToLine2()
{
	if (!isFinished())
		cancel();
}

void xActionDrawLineToLine2::mousePressEvent(QMouseEvent* e)
{
	auto spos = viewMapToScene(e);
	if (e->button() == Qt::LeftButton)
	{
		switch (m_status)
		{
		case xDef::AS_Default:// 没有点
			mp = spos;
			m_status = xDef::AS_DrawEntity1_P1;
			e->accept();
			break;

		case xDef::AS_DrawEntity1_P1:// 点完一个点
			if (Distance(mp, spos) > DELTA_DIST_2)
			{
				line_line->setFirstLine(mp, spos);
				line_line->setStyle(xDef::S_Drawing);


				m_status = xDef::AS_DrawEntity1_P2;
				e->accept();
			}
			break;
		case xDef::AS_DrawEntity1_P2:// 点完两个点
			mp = spos;
			m_status = xDef::AS_DrawEntity2_P1;
			e->accept();
			break;
		case xDef::AS_DrawEntity2_P1:// 点完第三个点
			if (Distance(mp, spos) > DELTA_DIST_2)
			{
				line_line->setSecondLine(mp, spos);
				line_line->setStyle(xDef::S_Drawing);


				m_status = xDef::AS_DrawEntity2_P2;
				e->accept();
			}
			break;
		case xDef::AS_DrawEntity2_P2:// 点完第四个点
			line_line->setMousePos(spos);
			line_line->setStyle(xDef::S_Drawn);
			m_status = xDef::AS_ActionFinished;
			e->accept();
			break;
		default:
			break;
		}
	}
}

void xActionDrawLineToLine2::mouseMoveEvent(QMouseEvent* e)
{
	switch (m_status)
	{
	case xDef::AS_DrawEntity1_P1:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			if (line_line == nullptr)
			{
				line_line = new xLineToLine(m_view);

				line_line->setStyle(xDef::S_Drawing);
				//connect(line_line, &xLineToLine::sendLineChange, this, &xActionDrawLineToLine::getLineChanged);
				m_scene->addItem(line_line);
			}
			line_line->setFirstLine(mp, viewMapToScene(e));
			e->accept();
		}
		break;
	case xDef::AS_DrawEntity2_P1:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			line_line->setSecondLine(mp, viewMapToScene(e));
			e->accept();
		}
		break;
	case xDef::AS_DrawEntity2_P2:

		line_line->setMousePos(viewMapToScene(e));
	default:
		break;
	}
}

void xActionDrawLineToLine2::mouseReleaseEvent(QMouseEvent* e)
{
}

void xActionDrawLineToLine2::cancel()
{
	if (line_line)
	{
		m_scene->removeItem(line_line);

		delete line_line;
		line_line = nullptr;
	}
	m_status = xDef::AS_Default;
}

