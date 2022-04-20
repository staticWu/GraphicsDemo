#include "xRegStraightLine.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

xRegStraightLine::xRegStraightLine(xGraphicView* view, QGraphicsItem* parent)
	:xRegionEntity(view,parent)
{
	// 先隐藏拟合直线
	m_straightLine = new xStraightLine(view, this);
	m_straightLine->setFlag(ItemIsMovable, false);
	m_straightLine->setFlag(ItemStacksBehindParent);
	m_straightLine->hide();
	m_regLine = new xRegLine(view, this);
	m_regLine->hide();
}

xRegStraightLine::xRegStraightLine(const QLineF& line, qreal width, xGraphicView* view, QGraphicsItem* parent)
	: xRegionEntity(width,view, parent)
{
	m_straightLine = new xStraightLine(view, this);
	m_straightLine->setFlag(ItemIsMovable, false);
	m_straightLine->setFlag(ItemStacksBehindParent);
	m_straightLine->hide();
	m_regLine = new xRegLine(view, this);
	m_regLine->setLine(line,width);
}

xRegStraightLine::xRegStraightLine(const QPointF& p1, const QPointF& p2, qreal width, xGraphicView* view, QGraphicsItem* parent)
	: xRegionEntity(view, parent)
{
	m_straightLine = new xStraightLine(view, this);
	m_straightLine->setFlag(ItemIsMovable, false);
	m_straightLine->setFlag(ItemStacksBehindParent);
	m_straightLine->hide();
	m_regLine = new xRegLine(view, this);
	m_regLine->setLine(p1, p2, width);
}

xRegStraightLine::~xRegStraightLine()
{
}

int xRegStraightLine::type() const
{
	return Type;
}

void xRegStraightLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);

	const qreal angle = m_regLine->lineData().angle() * M_PI / 180.0;
	// 矩形4个顶点，line.p1左边的点开始，顺时针
	const auto p1 = PointFromPolar(m_width, angle + M_PI * 0.5) + m_regLine->lineData().p1();
	const auto p2 = PointFromPolar(m_width, angle + M_PI * 0.5) + m_regLine->lineData().p2();
	const auto p3 = PointFromPolar(m_width, angle - M_PI * 0.5) + m_regLine->lineData().p2();
	const auto p4 = PointFromPolar(m_width, angle - M_PI * 0.5) + m_regLine->lineData().p1();

	// 边框路径
	QPainterPath path;
	path.moveTo(p1);
	path.lineTo(p2);
	path.lineTo(p3);
	path.lineTo(p4);
	path.closeSubpath();

	auto style = m_style;
	const qreal f = viewScaleFactor();

	if (style != xStyle::NoStyle)
	{
		// 选中状态
		if (option->state & QStyle::State_Selected)
		{
			style = xStyle::RegSelected;
			// 选中时绘画边框
			painter->setPen(QPen(QColor(255, 255, 0, 255), 1.0 / f));
			painter->drawPath(path);
		}

		// 悬停状态
		if (option->state & QStyle::State_MouseOver)
		{
			style = xStyle::RegHovered;
		}

		xStyle::makeStyle(style, &m_pen, &m_brush, f);
	}

	// 填充范围
	painter->fillPath(path, m_brush);
	// 画中心线
	painter->setPen(m_pen);
	painter->drawLine(m_regLine->lineData());

	// 填充中心线箭头
	const auto lp = PointFromPolar(14.0 / f, angle + M_PI - ANGLE_15_RAD) + m_regLine->lineData().p2();
	const auto rp = PointFromPolar(14.0 / f, angle + M_PI + ANGLE_15_RAD) + m_regLine->lineData().p2();
	QPainterPath ap;
	ap.moveTo(m_regLine->lineData().p2());
	ap.lineTo(lp);
	ap.lineTo(rp);
	ap.closeSubpath();
	painter->fillPath(ap, m_pen.color());

	// 选中时绘画控制点
	if ((option->state & QStyle::State_Selected) && (flags() & ItemIsMovable))
	{
		const qreal w = m_pen.widthF();
		painter->fillRect(QRectF(m_regLine->lineData().x1() - w, m_regLine->lineData().y1() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_regLine->lineData().x2() - w, m_regLine->lineData().y2() - w, w + w, w + w), Qt::yellow);
	}

}

QRectF xRegStraightLine::boundingRect() const
{
	if (m_regLine->lineData().isNull())
		return QRectF();

	const qreal angle = m_regLine->lineData().angle() * M_PI / 180.0;
	// 矩形4个顶点，line.p1点左边的点开始，顺时针
	const auto p1 = PointFromPolar(m_width, angle + M_PI * 0.5) + m_regLine->lineData().p1();
	const auto p2 = PointFromPolar(m_width, angle + M_PI * 0.5) + m_regLine->lineData().p2();
	const auto p3 = PointFromPolar(m_width, angle - M_PI * 0.5) + m_regLine->lineData().p2();
	const auto p4 = PointFromPolar(m_width, angle - M_PI * 0.5) + m_regLine->lineData().p1();

	const qreal sx = std::min({ p1.x(), p2.x(), p3.x(), p4.x() });
	const qreal bx = std::max({ p1.x(), p2.x(), p3.x(), p4.x() });
	const qreal sy = std::min({ p1.y(), p2.y(), p3.y(), p4.y() });
	const qreal by = std::max({ p1.y(), p2.y(), p3.y(), p4.y() });

	const qreal w = m_pen.widthF() * 2;
	return QRectF(sx - w, sy - w, bx - sx + w + w, by - sy + w + w);
}

