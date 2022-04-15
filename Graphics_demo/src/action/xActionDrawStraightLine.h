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
private:
	QPointF mp;// 传入两个定位点，根据定位点计算直线
	QPointF mp2;
	xStraightLine* m_line = nullptr;
};
