#pragma once

#include "xActionPreviewInterface.h"

class xRegPoint;
/*
* ���Ԥ���� 
*/
class xActionDrawPoint : public xActionPreviewInterface
{
public:
	xActionDrawPoint(xGraphicView* view);
	~xActionDrawPoint();
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;

private:
	xRegPoint* m_regPoint = nullptr;
};
