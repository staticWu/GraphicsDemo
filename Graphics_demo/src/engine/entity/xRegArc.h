#pragma once

#include "xRegionEntity.h"
#include "xMyArc.h"
class xRegArc  : public xRegionEntity
{
	Q_OBJECT

public:
	Q_DISABLE_COPY(xRegArc)
	xRegArc(xGraphicView* view, QGraphicsItem* parent = nullptr);
	xRegArc(const xArcData& arc, qreal width, xGraphicView* view, QGraphicsItem* parent = nullptr);
	xRegArc(const QPointF& center, qreal radius, qreal startAngle, qreal spanAngle, qreal width, xGraphicView* view, QGraphicsItem* parent = nullptr);
	xRegArc(const QPointF& p1, const QPointF& p2, const QPointF& p3, qreal width, xGraphicView* view, QGraphicsItem* parent = nullptr);
	~xRegArc();

	// 自定义实体类型枚举
	enum { Type = ET_RegArc};
	// 重写函数，返回当前的类型值
	int type() const override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	xArcData subArcData() const { return m_fittingArc->arcData(); }
	void setSubArc(const QPointF& p1, const QPointF& p2, const QPointF& p3);
	void setSubArc(const QPointF& center, qreal radius, qreal startAngle, qreal spanAngle);
	void hideSubCircle(bool hide = true) { m_fittingArc->setVisible(!hide); }

	xArcData arcData() const { return m_midArc->arcData(); }
	void setArc(const xArcData& arc,qreal width) { setArc(arc.center(), arc.radius(), arc.startAngle(), arc.spanAngle(), width); }
	void setArc(const QPointF& center, qreal radius, qreal startAngle, qreal spanAngle,qreal width);
	void setArc(const QPointF& p1, const QPointF& p2, const QPointF& p3, qreal width);

	QPointF center() const { return m_midArc->center(); }
	void setCenter(const QPointF& center) { moveBy(center - this->center()); }
	qreal radius() const { return m_midArc->radius(); }
	void setRadius(qreal radius);

	QPointF pt1() const { return m_midArc->pt1(); }
	QPointF pt2() const { return m_midArc->pt2(); }
	QPointF pt3() const { return m_midArc->pt3(); }
	void setPt1(const QPointF& p);
	void setPt2(const QPointF& p);
	void setPt3(const QPointF& p);

	qreal startAngle() const { return m_midArc->startAngle(); }
	qreal spanAngle() const { return m_midArc->spanAngle(); }
	void setStartAngle(qreal startAngle);
	void setSpanAngle(qreal spanAngle);
	void moveBy(const QPointF& delta) override;
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF& pt, const QPointF& movedPt) override;
	bool isCtrlPoint(const QPointF& p) const override;
	void changeEdgeByPoint(const QPointF& p) override;
	bool isRegionEdge(const QPointF& p) const override;
	void updateRegArc();
	QPolygonF drawArcPath(QPointF c, qreal radius, qreal startAngle, qreal endAngle, bool reversed) const;
	QPainterPath fillAnnular(const QPointF& c,qreal r,qreal w,qreal startAngle,double spanAngle)const;
private:
	// 范围弧的数据
	xMyArc* m_midArc = nullptr;
	xMyArc* m_outArc = nullptr;
	xMyArc* m_inArc = nullptr;
	xMyArc* m_fittingArc = nullptr;
	
};
