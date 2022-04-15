#include "xConcentricCircle.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
xConcentricCircle::xConcentricCircle(xGraphicView* view, QGraphicsItem* parent)
	:xEntity(view,parent)
{
}

xConcentricCircle::~xConcentricCircle()
{
}

int xConcentricCircle::type() const
{
	return Type;
}

void xConcentricCircle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);

	auto style = m_style;

	if (style != xStyle::NoStyle)
	{
		if (option->state & QStyle::State_Selected)
		{
			style = xStyle::Selected;
		}

		if (option->state & QStyle::State_MouseOver)
		{
			if (style == xStyle::Selected)
				style = xStyle::HoverSelected;
			else
				style = xStyle::Hovered;
		}

		const qreal f = viewScaleFactor();
		xStyle::makeStyle(style, &m_pen, nullptr, f);
	}

	painter->setPen(m_pen);
	painter->drawEllipse(m_thirdCircle.center(), m_thirdCircle.radius(), m_thirdCircle.radius());
	painter->drawEllipse(m_circle.center(), m_circle.radius(), m_circle.radius());

	if ((option->state & QStyle::State_Selected) && (flags() & ItemIsMovable))
	{
		const qreal w = m_pen.widthF();
		painter->fillRect(QRectF(m_thirdCircle.center().x() - w, m_thirdCircle.center().y() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_thirdCircle.pt1().x() - w, m_thirdCircle.pt1().y() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_thirdCircle.pt2().x() - w, m_thirdCircle.pt2().y() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_thirdCircle.pt3().x() - w, m_thirdCircle.pt3().y() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_ctrlPoint.x() - w, m_ctrlPoint.y() - w, w + w, w + w), Qt::yellow);
	}
}

QRectF xConcentricCircle::boundingRect() const
{
	if (m_circle.isNull())
		return QRectF();

	// 计算图形在视场中的矩形，包括画笔的宽度，否则无法正确显示
	// Note：画笔宽度设置为2倍以便更容易被选中
	qreal pw, x,y, w;
	pw = m_pen.widthF() * 2;
	bool max = m_circle.radius() > m_thirdCircle.radius();
	if (max)
	{
		x = m_circle.center().x() - m_circle.radius() - pw;
		y = m_circle.center().y() - m_circle.radius() - pw;
		w = m_circle.radius() + pw;
	}
	else
	{
		x = m_thirdCircle.center().x() - m_thirdCircle.radius() - pw;
		y = m_thirdCircle.center().y() - m_thirdCircle.radius() - pw;
		w = m_thirdCircle.radius() + pw;
	}
	
	return QRectF(x, y, w + w, w + w);
}

QPainterPath xConcentricCircle::shape() const
{
	QPainterPath path;
	if (m_circle.isNull() || m_thirdCircle.isNull())
		return path;

	path.addEllipse(m_thirdCircle.center(), m_thirdCircle.radius(), m_thirdCircle.radius());
	path.addEllipse(m_circle.center(), m_circle.radius(), m_circle.radius());
	return StrokeShapeFromPath(path, m_pen.widthF() * 2);
}

void xConcentricCircle::setCircle(const QPointF& center, qreal radius)
{
	if (center == m_circle.center() && qFuzzyCompare(radius, m_circle.radius()))
		return;

	prepareGeometryChange();
	m_circle = xCircleData(center, radius);
	update();
}

void xConcentricCircle::setThirdCircle(const QPointF& center, qreal radius)
{
	if (center == m_thirdCircle.center() && qFuzzyCompare(radius, m_thirdCircle.radius()))
		return;

	prepareGeometryChange();
	m_thirdCircle = xCircleData(center, radius);
	update();
}
void xConcentricCircle::setThirdCircle(const QPointF& p1, const QPointF& p2, const QPointF& p3)
{
	auto c = xCircleData(p1, p2, p3);
	if (c.center() == m_thirdCircle.center() && qFuzzyCompare(c.radius(), m_thirdCircle.radius()))
		return;

	prepareGeometryChange();
	m_thirdCircle = c;
	update();
}

