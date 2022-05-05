#include "xTypeDef.h"
#include <QPen>
#include <QDebug>

QPainterPath StrokeShapeFromPath(const QPainterPath &path, qreal width)
{
	if (path.isEmpty() || width < 0.0001)
		return path;

	QPainterPathStroker ps;
	ps.setCapStyle(Qt::FlatCap);// 设置帽风格，不覆盖直线末端
	ps.setJoinStyle(Qt::BevelJoin);// 设置连接样式，线与线之间的缺口填满
	ps.setWidth(width);
	return ps.createStroke(path);// 将规则给路径并返回一个新路径
}

double DistancePoint2Line(const QPointF &p, const QLineF &line)
{
	const auto v1 = p - line.p1();
	const auto v2 = line.p2() - line.p1();
	const double S = v1.x() * v2.y() - v1.y() * v2.x();
	const double l = line.length();
	if (l > 0.000001)
		return fabs(S / l);
	return -1.0;
}

xCircleData::xCircleData(const QPointF &c, qreal r)
	: c(c)
	, r(r)
{
	generate3P();
}

xCircleData::xCircleData(qreal cx, qreal cy, qreal r)
	: c(cx, cy)
	, r(r)
{
	generate3P();
}

xCircleData::xCircleData(const QPointF &p1, const QPointF &p2, const QPointF &p3)
	: p1(p1), p2(p2), p3(p3)
{
	createFrom3P();
}

xCircleData::xCircleData(const QPointF& c, const QPointF& p2)
	: c(c)
{
	r = sqrt(pow(p2.x() - c.x(), 2) + pow(p2.y() - c.y(), 2));

	generate3P();
}

void xCircleData::setCenter(const QPointF &center)
{
	if (center == c)
		return;
	auto d = center - c;
	p1 += d;
	p2 += d;
	p3 += d;
	c = center;
}

void xCircleData::setRadius(qreal radius)
{
	if (qFuzzyCompare(radius, r))
		return;

	r = radius;
	p1 = c + PointFromPolar(r, QLineF(c, p1).angle() * M_PI / 180.0);
	p2 = c + PointFromPolar(r, QLineF(c, p2).angle() * M_PI / 180.0);
	p3 = c + PointFromPolar(r, QLineF(c, p3).angle() * M_PI / 180.0);
}

void xCircleData::createFrom3P()
{
	if (p1 == p2 || p2 == p3 || p3 == p1)
	{
		qWarning() << __FUNCTION__": There are some points invalid";
		return;
	}

	auto va = p2 - p1;
	auto vb = p3 - p1;
	qreal ra2 = QPointF::dotProduct(va, va) * 0.5;
	qreal rb2 = QPointF::dotProduct(vb, vb) * 0.5;
	qreal crossp = va.x() * vb.y() - va.y() * vb.x();
	// crossp为0则3点在同一直线上
	if (qFuzzyCompare(crossp, 0.0))
	{
		qWarning() << __FUNCTION__": Three points in a line";
		c = QPointF();
		r = 0;
		return;
	}
	crossp = 1.0 / crossp;
	c.setX((ra2 * vb.y() - rb2 * va.y()) * crossp);
	c.setY((rb2 * va.x() - ra2 * vb.x()) * crossp);
	r = QLineF(c, QPointF(0, 0)).length();
	c += p1;
}

void xCircleData::generate3P()
{
	if (qFuzzyCompare(r, 0.0))
	{
		qWarning() << __FUNCTION__": The radius is invalid";
		return;
	}

	p1 = c + PointFromPolar(r, 0);
	p2 = c + PointFromPolar(r, M_PI * 2.0 / 3.0);
	p3 = c + PointFromPolar(r, M_PI * 4.0 / 3.0);
}
