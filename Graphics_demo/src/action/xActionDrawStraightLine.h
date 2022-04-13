#pragma once

#include "xActionPreviewInterface.h"
#include <QPoint>
class xStraightLine;
class xActionDrawStraightLine : public xActionPreviewInterface
{

public:
	xActionDrawStraightLine(xGraphicView* view);
	~xActionDrawStraightLine();

	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;
	void calStraighLinePoints();
private:
	QPointF mp;// 定位点
	QPointF mp2;
	QPointF pt1;// 直线的两个端点
	QPointF pt2;
	xStraightLine* m_line = nullptr;
};
