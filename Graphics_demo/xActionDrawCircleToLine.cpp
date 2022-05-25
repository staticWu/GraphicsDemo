#include "xActionDrawCircleToLine.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>
#include "xCircleToLine.h"

xActionDrawCircleToLine::xActionDrawCircleToLine(xGraphicView* view)
	: xActionPreviewInterface(view, xDef::AT_DrawCircleToLine)
{
}

xActionDrawCircleToLine::~xActionDrawCircleToLine()
{
	if (!isFinished())
		cancel();
}

void xActionDrawCircleToLine::mousePressEvent(QMouseEvent* e)
{
	auto spos = viewMapToScene(e);
	if (e->button() == Qt::LeftButton)
	{
		switch (m_status)
		{
		case xDef::S_Default:// 没有点
			mp = spos;
			m_status = xDef::S_DrawEntity1_P1;
			e->accept();
			break;

		case xDef::S_DrawEntity1_P1:// 点完一个点
			if (Distance(mp, spos) > DELTA_DIST_2)
			{
				circle_line->setFirstCirclePt1AndPt2(mp, spos);
				circle_line->setStyle(xStyle::Drawing);


				m_status = xDef::S_DrawEntity1_P2;
				e->accept();
			}
			break;
		case xDef::S_DrawEntity1_P2:// 点完两个点
			mp = spos;
			circle_line->setFirstCirclePt3(mp);
			circle_line->setStyle(xStyle::Drawing);
			m_status = xDef::S_DrawEntity1_P3;
			e->accept();
			break;
		case xDef::S_DrawEntity1_P3:// 点完两个点
			mp = spos;
			m_status = xDef::S_DrawEntity2_P1;
			e->accept();
			break;
		case xDef::S_DrawEntity2_P1:// 点完第三个点
			if (Distance(mp, spos) > DELTA_DIST_2)
			{
				circle_line->setSecondLine(mp, spos);
				circle_line->setStyle(xStyle::Drawing);


				m_status = xDef::S_DrawEntity2_P2;
				e->accept();
			}
			break;
		case xDef::S_DrawEntity2_P2:// 点完第四个点
			circle_line->setMousePos(spos);
			circle_line->setStyle(xStyle::Drawn);
			m_status = xDef::S_ActionFinished;
			e->accept();
			break;
		default:
			break;
		}
	}
}

void xActionDrawCircleToLine::mouseMoveEvent(QMouseEvent* e)
{
	switch (m_status)
	{
	case xDef::S_DrawEntity1_P1:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			if (circle_line == nullptr)
			{
				circle_line = new xCircleToLine(m_view);

				circle_line->setStyle(xStyle::Drawing);
				//connect(line_line, &xLineToLine::sendLineChange, this, &xActionDrawLineToLine::getLineChanged);
				m_scene->addItem(circle_line);
			}
			circle_line->setFirstCirclePt1AndPt2(mp, viewMapToScene(e));
			e->accept();
		}
		break;
	case xDef::S_DrawEntity1_P2:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			circle_line->setFirstCirclePt3(viewMapToScene(e));
			e->accept();
		}
		break;
	case xDef::S_DrawEntity2_P1:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			circle_line->setSecondLine(mp, viewMapToScene(e));
			e->accept();
		}
		break;
	case xDef::S_DrawEntity2_P2:

		circle_line->setMousePos(viewMapToScene(e));
	default:
		break;
	}
}

void xActionDrawCircleToLine::mouseReleaseEvent(QMouseEvent* e)
{
}

void xActionDrawCircleToLine::cancel()
{
	if (circle_line)
	{
		m_scene->removeItem(circle_line);

		delete circle_line;
		circle_line = nullptr;
	}
	m_status = xDef::S_Default;
}
