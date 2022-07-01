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
	// �Զ���ʵ������ö��
	enum { Type = ET_RegCircleDiameter};
	// ��д���������ص�ǰ������ֵ
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


	void setMousePos(const QPointF& p);// ������������
	void setMyText(const QString& text);// ��������
	void setIsSuccess(bool is_s);

	qreal getRadius(const QPointF& point1, const QPointF& point2, const QPointF& point3);
	//void sendFitting() override;
signals:
	void CircleChange(QPointF p1, QPointF p2, QPointF p3, int flag);
	void ComputeDistance(QPointF cen_1, qreal r_1);

private:
	qreal y_x_ratio_ = 1.0; //y���x��ı���
	QPointF circle_1_;// Բ��������
	QPointF circle_2_;
	QPointF circle_3_;
	QPointF circle_cen_;// Բ������
	qreal circle_rx_ = 0.0;// ��Բ��x����
	qreal circle_ry_ = 0.0;// ��Բ��y����
	qreal circle_width_;// Բ�Ŀ��
	QPointF entity_circle_cen_ = { 0.0,0.0 };// ʵ��Բ������
	qreal entity_circle_rx_ = 0.0;// ��Բ��x����
	qreal entity_circle_ry_ = 0.0;// ��Բ��y����
	bool is_circle_valid_ = false;// ��¼Բ�Ƿ���Ч
	bool is_first_cirle_p_1_ = false;
	bool is_first_cirle_p_2_ = false;
	bool is_first_cirle_p_3_ = false;
	QPointF circle_cen_dt_1;// ʵ�������Բ
	qreal r_dt_1 = 0.0;
	bool is_circle_success_ = false;

	QPointF circle_centerPt;

	QPointF mouse_pos_;// ���λ�õ�
	QString m_text_ = "dada";// ����
	bool is_text_valid_ = false;

	QLineF m_line; 
	QLineF m_diameter_line;
	QVector2D slope_;// ���߷�������
	QVector2D ver_;// �뷽��������ֱ����
	float angle_ = 0.0;// ��¼������Ҫ��ת�ĽǶ�

	bool is_move_able = false;
	double DistancePoint2Ellipse(const QPointF& p, const QPointF& e_cen, double rx, double ry);
	void updatePosition();// ����λ��
	void updateEllipse();// ����Բ
};

