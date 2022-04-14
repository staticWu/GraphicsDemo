#pragma once

#include "xActionPreviewInterface.h"
#include <QPoint>
class xStraightLine;
class xRegLine;

class xActionDrawRegStraightLine : public xActionPreviewInterface
{

public:
	xActionDrawRegStraightLine(xGraphicView* view);
	~xActionDrawRegStraightLine();
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;
private:
	QPointF mp;// 传入两个定位点，根据定位点计算直线两个端点
	QPointF mp2;
	xStraightLine* m_line = nullptr;
};
