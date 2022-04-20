#pragma once

#include "xActionPreviewInterface.h"
class xVerticalLine;
class xActionDrawVerticalLine : public xActionPreviewInterface
{

public:
	xActionDrawVerticalLine(xGraphicView* view);
	~xActionDrawVerticalLine();
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;

private:
	QPointF mp;
	QPointF mp2;

	xVerticalLine* m_line = nullptr;
};
