#pragma once

#include "xActionPreviewInterface.h"
#include <qobject.h>

class xLineToLine;

class xActionDrawLineToLine2 :public QObject, public xActionPreviewInterface
{
	Q_OBJECT
public:
	xActionDrawLineToLine2(xGraphicView* view);
	~xActionDrawLineToLine2();

	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;


private:
	QPointF mp;
	xLineToLine* line_line = nullptr;
};

