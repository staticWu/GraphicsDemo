#include "xActionDrawPoint.h"
#include "xRegPoint.h"
#include <QMouseEvent>
xActionDrawPoint::xActionDrawPoint(xGraphicView* view)
	: xActionPreviewInterface(view, xDef::AT_DrawPoint)
{
}

xActionDrawPoint::~xActionDrawPoint()
{
	if (!isFinished())
		cancel();
}

void xActionDrawPoint::mousePressEvent(QMouseEvent* e)
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

void xActionDrawPoint::mouseMoveEvent(QMouseEvent* e)
{
}

void xActionDrawPoint::mouseReleaseEvent(QMouseEvent* e)
{
}

void xActionDrawPoint::cancel()
{
	if (m_regPoint)
	{
		m_scene->removeItem(m_regPoint);
		delete m_regPoint;
		m_regPoint = nullptr;
	}
	m_status = xDef::S_Default;
}
