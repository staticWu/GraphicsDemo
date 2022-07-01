#include "xActionDrawRegPoint.h"
#include "xRegPoint.h"
#include <QMouseEvent>
xActionDrawRegPoint::xActionDrawRegPoint(xGraphicView* view)
	: xActionPreviewInterface(view, xDef::AT_DrawRegPoint)
{
}

xActionDrawRegPoint::~xActionDrawRegPoint()
{
	if (!isFinished())
		cancel();
}

void xActionDrawRegPoint::mousePressEvent(QMouseEvent* e)
{
	auto spos = viewMapToScene(e);
	if (e->button() == Qt::LeftButton)
	{
		switch (m_status)
		{
		case xDef::AS_Default:
			if (m_regPoint == nullptr)
			{
				m_regPoint = new xRegPoint(m_view);
				m_regPoint->setStyle(xDef::S_RegDrawn);
				m_scene->addItem(m_regPoint);
				m_regPoint->setRadius(5); // 默认半径
				m_regPoint->setPoint(spos, 30);//  默认一个宽度
			}
			m_regPoint->setSubPoint(spos);


			// 操作完成，设置为AS_ActionFinished
			m_status = xDef::AS_ActionFinished;
			e->accept();
			break;
		default:
			break;
		}
	}
}

void xActionDrawRegPoint::mouseMoveEvent(QMouseEvent* e)
{
}

void xActionDrawRegPoint::mouseReleaseEvent(QMouseEvent* e)
{
}

void xActionDrawRegPoint::cancel()
{
	if (m_regPoint)
	{
		m_scene->removeItem(m_regPoint);
		delete m_regPoint;
		m_regPoint = nullptr;
	}
	m_status = xDef::AS_Default;
}
