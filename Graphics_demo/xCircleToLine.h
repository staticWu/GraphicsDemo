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
	// �Զ���ʵ������ö��
	enum { Type = ET_InterLineAndCircle};
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
	void setFirstCircleWidth(qreal w);
	void setSecondLineWidth(qreal w);// �����������

	void setFirstCirclePt1AndPt2(const QPointF& p1, const QPointF& p2);
	void setFirstCirclePt1(const QPointF& p);
	void setFirstCirclePt2(const QPointF& p);
	void setFirstCirclePt3(const QPointF& p);

	void setSecondLine(const QLineF& line);// ���õڶ�����
	void setSecondLine(const QPointF& p1, const QPointF& p2);// ���õڶ�����
	void setSecondLinePt1(const QPointF& p);// ���õڶ����ߵĵ�һ����
	void setSecondLinePt2(const QPointF& p);// ���õڶ����ߵĵڶ�����

	void setSecondLineEntity(const QLineF& line);// ���õڶ�����
	void setSecondLineEntity(const QPointF& p1, const QPointF& p2);// ���õڶ�����
	void setSecondLineEntityPt1(const QPointF& p);// ���õڶ����ߵĵ�һ����
	void setSecondLineEntityPt2(const QPointF& p);// ���õڶ����ߵĵڶ�����

	void setMousePos(const QPointF& p);// ������������
	void setMyText(const QString& text);// ��������

private:

	QPointF circle_1_;// Բ��������
	QPointF circle_2_;
	QPointF circle_3_;
	qreal y_x_ratio_ = 1.0; //y���x��ı���
	QPointF circle_cen_;// Բ������
	qreal circle_rx_ = 0.0;// ��Բ��x����
	qreal circle_ry_ = 0.0;// ��Բ��y����
	qreal circle_width_;// Բ�Ŀ��
	QPointF entity_circle_cen_ = { 0.0,0.0 };// ʵ��Բ������
	qreal entity_circle_rx_ = 0.0;// ��Բ��x����
	qreal entity_circle_ry_ = 0.0;// ��Բ��y����
	bool is_circle_valid_ = false;// ��¼�ڶ������Ƿ���Ч
	bool is_first_cirle_p_1_ = false;
	bool is_first_cirle_p_2_ = false;
	bool is_first_cirle_p_3_ = false;

	QLineF line_2_;// �ڶ�����
	qreal line_2_width_;// �ڶ����߿��
	QLineF entity_line_2_;// �ڶ�����ʵ��
	bool is_line_2_valid_ = false;// ��¼�ڶ������Ƿ���Ч
	QVector2D line_2_ver_;// �ڶ����ߵĴ�ֱ����

	QPointF mouse_pos_;// ���λ�õ�
	QString m_text_ = "dada";// ����
	bool is_text_valid_ = false;
	QLineF m_line_1_;
	QLineF m_line_2_;
	QLineF m_line_3_;
	QVector2D slope_;// ���߷�������
	QVector2D ver_;// �뷽��������ֱ����
	float angle_ = 0.0;// ��¼������Ҫ��ת�ĽǶ�

	bool is_move_able = false;

	double DistancePoint2Ellipse(const QPointF& p, const QPointF& e_cen, double rx, double ry);
	void updatePosition();// ����λ��
	void updateEllipse();// ����Բ
	void getLineVer();// �����ߴ�ֱ����λ��
};

