#pragma once

#include "xActionPreviewInterface.h"
#include <QPoint>

class xCircle;
class xPoint;
class xActionDrawCircle : public xActionPreviewInterface
{

public:
	xActionDrawCircle(xGraphicView* view);
	~xActionDrawCircle();
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;

private:
	QPointF mp1, mp2;
	xCircle* m_circle = nullptr;
	xPoint* m_point1 = nullptr;
	xPoint* m_point2 = nullptr;
};
