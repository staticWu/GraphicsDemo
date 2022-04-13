#pragma once

#include "xActionPreviewInterface.h"
#include <QPoint>

class xRegCircle;
class xPoint;

class xActionDrawRegCircle : public xActionPreviewInterface
{
public:
	xActionDrawRegCircle(xGraphicView *view);
	~xActionDrawRegCircle();

	void mousePressEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;

	void cancel() override;

private:
	QPointF mp1, mp2;
	xRegCircle *m_circle = nullptr;
	xPoint *m_point1 = nullptr;
	xPoint *m_point2 = nullptr;
};
