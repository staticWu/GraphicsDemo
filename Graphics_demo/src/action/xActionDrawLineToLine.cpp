#include "xActionDrawLineToLine.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>
#include "engine/xGraphicView.h"
#include "entity/xRegLine.h"
#include "xText.h"

	
xActionDrawLineToLine::xActionDrawLineToLine(xGraphicView* view)
	: xActionPreviewInterface(view, xDef::AT_DrawLineToLine)
{
}

xActionDrawLineToLine::~xActionDrawLineToLine()
{
	if (!isFinished())
		cancel();
}

void xActionDrawLineToLine::mousePressEvent(QMouseEvent* e)
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
				m_line_1->setLine(mp, spos, 30);
				m_line_1->setStyle(xStyle::RegDrawn);
				// TEST
				m_line_1->setSubLine(mp, spos);

				m_status = xDef::S_DrawEntity1_P2;
				e->accept();
			}
			break;
		case xDef::S_DrawEntity1_P2:// 点完两个点
			mp = spos;
			m_status = xDef::S_DrawEntity2_P1;
			e->accept();
			break;
		case xDef::S_DrawEntity2_P1:// 点完第三个点
			if (Distance(mp, spos) > DELTA_DIST_2)
			{
				m_line_2->setLine(mp, spos, 30);
				m_line_2->setStyle(xStyle::RegDrawn);
				// TEST
				m_line_2->setSubLine(mp, spos);

				m_status = xDef::S_DrawEntity2_P2;
				e->accept();
			}
			break;
		case xDef::S_DrawEntity2_P2:// 点完第四个点
			m_text->setMousePos(spos);
			m_text->setStyle(xStyle::Drawn);
			m_status = xDef::S_ActionFinished;
			e->accept();
			break;
		default:
			break;
		}
	}
}

void xActionDrawLineToLine::mouseMoveEvent(QMouseEvent* e)
{
	switch (m_status)
	{
	case xDef::S_DrawEntity1_P1:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			if (m_line_1 == nullptr)
			{
				m_line_1 = new xRegLine(m_view);
				m_line_1->setRegWidth(30);
				m_line_1->setStyle(xStyle::RegDrawing);
				connect(m_line_1, &xRegLine::sendLineChange, this, &xActionDrawLineToLine::getLineChanged);
				m_scene->addItem(m_line_1);
			}
			m_line_1->setLine(mp, viewMapToScene(e), 30);
			e->accept();
		}
		break;
	case xDef::S_DrawEntity2_P1:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			if (m_line_2 == nullptr)
			{
				m_line_2 = new xRegLine(m_view);
				m_line_2->setRegWidth(30);
				m_line_2->setStyle(xStyle::RegDrawing);
				connect(m_line_2, &xRegLine::sendLineChange, this, &xActionDrawLineToLine::getLineChanged);
				m_scene->addItem(m_line_2);
			}
			m_line_2->setLine(mp, viewMapToScene(e), 30);
			e->accept();
		}
		break;
	case xDef::S_DrawEntity2_P2:
		if (m_text == nullptr)
		{
			m_text = new xText(m_line_1->middlePoint(), m_line_2->middlePoint(), m_view);
			m_text->setMyText("dsadsa");
			m_text->setStyle(xStyle::Drawing);
			
			m_scene->addItem(m_text);
		}
		m_text->setMousePos(viewMapToScene(e));
	default:
		break;
	}
}

void xActionDrawLineToLine::mouseReleaseEvent(QMouseEvent* e)
{
}

void xActionDrawLineToLine::cancel()
{
	if (m_line_1)
	{
		m_scene->removeItem(m_line_1);
		m_line_1->disconnect();
		delete m_line_1;
		m_line_1 = nullptr;
	}
	if (m_line_2)
	{
		m_scene->removeItem(m_line_2);
		m_line_2->disconnect();
		delete m_line_2;
		m_line_2 = nullptr;
	}
	if (m_text)
	{
		m_scene->removeItem(m_text);
		m_text->disconnect();
		delete m_text;
		m_text = nullptr;
	}
	m_status = xDef::S_Default;
}

void xActionDrawLineToLine::getLineChanged(QPointF p)
{
	if (m_line_1 != nullptr && m_line_2 != nullptr && m_text != nullptr)
	{
		m_text->setMidPoint(m_line_1->middlePoint(), m_line_2->middlePoint());
	}
}