void xConcentricCircle::setRadius(qreal radius)
{
	if (qFuzzyCompare(radius, m_circle.radius()))
		return;

	prepareGeometryChange();
	m_circle.setRadius(radius);
	update();
}

void xConcentricCircle::setThirdCircleRadius(qreal radius)
{
	if (qFuzzyCompare(radius, m_thirdCircle.radius()))
		return;

	prepareGeometryChange();
	m_thirdCircle.setRadius(radius);
	update();
}

void xConcentricCircle::setPt1(const QPointF& p)
{
	if (p == m_thirdCircle.pt1())
		return;

	prepareGeometryChange();
	m_thirdCircle = xCircleData(p, m_thirdCircle.pt2(), m_thirdCircle.pt3());
	// 圆心改变同心圆也改变
	m_circle = xCircleData(m_thirdCircle.center(), m_circle.radius());
	m_ctrlPoint = calCirclePoint(m_ctrlPoint);

	update();
}

void xConcentricCircle::setPt2(const QPointF& p)
{
	if (p == m_thirdCircle.pt2())
		return;

	prepareGeometryChange();
	m_thirdCircle = xCircleData(m_thirdCircle.pt1(), p, m_thirdCircle.pt3());
	// 圆心改变同心圆也改变
	m_circle = xCircleData(m_thirdCircle.center(),m_circle.radius());
	m_ctrlPoint = calCirclePoint(m_ctrlPoint);

	update();
}

void xConcentricCircle::setPt3(const QPointF& p)
{
	if (p == m_thirdCircle.pt3())
		return;

	prepareGeometryChange();
	m_thirdCircle = xCircleData(m_thirdCircle.pt1(), m_thirdCircle.pt2(), p);
	// 圆心改变同心圆也改变
	m_circle = xCircleData(m_thirdCircle.center(), m_circle.radius());
	m_ctrlPoint = calCirclePoint(m_ctrlPoint);

	update();
}

void xConcentricCircle::setPt4(const QPointF& p)
{
	if (p == m_ctrlPoint)
		return;

	prepareGeometryChange();
	m_ctrlPoint = p;
	m_circle = xCircleData(m_thirdCircle.center(), m_ctrlPoint);
	m_ctrlPoint = calCirclePoint(m_ctrlPoint);

	update();
}

void xConcentricCircle::moveBy(const QPointF& delta)
{
	if (delta.isNull())
		return;

	prepareGeometryChange();
	m_circle.translate(delta);
	m_thirdCircle.translate(delta);
	m_ctrlPoint = calCirclePoint(m_ctrlPoint + delta); 
	update();
}

QList<QPointF> xConcentricCircle::controlPoints() const
{
	return {pt1(),pt2() ,pt3() ,pt4() };
}

void xConcentricCircle::moveCtrlPoint(const QPointF& pt, const QPointF& movedPt)
{
	if (Distance(pt, pt1()) < DELTA_DIST / viewScaleFactor())
	{
		setPt1(movedPt);
	}
	else if (Distance(pt, pt2()) < DELTA_DIST / viewScaleFactor())
	{
		setPt2(movedPt);
	}
	else if (Distance(pt, pt3()) < DELTA_DIST / viewScaleFactor())
	{
		setPt3(movedPt);
	}
	else if (Distance(pt, pt4()) < DELTA_DIST / viewScaleFactor())
	{
		setPt4(movedPt);
	}
}

bool xConcentricCircle::isCtrlPoint(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	return (Distance(pt1(), p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(pt2(), p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(pt3(), p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(pt4(), p) < DELTA_DIST_2 / viewScaleFactor());
}

bool xConcentricCircle::isFittingEntity(const QPointF& p)
{
	const qreal dw = fabs(Distance(p, m_circle.center()) - m_circle.radius());
	if (fabs(dw) < DELTA_DIST_2 / viewScaleFactor())
		return true;
	return false;
}

QPointF xConcentricCircle::calCirclePoint(const QPointF& p)
{
	QLineF tempLine = QLineF(m_circle.center(), p);
	QPointF tempPoint;
	tempPoint = m_circle.center() + PointFromPolar(m_circle.radius(), tempLine.angle() * M_PI / 180.0);
	return tempPoint;
}
