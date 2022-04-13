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
	QPointF mp;// ��λ��
	QPointF mp2;
	QPointF pt1;// ֱ�ߵ������˵�
	QPointF pt2;
	xStraightLine* m_line = nullptr;
};
