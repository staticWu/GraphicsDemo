#pragma once

#include "xEntity.h"
#include <QPointF>
class xConcentricCircle : public xEntity
{
	Q_OBJECT

public:
	xConcentricCircle(xGraphicView* view, QGraphicsItem* parent = nullptr);
	~xConcentricCircle();
	// 自定义实体类型枚举
	enum { Type = ET_ConcentricCircle};
	// 重写函数，返回当前的类型值
	int type() const override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	xCircleData circleData() const { return m_circle; }
	xCircleData thirdCircleData() const { return m_thirdCircle; }
	void setThirdCircle(const xCircleData& circle) { setCircle(circle.center(), circle.radius()); }
	void setThirdCircle(const QPointF& center, qreal radius);
	void setThirdCircle(const QPointF& p1, const QPointF& p2, const QPointF& p3);

	void setCircle(const QPointF& center, qreal radius);

	QPointF center() const { return m_circle.center(); }
	void setCenter(const QPointF& center) { moveBy(center - this->center()); }
	qreal radius() const { return m_circle.radius(); }
	void setRadius(qreal radius);
	qreal thirdCircleradius() const { return m_thirdCircle.radius(); }
	void setThirdCircleRadius(qreal radius);

	QPointF pt1() const { return m_thirdCircle.pt1(); }
	QPointF pt2() const { return m_thirdCircle.pt2(); }
	QPointF pt3() const { return m_thirdCircle.pt3(); }
	QPointF pt4() const { return m_ctrlPoint; }
	void setPt1(const QPointF& p);
	void setPt2(const QPointF& p);
	void setPt3(const QPointF& p);
	void setPt4(const QPointF& p);

	void moveBy(const QPointF& delta) override;
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF& pt, const QPointF& movedPt) override;
	bool isCtrlPoint(const QPointF& p) const override;
	bool isFittingEntity(const QPointF& p) override;
	QPointF calCirclePoint(const QPointF &p);// 重新计算控制点位置

protected:
	// 三点拟合定位圆心的圆
	xCircleData m_thirdCircle;
	// 圆心加一点生成的圆
	xCircleData m_circle;
	QPointF m_ctrlPoint ;
};
