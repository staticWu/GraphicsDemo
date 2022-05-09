#pragma once

#include "xActionPreviewInterface.h"

class xRegLine;
class xText;

class xActionDrawLineToLine : public xActionPreviewInterface
{
public:
	xActionDrawLineToLine(xGraphicView* view);
	~xActionDrawLineToLine();

	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;

private:
	QPointF mp;
	xRegLine* m_line_1 = nullptr;
	xRegLine* m_line_2 = nullptr;
	xText* m_text = nullptr;
};

