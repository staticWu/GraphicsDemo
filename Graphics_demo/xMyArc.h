#pragma once

#include "xEntity.h"
class xMyArc  : public xEntity
{
	Q_OBJECT

public:
	Q_DISABLE_COPY(xMyArc)
	xMyArc(xGraphicView* view, QGraphicsItem* parent = nullptr);
	xMyArc(const xArcData& arc, xGraphicView* view, QGraphicsItem* parent = nullptr);
	xMyArc(const QPointF& center, qreal radius, qreal startAngle, qreal spanAngle, xGraphicView* view, QGraphicsItem* parent = nullptr);
	xMyArc(const QPointF& p1, const QPointF& p2, const QPointF& p3, xGraphicView* view, QGraphicsItem* parent = nullptr);
	~xMyArc();

	// 自定义实体类型枚举
	enum { Type = ET_Arc};
	// 重写函数，返回当前的类型值
	int type() const override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	xArcData circleData() const { return m_arc; }
	void setArc(const xArcData& arc) { setArc(arc.center(), arc.radius(), arc.startAngle(),arc.spanAngle()); }
	void setArc(const QPointF& center, qreal radius, qreal startAngle, qreal spanAngle);
	void setArc(const QPointF& p1, const QPointF& p2, const QPointF& p3);

	QPointF center() const { return m_arc.center(); }
	void setCenter(const QPointF& center) { moveBy(center - this->center()); }
	qreal radius() const { return m_arc.radius(); }
	void setRadius(qreal radius);

	QPointF pt1() const { return m_arc.pt1(); }
	QPointF pt2() const { return m_arc.pt2(); }
	QPointF pt3() const { return m_arc.pt3(); }
	void setPt1(const QPointF& p);
	void setPt2(const QPointF& p);
	void setPt3(const QPointF& p);

	qreal startAngle() const { return m_arc.startAngle(); }
	qreal spanAngle() const { return m_arc.spanAngle(); }
	void setStartAngle(qreal startAngle);
	void setSpanAngle(qreal spanAngle);

	void moveBy(const QPointF& delta) override;
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF& pt, const QPointF& movedPt) override;
	bool isCtrlPoint(const QPointF& p) const override;
	bool isFittingEntity(const QPointF& p) override;
	QPolygonF drawArcPath( QPointF c, qreal radius, qreal startAngle, qreal endAngle, bool reversed) const;
	bool pointIsOnArc(QPointF pt);
protected:
	xArcData m_arc;
};
