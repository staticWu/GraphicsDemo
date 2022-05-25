#pragma once

#include <QGraphicsItem>
#include <QBrush>
#include "xTypeDef.h"
#include "xEntity.h"
#include <qvector2d.h>

class xCircleToLine : public xEntity
{
	Q_OBJECT
public:
	Q_DISABLE_COPY(xCircleToLine)
	explicit xCircleToLine(xGraphicView* view, QGraphicsItem* parent = nullptr, qreal w = 10);
	virtual ~xCircleToLine() = default;
	// 自定义实体类型枚举
	enum { Type = ET_InterLineAndCircle};
	// 重写函数，返回当前的类型值
	int type() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	void moveBy(const QPointF& delta) override;
	void moveBy2Point(const QPointF& p_after, const QPointF& p_before);
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF& pt, const QPointF& movedPt) override;
	bool isCtrlPoint(const QPointF& p) const override;
	bool isFittingEntity(const QPointF& p) override;

	void setTwoWidth(qreal w);
	void setFirstCircleWidth(qreal w);
	void setSecondLineWidth(qreal w);// 设置线区域宽

	void setFirstCirclePt1AndPt2(const QPointF& p1, const QPointF& p2);
	void setFirstCirclePt1(const QPointF& p);
	void setFirstCirclePt2(const QPointF& p);
	void setFirstCirclePt3(const QPointF& p);

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

	QPointF circle_1_;// 圆的三个点
	QPointF circle_2_;
	QPointF circle_3_;
	qreal y_x_ratio_ = 1.0; //y轴和x轴的比例
	QPointF circle_cen_;// 圆的中心
	qreal circle_rx_ = 0.0;// 画圆的x半轴
	qreal circle_ry_ = 0.0;// 画圆的y半轴
	qreal circle_width_;// 圆的宽度
	QPointF entity_circle_cen_ = { 0.0,0.0 };// 实体圆的中心
	qreal entity_circle_rx_ = 0.0;// 画圆的x半轴
	qreal entity_circle_ry_ = 0.0;// 画圆的y半轴
	bool is_circle_valid_ = false;// 记录第二条线是否有效
	bool is_first_cirle_p_1_ = false;
	bool is_first_cirle_p_2_ = false;
	bool is_first_cirle_p_3_ = false;

	QLineF line_2_;// 第二条线
	qreal line_2_width_;// 第二条线宽度
	QLineF entity_line_2_;// 第二条线实体
	bool is_line_2_valid_ = false;// 记录第二条线是否有效
	QVector2D line_2_ver_;// 第二条线的垂直方向

	QPointF mouse_pos_;// 鼠标位置点
	QString m_text_ = "dada";// 文字
	bool is_text_valid_ = false;
	QLineF m_line_1_;
	QLineF m_line_2_;
	QLineF m_line_3_;
	QVector2D slope_;// 中线方向向量
	QVector2D ver_;// 与方向向量垂直方向
	float angle_ = 0.0;// 记录文字需要旋转的角度

	bool is_move_able = false;

	double DistancePoint2Ellipse(const QPointF& p, const QPointF& e_cen, double rx, double ry);
	void updatePosition();// 更新位置
	void updateEllipse();// 更新圆
	void getLineVer();// 更新线垂直方向位置
};

