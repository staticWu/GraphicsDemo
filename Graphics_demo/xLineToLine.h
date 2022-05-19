#pragma once

#include <QGraphicsItem>
#include <QBrush>
#include "xTypeDef.h"
#include "xEntity.h"
#include <qvector2d.h>

class xLineToLine : public xEntity
{
	Q_OBJECT
public:
	Q_DISABLE_COPY(xLineToLine)
	explicit xLineToLine(xGraphicView* view, QGraphicsItem* parent = nullptr, qreal w = 20);
	virtual ~xLineToLine() = default;
	// 自定义实体类型枚举
	enum { Type = ET_InterLineAndLine};
	// 重写函数，返回当前的类型值
	int type() const override;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	void moveBy(const QPointF & delta) override;
	void moveBy2Point(const QPointF& p_after, const QPointF& p_before);
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF & pt, const QPointF & movedPt) override;
	bool isCtrlPoint(const QPointF & p) const override;
	bool isFittingEntity(const QPointF & p) override;

	void setLineWidth(qreal w);// 设置线区域宽
	void setFirstLineWidth(qreal w);// 设置线区域宽
	void setSecondLineWidth(qreal w);// 设置线区域宽
	void setFirstLine(const QLineF& line);// 设置第一条线
	void setFirstLine(const QPointF& p1, const QPointF& p2);// 设置第一条线
	void setFirstLinePt1(const QPointF& p);// 设置第一条线的第一个点
	void setFirstLinePt2(const QPointF& p);// 设置第一条线的第二个点

	void setFirstLineEntity(const QLineF& line);// 设置第一条线
	void setFirstLineEntity(const QPointF& p1, const QPointF& p2);// 设置第一条线
	void setFirstLineEntityPt1(const QPointF& p);// 设置第一条线的第一个点
	void setFirstLineEntityPt2(const QPointF& p);// 设置第一条线的第二个点

	void setSecondLine(const QLineF& line);// 设置第二条线
	void setSecondLine(const QPointF& p1, const QPointF& p2);// 设置第二条线
	void setSecondLinePt1(const QPointF& p);// 设置第二条线的第一个点
	void setSecondLinePt2(const QPointF& p);// 设置第二条线的第二个点

	void setSecondLineEntity(const QLineF& line);// 设置第二条线
	void setSecondLineEntity(const QPointF& p1, const QPointF& p2);// 设置第二条线
	void setSecondLineEntityPt1(const QPointF& p);// 设置第二条线的第一个点
	void setSecondLineEntityPt2(const QPointF& p);// 设置第二条线的第二个点

	void setMousePos(const QPointF& p);// 设置文字鼠标点
	void setMyText(const QString& text);// 设置文字

private:
	QLineF line_1_;// 第一条线
	qreal line_1_width_;// 第一条线的宽度
	QVector2D line_1_ver_;// 第一条线的垂直方向
	QLineF entity_line_1_;// 第一条线实体
	bool is_line_1_valid_ = false;// 记录第一条线是否有效
	QLineF line_2_;// 第二条线
	qreal line_2_width_;// 第二条线宽度
	QLineF entity_line_2_;// 第二条线实体
	bool is_line_2_valid_ = false;// 记录第二条线是否有效
	QVector2D line_2_ver_;// 第二条线的垂直方向

	QPointF mouse_pos_;// 鼠标位置点
	QString m_text_="dada";// 文字
	bool is_text_valid_ = false;
	QLineF m_line_1_;
	QLineF m_line_2_;
	QLineF m_line_3_;
	QVector2D slope_;// 中线方向向量
	QVector2D ver_;// 与方向向量垂直方向
	float angle_ = 0.0;// 记录文字需要旋转的角度

	bool is_move_able = false;

	void updatePosition();// 更新位置
	void getLineVer();// 更新线垂直方向位置
};

