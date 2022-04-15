#pragma once

#include "xRegionEntity.h"
#include "xRegLine.h"
#include "xStraightLine.h"

class xRegStraightLine : public xRegionEntity
{
	Q_OBJECT

public:
	Q_DISABLE_COPY(xRegStraightLine)
	explicit xRegStraightLine(xGraphicView* view, QGraphicsItem* parent = nullptr);
	xRegStraightLine(const QLineF& line, qreal width, xGraphicView* view, QGraphicsItem* parent = nullptr);
	xRegStraightLine(const QPointF& p1, const QPointF& p2, qreal width, xGraphicView* view, QGraphicsItem* parent = nullptr);
	~xRegStraightLine();
	// �Զ���ʵ������ö��
	enum { Type = ET_RegStraightLine};
	// ��д���������ص�ǰ������ֵ
	int type() const override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	QLineF lineData() const { return m_straightLine->lineData(); }
	void setLine(const QLineF& line, qreal width) { setLine(line.p1(), line.p2(),width); }
	void setLine(const QPointF& p1, const QPointF& p2,qreal width);

	QPointF anchorPoint1() const { return m_regLine->pt1(); }; // ��λ��
	QPointF anchorPoint2() const { return m_regLine->pt2(); };

	QPointF pt1() const { return m_straightLine->pt1(); };// ֱ�ߵĶ���
	QPointF pt2() const { return m_straightLine->pt2(); };

	// ������Χ��ֱ��
	QLineF subLineData()const { return m_straightLine->lineData(); };
	void setSubLine(const QPointF& p1, const QPointF& p2);
	void setSubLine(const QLineF& line);
	void hideSubLine(bool hide = true) { m_straightLine->setVisible(!hide); }

	void setAnchorPoint1(const QPointF& p);
	void setAnchorPoint2(const QPointF& p);

	void moveBy(const QPointF& delta) override;
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF& p, const QPointF& movedPt) override;
	bool isCtrlPoint(const QPointF& p) const override;
	void changeEdgeByPoint(const QPointF& p) override;
	bool isRegionEdge(const QPointF& p) const override;
	QList<QPointF> calStraighLinePoints(const QPointF& p1, const QPointF& p2);
protected:
	xStraightLine* m_straightLine = nullptr; // ֱ��
	xRegLine* m_regLine = nullptr;// ����Χ����

};
