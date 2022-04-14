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
	QPointF mp;// ����������λ�㣬���ݶ�λ�����ֱ�������˵�
	QPointF mp2;
	xStraightLine* m_line = nullptr;
};
