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
					m_line->setStyle(xStyle::Drawn);

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

				calStraighLinePoints();
				if (pt1.isNull() || pt2.isNull())
					return mouseMoveEvent(e);

				m_line->setLine(pt1, pt2);
				m_line->setAnchorPoint1(mp);
				m_line->setAnchorPoint2(mp2);
				m_line->setStyle(xStyle::Drawn);
				// TEST
				//m_line->setSubLine(mp, spos);

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
				m_line->setStyle(xStyle::Drawn);
				m_scene->addItem(m_line);
			}
			// 计算直线的两点位置
			mp2 = viewMapToScene(e);
			calStraighLinePoints();
			if (pt1.isNull() || pt2.isNull())
				return mouseMoveEvent(e);
			m_line->setAnchorPoint2(mp2);
			m_line->setLine(pt1, pt2);

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

void xActionDrawStraightLine::calStraighLinePoints()
{
	// 计算画线的起始点和结束点 根据点的直线方程 k = (y2-y1) / (x2-x1)
	if (mp.isNull() || mp2.isNull())
		return;

	double disX = mp2.x() - mp.x();
	if (disX == 0) // 垂直时
	{
		pt1.setX(mp.x());
		pt1.setY(0);
		pt2.setX(mp.x());
		pt2.setY(m_view->height());
		return;
	}

	double k = (mp2.y() - mp.y()) / disX;
	if (k == 0)  // 平行时
	{
		pt1.setX(0);
		pt1.setY(mp.y());
		pt2.setX(m_view->width());
		pt2.setY(mp.y());
	}
	else // 开始点小于结束点
	{
		pt1.setY(0);
		pt2.setY(m_view->height());
		double startX = (pt1.y() - mp.y()) / k + mp.x();
		double endX = (pt2.y() - mp.y()) / k + mp.x();
		pt1.setX(startX);
		pt2.setX(endX);
	}
}
