#pragma once
#include "xRegionEntity.h"
#include "xPoint.h"

/**
 * @brief �̳�xRegionEntity�Ĵ���Χ��
*/
class xRegPoint :public xRegionEntity
{
	Q_OBJECT

public:
	Q_DISABLE_COPY(xRegPoint)
		explicit xRegPoint(xGraphicView* view, QGraphicsItem* parent = nullptr);
	xRegPoint(const QPoint& p, qreal width, xGraphicView* view, QGraphicsItem* parent = nullptr);
	~xRegPoint();

	// �Զ���ʵ������ö��
	enum { Type = ET_RegPoint };
	// ��д���������ص�ǰ������ֵ
	int type() const override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	// �������ɵ��޷�Χ�ĵ�
	QPointF subPointData() const { return m_subPoint->pointData(); }
	// ���ö������ɵĵ�
	void setSubPoint(const QPointF& p);
	// ���ö������ɵĵ�
	void hideSubPoint(bool hide = true) { m_subPoint->setVisible(!hide); }

	QPointF pointData() const { return { m_subPoint->x(),m_subPoint->y() }; }
	void setPoint(const QPointF& p, qreal width);

	QPointF pt() const { return mapToScene(m_regPoint.center()); }
	void setPt(const QPointF& p);

	void setRadius(qreal radius);
	qreal radius() { return m_regPoint.radius(); };

	void moveBy(const QPointF& delta) override;
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF& p, const QPointF& movedPt) override;
	bool isCtrlPoint(const QPointF& p) const override;
	void changeEdgeByPoint(const QPointF& p) override;
	bool isRegionEdge(const QPointF& p) const override;
protected:
	// ��Χ�������
	xCircleData m_regPoint;
	// �������ɵĲ�����Χ�ĵ�
	xPoint* m_subPoint = nullptr;
	qreal minWidth = 0;
};

