#pragma once
#include "xEntity.h"
/**
 * @brief 继承于xEntity的直线
*/

class xStraightLine : public xEntity
{
	Q_OBJECT

public:
	Q_DISABLE_COPY(xStraightLine)
	explicit xStraightLine(xGraphicView* view, QGraphicsItem* parent = nullptr);
	xStraightLine(const QPointF& p1, const QPointF& p2, xGraphicView* view, QGraphicsItem* parent = nullptr);
	xStraightLine(const QLineF& line, xGraphicView* view, QGraphicsItem* parent = nullptr);	
	~xStraightLine();

	// 自定义实体类型枚举
	enum { Type = ET_StraightLine};
	// 重写函数，返回当前的类型值
	int type() const override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	QLineF lineData() const { return m_line; }
	void setLine(const QLineF& line) { setLine(line.p1(), line.p2()); }
	void setLine(const QPointF& p1, const QPointF& p2);

	QPointF anchorPoint1() const { return mapToScene(m_point1); }; // 定位点
	QPointF anchorPoint2() const { return mapToScene(m_point2); };

	QPointF pt1() const { return mapToScene(m_line.p1()); };// 直线的顶点
	QPointF pt2() const { return mapToScene(m_line.p2()); };

	void setPt1(const QPointF& p);
	void setPt2(const QPointF& p);

	void setAnchorPoint1(const QPointF& p);
	void setAnchorPoint2(const QPointF& p);

	void moveBy(const QPointF& delta) override;
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF& p, const QPointF& movedPt) override;
	bool isCtrlPoint(const QPointF& p) const override;
	bool isFittingEntity(const QPointF& p) override;
	void calStraighLinePoints();

signals:
	void pointChange();
protected:
	QPointF m_point1;// 定位点1
	QPointF m_point2;// 定位点2
	QLineF m_line; // 直线
};
