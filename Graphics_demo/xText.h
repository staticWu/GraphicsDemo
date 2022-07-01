#pragma once

#include <QGraphicsItem>
#include <QBrush>
#include "xTypeDef.h"
#include "xEntity.h"
#include <qvector2d.h>

class xText : public xEntity
{
	Q_OBJECT

public:
	Q_DISABLE_COPY(xText)
		explicit xText(QPointF line_1, QPointF line_2, xGraphicView* view, QGraphicsItem* parent = nullptr);
	virtual ~xText() = default;
	// �Զ���ʵ������ö��
	enum { Type = ET_Line };
	// ��д���������ص�ǰ������ֵ
	int type() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	void moveBy(const QPointF& delta) override;
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF& pt, const QPointF& movedPt) override;
	bool isCtrlPoint(const QPointF& p) const override;
	bool isFittingEntity(const QPointF& p) override;

	void setMousePos(const QPointF& p);
	void setMyText(const QString& text);
	void updatePosition();// ����λ��

	void setMidPoint(QPointF l_1, QPointF l_2);

private:
	QPointF mouse_pos;// ���λ�õ�
	QString m_text;// ����
	QLineF m_line_1;
	QLineF m_line_2;
	QLineF m_line_3;
	QPointF line_1_mid;// ��һ���ߵ��е�
	QPointF line_2_mid;// �ڶ����ߵ��е�
	QVector2D slope;// ���߷�������
	QVector2D ver;// �뷽��������ֱ����
	float angle;// ��¼������Ҫ��ת�ĽǶ�
};

