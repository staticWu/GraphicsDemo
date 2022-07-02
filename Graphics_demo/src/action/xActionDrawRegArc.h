#pragma once
#include "xActionPreviewInterface.h"
#include <QPoint>
class xRegArc;
class xPoint;
class xActionDrawRegArc  : public xActionPreviewInterface
{
public:
	xActionDrawRegArc(xGraphicView* view);
	~xActionDrawRegArc();
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;

private:
	QPointF mp1, mp2;
	xRegArc* m_arc = nullptr;
	xPoint* m_point1 = nullptr;
	xPoint* m_point2 = nullptr;
};
