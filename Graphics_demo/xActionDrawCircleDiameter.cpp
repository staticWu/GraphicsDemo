#include "xActionDrawCircleDiameter.h"
#include "xCircleDiameter.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QDebug>

xActionDrawCircleDiameter::xActionDrawCircleDiameter(xGraphicView* view, qreal y_x)
	: xActionPreviewInterface(view, xDef::AT_DrawCircleDiameter)

{
	if (y_x > 0)
		y_x_ratio = y_x;
}

xActionDrawCircleDiameter::~xActionDrawCircleDiameter()
{
	if (!isFinished())
		cancel();
}

void xActionDrawCircleDiameter::mousePressEvent(QMouseEvent* e)
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
				circle_diameter->setCirclePt1AndPt2(mp, spos);
				circle_diameter->setStyle(xDef::S_Drawing);

				m_status = xDef::AS_DrawEntity1_P2;
				e->accept();
			}
			break;
		case xDef::AS_DrawEntity1_P2:// 点完两个点
			mp = spos;
			circle_diameter->setCirclePt3(mp);
			circle_diameter->setStyle(xDef::S_Drawing);
			m_status = xDef::AS_DrawEntity1_P3;
			e->accept();
			break;
		case xDef::AS_DrawEntity1_P3:// 点完三个点
			mp = spos;
			circle_diameter->setMousePos(spos);
			circle_diameter->setStyle(xDef::S_Drawn);
			m_status = xDef::AS_ActionFinished;
			e->accept();
			break;
		
		default:
			break;
		}
	}
}

void xActionDrawCircleDiameter::mouseMoveEvent(QMouseEvent* e)
{
	switch (m_status)
	{
	case xDef::AS_DrawEntity1_P1:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			if (circle_diameter == nullptr)
			{
				circle_diameter = new xCircleDiameter(m_view, nullptr, 10, y_x_ratio);
				connect(circle_diameter, &xCircleDiameter::CircleChange, this, &xActionDrawCircleDiameter::CircleChange);
				connect(circle_diameter, &xCircleDiameter::ComputeDistance, this, &xActionDrawCircleDiameter::ComputeDistance);
				circle_diameter->setStyle(xDef::S_Drawing);
				m_scene->addItem(circle_diameter);
			}
			circle_diameter->setCirclePt1AndPt2(mp, viewMapToScene(e));
			e->accept();
		}
		break;
	case xDef::AS_DrawEntity1_P2:
		if (Distance(mp, viewMapToScene(e)) > DELTA_DIST_2)
		{
			circle_diameter->setCirclePt3(viewMapToScene(e));
			e->accept();
		}
		break;
	case xDef::AS_DrawEntity1_P3:

		circle_diameter->setMousePos(viewMapToScene(e));
	default:
		break;
	}
}

void xActionDrawCircleDiameter::mouseReleaseEvent(QMouseEvent* e)
{
}

void xActionDrawCircleDiameter::cancel()
{
	if (circle_diameter)
	{
		m_scene->removeItem(circle_diameter);

		delete circle_diameter;
		circle_diameter = nullptr;
	}
	m_status = xDef::AS_Default;
}

void xActionDrawCircleDiameter::getCircle(QPointF cen, qreal r, QPointF cen_draw, QPointF cen_x_draw, QPointF cen_y_draw)
{
	if (circle_diameter != nullptr)
		circle_diameter->setMyCircle(cen, r, cen_draw, cen_x_draw, cen_y_draw);
}

void xActionDrawCircleDiameter::getDistance(QString t)
{
	if (circle_diameter != nullptr)
		circle_diameter->setMyText(t);
}

void xActionDrawCircleDiameter::getIsSuccess(bool is_s)
{
	if (circle_diameter != nullptr)
		circle_diameter->setIsSuccess(is_s);
}
