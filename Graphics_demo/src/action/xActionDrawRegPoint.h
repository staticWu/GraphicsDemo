#pragma once

#include "xActionPreviewInterface.h"

class xRegPoint;
/*
* µãµÄÔ¤ÀÀ²ã 
*/
class xActionDrawRegPoint : public xActionPreviewInterface
{
public:
	xActionDrawRegPoint(xGraphicView* view);
	~xActionDrawRegPoint();
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;

private:
	xRegPoint* m_regPoint = nullptr;
};
