#include "xMyArc.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

xMyArc::xMyArc(xGraphicView* view, QGraphicsItem* parent)
	:xEntity(view,parent)
{
}

xMyArc::xMyArc(const xArcData& arc, xGraphicView* view, QGraphicsItem* parent)
	: xEntity(view, parent)
	, m_arc(arc)
{
}

xMyArc::xMyArc(const QPointF& center, qreal radius, qreal startAngle, qreal spanAngle, xGraphicView* view, QGraphicsItem* parent)
	: xEntity(view, parent)
	, m_arc(center,radius, startAngle,spanAngle)
{
}

xMyArc::xMyArc(const QPointF& p1, const QPointF& p2, const QPointF& p3, xGraphicView* view, QGraphicsItem* parent)
	: xEntity(view, parent)
	, m_arc(p1,p2,p3)
{
}

xMyArc::~xMyArc()
{}

int xMyArc::type() const
{
	return Type;
}

void xMyArc::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);

	auto style = m_style;

	if (style !=xDef::S_NoStyle)
	{
		if (option->state & QStyle::State_Selected)
		{
			style = xDef::S_Selected;
		}

		if (option->state & QStyle::State_MouseOver)
		{
			if (style == xDef::S_Selected)
				style = xDef::S_HoverSelected;
			else
				style = xDef::S_Hovered;
		}

		const qreal f = viewScaleFactor();
		MakeStyle(style, &m_pen, nullptr, f);
	}

	painter->setPen(m_pen);
	if (m_arc.isValid())
	{
		QPolygonF path = drawArcPath(m_arc.center(), m_arc.radius(), m_arc.startAngle(), m_arc.spanAngle() + m_arc.startAngle(), m_arc.spanAngle() < 0);
		painter->drawPolyline(path);
	}


	if ((option->state & QStyle::State_Selected) && (flags() & ItemIsMovable))
	{
		const qreal w = m_pen.widthF();
		painter->fillRect(QRectF(m_arc.center().x() - w, m_arc.center().y() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_arc.pt1().x() - w, m_arc.pt1().y() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_arc.pt2().x() - w, m_arc.pt2().y() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_arc.pt3().x() - w, m_arc.pt3().y() - w, w + w, w + w), Qt::yellow);
	}
}

QRectF xMyArc::boundingRect() const
{
	if (!m_arc.isValid())
		return QRectF();

	// 计算图形在视场中的矩形，包括画笔的宽度，否则无法正确显示
	// Note：画笔宽度设置为2倍以便更容易被选中
	const qreal pw = m_pen.widthF() * 2;
	const qreal x = m_arc.center().x() - m_arc.radius() - pw;
	const qreal y = m_arc.center().y() - m_arc.radius() - pw;
	const qreal w = m_arc.radius() + pw;
	return QRectF(x, y, w + w, w + w);
}

QPainterPath xMyArc::shape() const
{
	QPainterPath path;
	if (!m_arc.isValid())
		return path;

	path.addPolygon(drawArcPath(m_arc.center(), m_arc.radius(), m_arc.startAngle(), m_arc.spanAngle()+ m_arc.startAngle(), m_arc.spanAngle() < 0));
	return StrokeShapeFromPath(path, m_pen.widthF() * 2);
}

void xMyArc::setArc(const QPointF& center, qreal radius, qreal startAngle, qreal spanAngle)
{
	if (center == m_arc.center() && qFuzzyCompare(radius, m_arc.radius()) && qFuzzyCompare(startAngle, m_arc.startAngle()) &&
		qFuzzyCompare(spanAngle, m_arc.spanAngle()))
		return;
	prepareGeometryChange();
	m_arc = xArcData(center, radius, startAngle,spanAngle);
	update();
	emit shapeChanged();
}

void xMyArc::setArc(const QPointF& p1, const QPointF& p2, const QPointF& p3)
{
	if (p1 == m_arc.pt1() && p2 == m_arc.pt2() && p3 == m_arc.pt3())
		return;
	prepareGeometryChange();
	m_arc = xArcData(p1, p2, p3);
	update();
	emit shapeChanged();
}

void xMyArc::setRadius(qreal radius)
{
	if (qFuzzyCompare(radius, m_arc.radius()))
		return;

	prepareGeometryChange();
	m_arc.setRadius(radius);
	update();
	emit shapeChanged();
}

void xMyArc::setPt1(const QPointF& p)
{
	if (p == m_arc.pt1())
		return;

	prepareGeometryChange();
	m_arc = xArcData(p,m_arc.pt2(),m_arc.pt3());
	update();
	emit shapeChanged();
}

