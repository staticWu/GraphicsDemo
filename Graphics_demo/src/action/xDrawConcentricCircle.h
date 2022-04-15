#pragma once

#include <QObject>
#include <QPoint>
#include "xActionPreviewInterface.h"
class xConcentricCircle;
class xPoint;
class xDrawConcentricCircle : public xActionPreviewInterface
{

public:
	xDrawConcentricCircle(xGraphicView* view);
	~xDrawConcentricCircle();
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;

private:
	QPointF mp1, mp2;
	xConcentricCircle* m_circle = nullptr;
	xPoint* m_point1 = nullptr;
	xPoint* m_point2 = nullptr;
};