QPainterPath xRegStraightLine::shape() const
{
	QPainterPath path;
	if (m_regLine->lineData().isNull())
		return path;

	path.moveTo(m_regLine->lineData().p1());
	path.lineTo(m_regLine->lineData().p2());

	return StrokeShapeFromPath(path, (m_width + m_pen.widthF()) * 2);
}

void xRegStraightLine::setLine(const QPointF& p1, const QPointF& p2,qreal width)
{
	if (m_regLine->lineData().p1() == p1 && m_regLine->lineData().p2() == p2 && qFuzzyCompare(width, m_width))
		return;
	prepareGeometryChange();// 通知场景，几何体将改变
	m_regLine->setPt1(p1);
	m_regLine->setPt2(p2);
	m_width = width;
	update();
}

void xRegStraightLine::setSubLine(const QPointF& p1, const QPointF& p2)
{
	// 给进来的点是定位点，直线的端点需要重现计算
	QList<QPointF> list = calStraighLinePoints(p1, p2); // 返回直线两个端点
	if (list.isEmpty() && list.count() < 2)
		return;

	m_straightLine->setLine(list[0], list[1]);
	m_straightLine->setStyle(xStyle::Measured);
	m_straightLine->show();
}

void xRegStraightLine::setSubLine(const QLineF& line)
{
	QList<QPointF> list = calStraighLinePoints(line.p1(),line.p2());
	if (list.isEmpty() && list.count() < 2)
		return;
	m_straightLine->setLine(list[0], list[1]);
	m_straightLine->setStyle(xStyle::Measured);
	m_straightLine->show();
}

void xRegStraightLine::setAnchorPoint1(const QPointF& p)
{
	if (m_regLine->lineData().p1() == p)
		return;

	prepareGeometryChange();
	m_regLine->setPt1(p);

	/*QList<QPointF> list = calStraighLinePoints(p,anchorPoint2());
	if (list.isEmpty() && list.count() < 2)
		return;
	m_straightLine->setLine(list[0], list[1]);*/
	update();
}

void xRegStraightLine::setAnchorPoint2(const QPointF& p)
{
	if (m_regLine->lineData().p2() == p)
		return;

	prepareGeometryChange();
	m_regLine->setPt2(p);

	/*QList<QPointF> list = calStraighLinePoints(anchorPoint1(), p);
	if (list.isEmpty() && list.count() < 2)
		return;
	m_straightLine->setLine(list[0], list[1]);*/
	update();
}

void xRegStraightLine::moveBy(const QPointF& delta)
{
	if (delta.isNull())
		return;

	prepareGeometryChange();
	m_regLine->setPt1(m_regLine->pt1() + delta);
	m_regLine->setPt2(m_regLine->pt2() + delta);	
	update();
}

QList<QPointF> xRegStraightLine::controlPoints() const
{
	return { anchorPoint1(),anchorPoint2() };
}

void xRegStraightLine::moveCtrlPoint(const QPointF& p, const QPointF& movedPt)
{
	if (Distance(p, anchorPoint1()) < DELTA_DIST / viewScaleFactor())
	{
		setAnchorPoint1(movedPt);
	}
	else if (Distance(p, anchorPoint2()) < DELTA_DIST / viewScaleFactor())
	{
		setAnchorPoint2(movedPt);
	}
}

bool xRegStraightLine::isCtrlPoint(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	return (Distance(anchorPoint1(), p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(anchorPoint2(), p) < DELTA_DIST_2 / viewScaleFactor());
}

void xRegStraightLine::changeEdgeByPoint(const QPointF& p)
{
	setRegWidth(DistancePoint2Line(p, m_regLine->lineData()));
}

bool xRegStraightLine::isRegionEdge(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	if (fabs(DistancePoint2Line(p, m_regLine->lineData()) - m_width) < DELTA_DIST_2 / viewScaleFactor())
		return true;
	return false;
}

QList<QPointF> xRegStraightLine::calStraighLinePoints(const QPointF& p1, const QPointF& p2)
{
	// 计算画线的起始点和结束点 根据点的直线方程 k = (y2-y1) / (x2-x1)
	if (p1.isNull() || p2.isNull())
		return {};

	QPoint pt1, pt2;
	double disX = p2.x() - p1.x();
	if (disX == 0) // 垂直时
	{
		pt1.setX(p1.x());
		pt1.setY(0);
		pt2.setX(p1.x());
		pt2.setY(m_view->scene()->height());
		return {pt1,pt2};
	}

	double k = (p2.y() - p1.y()) / disX;
	if (k == 0)  // 平行时
	{
		pt1.setX(0);
		pt1.setY(p1.y());
		pt2.setX(m_view->scene()->width());
		pt2.setY(p1.y());
	}
	else // 开始点小于结束点
	{
		pt1.setY(0);
		pt2.setY(m_view->scene()->height());
		double startX = (pt1.y() - p1.y()) / k + p1.x();
		double endX = (pt2.y() - p1.y()) / k + p1.x();
		pt1.setX(startX);
		pt2.setX(endX);
	}
	return { pt1,pt2 };
}
