#include "xActionDrawLine.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>
#include "engine/xGraphicView.h"
#include "entity/xRegLine.h"
xActionDrawLine::xActionDrawLine(xGraphicView* view)
	:xActionPreviewInterface(view,xDef::AT_DrawLine)
{
}

xActionDrawLine::~xActionDrawLine()
{
	if (!isFinished())
		cancel();
}

void xActionDrawLine::mousePressEvent(QMouseEvent* e)
{
	// 鼠标点位置
	auto spos = viewMapToScene(e);
	if (e->button() == Qt::LeftButton)
	{
		switch (m_status)
		{
		case xDef::S_Default:// 默认，刚开始
			mp = spos;// 第一个点
			m_status = xDef::S_DrawEntity1_P1;// 第一个点
			e->accept();
			break;

		case xDef::S_DrawEntity1_P1:// 画完第一个点
			if (Distance(mp, spos) > DELTA_DIST_2)// 两个点不重合
			{
				m_line->setLine(mp, spos);// 画线
				m_line->setStyle(xStyle::Drawn);// 固定画

				// 操作完成，设置为S_ActionFinished
				m_status = xDef::S_ActionFinished;
				e->accept();
			}
			break;

		default:
			break;
		}
	}
}

void xActionDrawLine::mouseMoveEvent(QMouseEvent* e)
{
	switch (m_status)
	{
	case xDef::S_DrawEntity1_P1:// 画完第一个点
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			if (m_line == nullptr)
			{
				m_line = new xLine(m_view);
				m_line->setStyle(xStyle::Drawing);// 动态画
				m_scene->addItem(m_line);
			}
			m_line->setLine(mp, viewMapToScene(e));// 补上第二个点
			e->accept();
		}
		break;

	default:
		break;
	}
}

void xActionDrawLine::mouseReleaseEvent(QMouseEvent* e)
{

}
// 取消
void xActionDrawLine::cancel()
{
	if (m_line)
	{
		m_scene->removeItem(m_line);
		delete m_line;
		m_line = nullptr;
	}
	m_status = xDef::S_Default;// 恢复默认，起始没点
}
