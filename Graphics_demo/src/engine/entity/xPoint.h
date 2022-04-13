#pragma once

#include "xEntity.h"
/**
 * @brief 继承于xEntity的无范围点
*/
class xPoint : public xEntity
{
	Q_OBJECT

public:
	Q_DISABLE_COPY(xPoint)
	explicit xPoint(xGraphicView* view, QGraphicsItem* parent = nullptr);
	xPoint(const QPointF& p, xGraphicView* view, QGraphicsItem* parent = nullptr);
	~xPoint();

	// 自定义实体类型枚举
	enum { Type = ET_Point };
	// 重写函数，返回当前的类型值
	int type() const override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	QPointF pointData() const { return m_point; }
	QPointF pt() const { return mapToScene(m_point); };
	void setPt(const QPointF& p);
	void moveBy(const QPointF& delta) override;
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF& p, const QPointF& movedPt) override;
	bool isCtrlPoint(const QPointF& p) const override;
	bool isFittingEntity(const QPointF& p) override;
protected:
	QPointF m_point;
	QLineF m_line1; // 一个点用一个叉表示 线1表示左上角到右下角 线2表示右上角到左下角
	QLineF m_line2;
};
