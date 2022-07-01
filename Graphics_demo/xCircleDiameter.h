#pragma once
#include <QGraphicsItem>
#include <QBrush>
#include "xTypeDef.h"
#include "xEntity.h"
#include <QVector2D>
class xCircleDiameter : public xEntity
{
	Q_OBJECT
public:
	Q_DISABLE_COPY(xCircleDiameter)

	explicit xCircleDiameter(xGraphicView* view, QGraphicsItem* parent = nullptr, qreal w = 10, qreal y_x = 1.0);
	virtual ~xCircleDiameter() = default;
	// 自定义实体类型枚举
	enum { Type = ET_RegCircleDiameter};
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
	void setCircleWidth(qreal w);
	void setCirclePt1(const QPointF& p);
	void setCirclePt2(const QPointF& p);
	void setCirclePt3(const QPointF& p);
	void setCircleEntity(QPointF cen_draw, QPointF cen_x_draw, QPointF cen_y_draw);
	void setCirclePt1AndPt2(const QPointF& p1, const QPointF& p2);

	void setMyCircle(QPointF cen, qreal r, QPointF cen_draw, QPointF cen_x_draw, QPointF cen_y_draw);


	void setMousePos(const QPointF& p);// 设置文字鼠标点
	void setMyText(const QString& text);// 设置文字
	void setIsSuccess(bool is_s);

	qreal getRadius(const QPointF& point1, const QPointF& point2, const QPointF& point3);
	//void sendFitting() override;
signals:
	void CircleChange(QPointF p1, QPointF p2, QPointF p3, int flag);
	void ComputeDistance(QPointF cen_1, qreal r_1);

private:
	qreal y_x_ratio_ = 1.0; //y轴和x轴的比例
	QPointF circle_1_;// 圆的三个点
	QPointF circle_2_;
	QPointF circle_3_;
	QPointF circle_cen_;// 圆的中心
	qreal circle_rx_ = 0.0;// 画圆的x半轴
	qreal circle_ry_ = 0.0;// 画圆的y半轴
	qreal circle_width_;// 圆的宽度
	QPointF entity_circle_cen_ = { 0.0,0.0 };// 实体圆的中心
	qreal entity_circle_rx_ = 0.0;// 画圆的x半轴
	qreal entity_circle_ry_ = 0.0;// 画圆的y半轴
	bool is_circle_valid_ = false;// 记录圆是否有效
	bool is_first_cirle_p_1_ = false;
	bool is_first_cirle_p_2_ = false;
	bool is_first_cirle_p_3_ = false;
	QPointF circle_cen_dt_1;// 实际坐标的圆
	qreal r_dt_1 = 0.0;
	bool is_circle_success_ = false;

	QPointF circle_centerPt;

	QPointF mouse_pos_;// 鼠标位置点
	QString m_text_ = "dada";// 文字
	bool is_text_valid_ = false;

	QLineF m_line; 
	QLineF m_diameter_line;
	QVector2D slope_;// 中线方向向量
	QVector2D ver_;// 与方向向量垂直方向
	float angle_ = 0.0;// 记录文字需要旋转的角度

	bool is_move_able = false;
	double DistancePoint2Ellipse(const QPointF& p, const QPointF& e_cen, double rx, double ry);
	void updatePosition();// 更新位置
	void updateEllipse();// 更新圆
};

