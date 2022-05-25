#pragma once

#include "xActionPreviewInterface.h"
#include <qobject.h>

class xCircleToLine;

class xActionDrawCircleToLine :public QObject, public xActionPreviewInterface
{
	Q_OBJECT
public:
	xActionDrawCircleToLine(xGraphicView* view);
	~xActionDrawCircleToLine();

	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;


private:
	QPointF mp;
	xCircleToLine* circle_line = nullptr;
};



