#pragma once

#include "xActionPreviewInterface.h"

class xRegStraightLine;

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
	QPointF mp;// ����������λ�㣬���ݶ�λ�����ֱ�������˵�
	QPointF mp2;
	xRegStraightLine* m_line = nullptr;
};
