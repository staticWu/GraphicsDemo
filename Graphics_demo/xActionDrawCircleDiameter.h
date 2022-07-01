#pragma once
#include "xActionPreviewInterface.h"
#include <qobject.h>
class xCircleDiameter;
class xActionDrawCircleDiameter :public QObject, public xActionPreviewInterface
{
	Q_OBJECT
public:
	xActionDrawCircleDiameter(xGraphicView* view,qreal y_x =1.0);
	~xActionDrawCircleDiameter();

	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;

	void cancel() override;

	void getCircle(QPointF cen, qreal r, QPointF cen_draw, QPointF cen_x_draw, QPointF cen_y_draw);
	void getDistance(QString t);
	void getIsSuccess(bool is_s);

signals:
	void CircleChange(QPointF p1, QPointF p2, QPointF p3, int flag);
	void ComputeDistance(QPointF cen_1, qreal r_1);

private:
	QPointF mp;
	xCircleDiameter* circle_diameter = nullptr;
	qreal y_x_ratio = 1.0;
};

