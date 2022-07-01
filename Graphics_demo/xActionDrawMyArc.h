#pragma once

#include "xActionPreviewInterface.h"
class xMyArc;
class xPoint;
class xActionDrawMyArc  : public xActionPreviewInterface
{

public:
	xActionDrawMyArc(xGraphicView* view);
	~xActionDrawMyArc();
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;

private:
	QPointF mp1, mp2;
	xPoint* m_point1 = nullptr;
	xPoint* m_point2 = nullptr;
	xMyArc* m_arc = nullptr;

};
