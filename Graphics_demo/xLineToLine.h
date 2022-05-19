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
	// �Զ���ʵ������ö��
	enum { Type = ET_InterLineAndLine};
	// ��д���������ص�ǰ������ֵ
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

	void setLineWidth(qreal w);// �����������
	void setFirstLineWidth(qreal w);// �����������
	void setSecondLineWidth(qreal w);// �����������
	void setFirstLine(const QLineF& line);// ���õ�һ����
	void setFirstLine(const QPointF& p1, const QPointF& p2);// ���õ�һ����
	void setFirstLinePt1(const QPointF& p);// ���õ�һ���ߵĵ�һ����
	void setFirstLinePt2(const QPointF& p);// ���õ�һ���ߵĵڶ�����

	void setFirstLineEntity(const QLineF& line);// ���õ�һ����
	void setFirstLineEntity(const QPointF& p1, const QPointF& p2);// ���õ�һ����
	void setFirstLineEntityPt1(const QPointF& p);// ���õ�һ���ߵĵ�һ����
	void setFirstLineEntityPt2(const QPointF& p);// ���õ�һ���ߵĵڶ�����

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
	QLineF line_1_;// ��һ����
	qreal line_1_width_;// ��һ���ߵĿ��
	QVector2D line_1_ver_;// ��һ���ߵĴ�ֱ����
	QLineF entity_line_1_;// ��һ����ʵ��
	bool is_line_1_valid_ = false;// ��¼��һ�����Ƿ���Ч
	QLineF line_2_;// �ڶ�����
	qreal line_2_width_;// �ڶ����߿��
	QLineF entity_line_2_;// �ڶ�����ʵ��
	bool is_line_2_valid_ = false;// ��¼�ڶ������Ƿ���Ч
	QVector2D line_2_ver_;// �ڶ����ߵĴ�ֱ����

	QPointF mouse_pos_;// ���λ�õ�
	QString m_text_="dada";// ����
	bool is_text_valid_ = false;
	QLineF m_line_1_;
	QLineF m_line_2_;
	QLineF m_line_3_;
	QVector2D slope_;// ���߷�������
	QVector2D ver_;// �뷽��������ֱ����
	float angle_ = 0.0;// ��¼������Ҫ��ת�ĽǶ�

	bool is_move_able = false;

	void updatePosition();// ����λ��
	void getLineVer();// �����ߴ�ֱ����λ��
};