void xMyArc::setPt2(const QPointF& p)
{
	if (p == m_arc.pt2())
		return;

	prepareGeometryChange();
	m_arc = xArcData( m_arc.pt1(),p, m_arc.pt3());
	update();
	emit shapeChanged();
}

void xMyArc::setPt3(const QPointF& p)
{
	if (p == m_arc.pt3())
		return;

	prepareGeometryChange();
	m_arc = xArcData(m_arc.pt1(), m_arc.pt2(), p);
	update();
	emit shapeChanged();
}

void xMyArc::setStartAngle(qreal startAngle)
{
	if (qFuzzyCompare(startAngle, m_arc.startAngle()))
		return;

	prepareGeometryChange();
	m_arc.setStartAngle(startAngle);
	update();
	emit shapeChanged();
}

void xMyArc::setSpanAngle(qreal spanAngle)
{
	if (qFuzzyCompare(spanAngle, m_arc.spanAngle()))
		return;

	prepareGeometryChange();
	m_arc.setSpanAngle(spanAngle);
	update();
	emit shapeChanged();
}

void xMyArc::moveBy(const QPointF& delta)
{
	if (delta.isNull())
		return;

	prepareGeometryChange();
	m_arc.translate(delta);
	update();
	emit posChanged(delta);
}

QList<QPointF> xMyArc::controlPoints() const
{
	return { pt1(), pt2(), pt3() };
}

void xMyArc::moveCtrlPoint(const QPointF& pt, const QPointF& movedPt)
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
}

bool xMyArc::isCtrlPoint(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	return (Distance(pt1(), p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(pt2(), p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(pt3(), p) < DELTA_DIST_2 / viewScaleFactor());
}

bool xMyArc::isFittingEntity(const QPointF& p)
{
	return  pointIsOnArc(p);
}

QPolygonF xMyArc::drawArcPath( QPointF c, qreal radius, qreal startAngle, qreal endAngle, bool reversed)const
{
	QPolygonF path;
	if (radius < 1.0e-6) {
		qDebug("xPainter::createArc: invalid radius: %f", radius);
		return path;
	}
	double aStep = fabs(2.0 / radius); // 角度步径（弧度）
	if (aStep < 0.05) aStep = 0.05;
	if (reversed)
	{
		if (startAngle <= endAngle)
			startAngle += 2.0 * M_PI;
		aStep *= -1;
	}
	else
	{
		if (startAngle >= endAngle)
			endAngle += 2.0 * M_PI;
	}
	double a;
	path.clear();

	double da = fabs(endAngle - startAngle);
	for (a = startAngle; fabs(a - startAngle) < da; a += aStep)
	{
		if (path.size() > 30000)
			break;// 避免栈溢出
		path << QPointF(c.x() + cos(a) * radius, c.y() - sin(a) * radius);
	}
	QPointF pt2(c.x() + cos(endAngle) * radius, c.y() - sin(endAngle) * radius);
	if (path.size() > 0 && path.last() != pt2)
		path << pt2;

	return path;
}

bool xMyArc::pointIsOnArc(QPointF pt)
{
	bool arcDire = m_arc.spanAngle() < 0;
	double Angle1 = atan2((m_arc.pt3().y() - m_arc.center().y()), (m_arc.pt3().x() - m_arc.center().x()));
	double Angle2 = atan2((m_arc.pt1().y() - m_arc.center().y()), (m_arc.pt1().x() - m_arc.center().x()));

	double Angle = Angle1 - Angle2;
	Angle = Angle * 180.0 / M_PI;

	if (Angle < 0) Angle = 360 + Angle;
	if (Angle == 0) Angle = 360;

	// 判断当前线段的向量
	double radius = m_arc.radius();
	double distance = Distance(pt,m_arc.center());
	if (qAbs(distance - radius) < DELTA_DIST_2 / viewScaleFactor())
	{
		double tempAngle1 = atan2((pt.y() - m_arc.center().y()), (pt.x() - m_arc.center().x()));
		double tempAngle = tempAngle1 - Angle2;
		tempAngle = tempAngle * 180.0 / M_PI;
		if (tempAngle < 0) tempAngle = 360 + tempAngle;
		if (tempAngle == 0) tempAngle = 360;
		if (arcDire) // 判断圆弧方向
		{
			if (tempAngle > Angle)
				return true;
		}
		else
		{
			if (tempAngle < Angle)
				return true;
		}

	}
	return false;
}


