#pragma once

#include "xActionPreviewInterface.h"
#include <qobject.h>

class xRegLine;
class xText;

class xActionDrawLineToLine :public QObject ,public xActionPreviewInterface
{
	Q_OBJECT
public:
	xActionDrawLineToLine(xGraphicView* view);
	~xActionDrawLineToLine();

	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;

	void getLineChanged(QPointF p);

private:
	QPointF mp;
	xRegLine* m_line_1 = nullptr;
	xRegLine* m_line_2 = nullptr;
	xText* m_text = nullptr;
};

