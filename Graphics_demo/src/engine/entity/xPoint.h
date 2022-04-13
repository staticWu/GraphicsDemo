#pragma once

#include "xEntity.h"
/**
 * @brief �̳���xEntity���޷�Χ��
*/
class xPoint : public xEntity
{
	Q_OBJECT

public:
	Q_DISABLE_COPY(xPoint)
	explicit xPoint(xGraphicView* view, QGraphicsItem* parent = nullptr);
	xPoint(const QPointF& p, xGraphicView* view, QGraphicsItem* parent = nullptr);
	~xPoint();

	// �Զ���ʵ������ö��
	enum { Type = ET_Point };
	// ��д���������ص�ǰ������ֵ
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
	QLineF m_line1; // һ������һ�����ʾ ��1��ʾ���Ͻǵ����½� ��2��ʾ���Ͻǵ����½�
	QLineF m_line2;
};
