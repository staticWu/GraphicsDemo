#include "xActionDrawStraightLine.h"
#include "xStraightLine.h"
#include <QMouseEvent>
xActionDrawStraightLine::xActionDrawStraightLine(xGraphicView* view)
	:xActionPreviewInterface(view, xDef::AT_DrawStraightLine)
{
}

xActionDrawStraightLine::~xActionDrawStraightLine()
{
	if (!isFinished())
		cancel();
}

void xActionDrawStraightLine::mousePressEvent(QMouseEvent* e)
{
	auto spos = viewMapToScene(e);
	if (e->button() == Qt::LeftButton)
	{
		switch (m_status)
		{
		case xDef::S_Default:
		{
			// 如果第一个点选中了能拟合的物体就直接将当前实体进行拟合
			auto gi = m_view->itemAt(e->pos());
			// 辅助元素构造实体
			if (gi != nullptr && gi->type() > xEntity::ET_Entity_Start && gi->type() < xEntity::ET_Entity_End)
			{
				auto ri = static_cast<xEntity*>(gi);
				if (ri->type() == xEntity::ET_StraightLine) // 选中为直线,直接拟合
				{
					xStraightLine* temp = static_cast<xStraightLine*>(ri); // 选中的直线
					if (m_line == nullptr)
					{
						m_line = new xStraightLine(m_view);
						m_line->setStyle(xStyle::Drawn);
						m_scene->addItem(m_line);
					}
					m_line->setLine(temp->lineData());
					m_line->setAnchorPoint1(temp->anchorPoint1());
					m_line->setAnchorPoint2(temp->anchorPoint2());

					// 操作完成，设置为S_ActionFinished
					m_status = xDef::S_ActionFinished;
					e->accept();
					break;
				}
			}

			mp = spos;
			m_status = xDef::S_DrawEntity1_P1;
			e->accept();
		}
		break;

		case xDef::S_DrawEntity1_P1:
			if (Distance(mp, spos) > DELTA_DIST_2)
			{
				mp2 = spos;

				m_line->setAnchorPoint1(mp);
				m_line->setAnchorPoint2(mp2);
				m_line->setStyle(xStyle::Drawn);

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

void xActionDrawStraightLine::mouseMoveEvent(QMouseEvent* e)
{
	switch (m_status)
	{
	case xDef::S_DrawEntity1_P1:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			if (m_line == nullptr)
			{
				m_line = new xStraightLine(m_view);
				m_line->setStyle(xStyle::Drawing);
				m_scene->addItem(m_line);
			}
			// 计算直线的两点位置
			mp2 = viewMapToScene(e);

			m_line->setAnchorPoint1(mp);
			m_line->setAnchorPoint2(mp2);

			e->accept();
		}
		break;

	default:
		break;
	}
}

void xActionDrawStraightLine::mouseReleaseEvent(QMouseEvent* e)
{
}

void xActionDrawStraightLine::cancel()
{
	if (m_line)
	{
		m_scene->removeItem(m_line);
		delete m_line;
		m_line = nullptr;
	}
	m_status = xDef::S_Default;
}

