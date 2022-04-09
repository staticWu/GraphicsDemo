#include "xPoint.h"
#include <QStyleOptionGraphicsItem>
#include <QDebug>
xPoint::xPoint(xGraphicView* view, QGraphicsItem* parent)
	:xEntity(view,parent)
{
	setStyle(xStyle::Measured);
}

xPoint::xPoint(const QPointF& p, xGraphicView* view, QGraphicsItem* parent)
	: xEntity(view, parent), m_point(p)
{
	setStyle(xStyle::Measured);
}

xPoint::~xPoint()
{
}

int xPoint::type() const
{
	return Type;
}

void xPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);

	auto style = m_style;
	const qreal f = viewScaleFactor();
	if (style != xStyle::NoStyle)
	{
		// ѡ��״̬
		if (option->state & QStyle::State_Selected)
		{
			style = xStyle::Selected;
		}
		// ��ͣ״̬
		if (option->state & QStyle::State_MouseOver)
		{
			if (style == xStyle::Selected)
				style = xStyle::HoverSelected;
			else
				style = xStyle::Hovered;
		}
		xStyle::makeStyle(style, &m_pen, nullptr, f);
	}
	// ��������߼���
	qreal w = 5.0 / f;
	QPointF startP1 = { m_point.x() - w ,m_point.y() - w };
	QPointF startP2 = { m_point.x() + w ,m_point.y() + w };
	QPointF endP1 = { m_point.x() + w ,m_point.y() - w };
	QPointF endP2 = { m_point.x() - w ,m_point.y() + w };
	m_line1 = { startP1 ,startP2 };
	m_line2 = { endP1 ,endP2 };

	m_pen.setStyle(Qt::SolidLine);// Ĭ��ѡ��������
	painter->setPen(m_pen);
	painter->drawLine(m_line1);
	painter->drawLine(m_line2);
}

QRectF xPoint::boundingRect() const
{
	if (m_point.isNull())
		return QRectF();
	// ����ͼ�����ӳ��еľ��Σ��������ʵĿ�ȣ������޷���ȷ��ʾ
	// Note�����ʿ������Ϊ2���Ա�����ױ�ѡ�� ����Ĭ��Ϊ2.5������������С���߾���ʱ�ͻ�ˢ�·�Χ����
	const qreal w = m_pen.widthF() * 2 + 5.0 / viewScaleFactor();
	return QRectF(m_point.x() - w, m_point.y() - w, w + w, w + w);
}

QPainterPath xPoint::shape() const
{
	QPainterPath path;
	if (m_point.isNull())
		return path;
	// Ūһ���Ȳ��ľ��ΰ�Χ�棬����ѡ��
	path.moveTo(m_line1.p1());
	path.lineTo(m_line1.p2());
	path.moveTo(m_line2.p1());
	path.lineTo(m_line2.p2());

	return StrokeShapeFromPath(path, m_pen.widthF() * 2);
}

void xPoint::setPt(const QPointF& p)
{
	auto sp = mapFromScene(p);
	if (m_point == sp)
		return;
	prepareGeometryChange();
	m_point.setX(p.x());
	m_point.setY(p.y());


	update();
}

void xPoint::moveBy(const QPointF& delta)
{
	if (delta.isNull())
		return;

	prepareGeometryChange();
	m_point += delta;
	update();
}

QList<QPointF> xPoint::controlPoints() const
{
	return {pt()};
}


void xPoint::moveCtrlPoint(const QPointF& p, const QPointF& movedPt)
{
	if (Distance(p, pt()) < DELTA_DIST / viewScaleFactor())
	{
		setPt(movedPt);
	}
}

bool xPoint::isCtrlPoint(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	return  Distance(p, pt()) < DELTA_DIST_2 / viewScaleFactor();
}
