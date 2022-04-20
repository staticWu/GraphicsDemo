#pragma once

#include "xEntity.h"
#include <QLineF>
#include "xStraightLine.h"

/**
 * @brief �̳���xEntity���޷�Χ��ֱ��
*/

class xVerticalLine : public xEntity
{
	Q_OBJECT

public:
	Q_DISABLE_COPY(xVerticalLine)
	explicit xVerticalLine(xGraphicView* view,QGraphicsItem* item = nullptr);
	~xVerticalLine();

	// �Զ���ʵ������ö��
	enum { Type = ET_VerticalLine};
	// ��д���������ص�ǰ������ֵ
	int type() const override;

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	
	QLineF straightLineData() const { return m_SLine->lineData(); }
	void setStraightLine(const QLineF& line) { setLine(line.p1(), line.p2()); }
	void setStraightLine(const QPointF& p1, const QPointF& p2);

	QLineF lineData() const { return m_line; }
	void setLine(const QLineF& line) { setLine(line.p1(), line.p2()); }
	void setLine(const QPointF& p1, const QPointF& p2);

	QPointF pt1() const { return m_SLine->anchorPoint1(); }  // ��һ�����ϵ�����
	QPointF pt2() const { return m_SLine->anchorPoint2(); }
	void setPt1(const QPointF& p);
	void setPt2(const QPointF& p);
	void setPt3(const QPointF& p);

	void moveBy(const QPointF& delta) override;
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF& pt, const QPointF& movedPt) override;
	bool isCtrlPoint(const QPointF& p) const override;
	bool isFittingEntity(const QPointF& p) override;
	void calVLine();
protected:
	QLineF m_line;
	xStraightLine* m_SLine = nullptr; // ���ڸ�����
	QPointF m_point;// �ڶ����ߵĶ�λ��

};
