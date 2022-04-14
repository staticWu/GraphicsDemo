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
		case xDef::S_Default:
			if (m_regPoint == nullptr)
			{
				m_regPoint = new xRegPoint(m_view);
				m_regPoint->setStyle(xStyle::RegDrawn);
				m_scene->addItem(m_regPoint);
				m_regPoint->setRadius(5); // Ĭ�ϰ뾶
				m_regPoint->setPoint(spos, 30);//  Ĭ��һ�����
			}
			m_regPoint->setSubPoint(spos);


			// ������ɣ�����ΪS_ActionFinished
			m_status = xDef::S_ActionFinished;
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
	m_status = xDef::S_Default;
}
