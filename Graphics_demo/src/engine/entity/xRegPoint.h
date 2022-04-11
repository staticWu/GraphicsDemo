#pragma once
#include "xRegionEntity.h"
#include "xPoint.h"

/**
 * @brief 继承xRegionEntity的带范围点
*/
class xRegPoint :public xRegionEntity
{
	Q_OBJECT

public:
	Q_DISABLE_COPY(xRegPoint)
		explicit xRegPoint(xGraphicView* view, QGraphicsItem* parent = nullptr);
	xRegPoint(const QPoint& p, qreal width, xGraphicView* view, QGraphicsItem* parent = nullptr);
	~xRegPoint();

	// 自定义实体类型枚举
	enum { Type = ET_RegPoint };
	// 重写函数，返回当前的类型值
	int type() const override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	// 额外生成的无范围的点
	QPointF subPointData() const { return m_subPoint->pointData(); }
	// 设置额外生成的点
	void setSubPoint(const QPointF& p);
	// 设置额外生成的点
	void hideSubPoint(bool hide = true) { m_subPoint->setVisible(!hide); }

	QPointF pointData() const { return { m_subPoint->x(),m_subPoint->y() }; }
	void setPoint(const QPointF& p, qreal width);

	QPointF pt() const { return mapToScene(m_regPoint.center()); }
	void setPt(const QPointF& p);

	void setRadius(qreal radius);
	qreal radius() { return m_regPoint.radius(); };

	void moveBy(const QPointF& delta) override;
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF& p, const QPointF& movedPt) override;
	bool isCtrlPoint(const QPointF& p) const override;
	void changeEdgeByPoint(const QPointF& p) override;
	bool isRegionEdge(const QPointF& p) const override;
protected:
	// 范围点的数据
	xCircleData m_regPoint;
	// 额外生成的不带范围的点
	xPoint* m_subPoint = nullptr;
	qreal minWidth = 0;
};

