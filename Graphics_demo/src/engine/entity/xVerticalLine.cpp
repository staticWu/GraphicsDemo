#include "xVerticalLine.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

xVerticalLine::xVerticalLine(xGraphicView* view, QGraphicsItem* item)
	:xEntity(view,item)
{
	m_SLine = new xStraightLine(view,this);
	connect(m_SLine,&xStraightLine::pointChange,this,&xVerticalLine::getChangePoint);
	m_SLine->hide();
}

xVerticalLine::~xVerticalLine()
{
}

int xVerticalLine::type() const
{
	return Type;
}

void xVerticalLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);

	auto style = m_style;

	if (style != xStyle::NoStyle)
	{
		// 选中状态
		if (option->state & QStyle::State_Selected)
		{
			style = xStyle::Selected;
		}

		// 悬停状态
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
	painter->drawLine(m_line);

	// 选中时绘画控制点
	/*if ((option->state & QStyle::State_Selected) && (flags() & ItemIsMovable))
	{
		const qreal w = m_pen.widthF();
		painter->fillRect(QRectF(m_line.x1() - w, m_line.y1() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_line.x2() - w, m_line.y2() - w, w + w, w + w), Qt::yellow);
	}*/
}

QRectF xVerticalLine::boundingRect() const
{
	if (m_line.isNull())
		return QRectF();

	// 计算图形在视场中的矩形，包括画笔的宽度，否则无法正确显示
	// Note：画笔宽度设置为2倍以便更容易被选中
	const qreal w = m_pen.widthF() * 2;
	const qreal sx = std::min(m_line.x1(), m_line.x2());
	const qreal bx = std::max(m_line.x1(), m_line.x2());
	const qreal sy = std::min(m_line.y1(), m_line.y2());
	const qreal by = std::max(m_line.y1(), m_line.y2());
	return QRectF(sx - w, sy - w, bx - sx + w + w, by - sy + w + w);
}

QPainterPath xVerticalLine::shape() const
{
	QPainterPath path;
	if (m_line.isNull())
		return path;

	path.moveTo(m_line.p1());
	path.lineTo(m_line.p2());
	return StrokeShapeFromPath(path, m_pen.widthF() * 2);
}

void xVerticalLine::setStraightLine(const QPointF& p1, const QPointF& p2)
{
	if (m_SLine->anchorPoint1() == p1 && m_SLine->anchorPoint2() == p2)
		return;

	prepareGeometryChange();
	m_SLine->setAnchorPoint1(p1);
	m_SLine->setAnchorPoint2(p2);
	m_SLine->setStyle(xStyle::Measured);
	m_SLine->show();
	update();
}

void xVerticalLine::setLine(const QPointF& p1, const QPointF& p2)
{
	if (m_line.p1() == p1 && m_line.p2() == p2)
		return;

	prepareGeometryChange();
	m_line.setPoints(p1, p2);
	update();
}

void xVerticalLine::setPt1(const QPointF& p)
{
	if (m_SLine->anchorPoint1() == p)
		return;

	prepareGeometryChange();
	m_SLine->setAnchorPoint1(p);
	update();
}

void xVerticalLine::setPt2(const QPointF& p)
{
	if (m_SLine->anchorPoint2() == p)
		return;

	prepareGeometryChange();
	m_SLine->setAnchorPoint2(p);
	update();
}

void xVerticalLine::setPt3(const QPointF& p)
{
	if (m_point == p)
		return;

	prepareGeometryChange();
	m_point = p;
	calVLine();
	update();
}

void xVerticalLine::moveBy(const QPointF& delta)
{
	if (delta.isNull())
		return;

	prepareGeometryChange();
	m_line.translate(delta);
	update();
}

QList<QPointF> xVerticalLine::controlPoints() const
{
	return {m_SLine->controlPoints()};
}

void xVerticalLine::moveCtrlPoint(const QPointF& pt, const QPointF& movedPt)
{
	if (Distance(pt, pt1()) < DELTA_DIST / viewScaleFactor())
	{
		setPt1(movedPt);
	}
	else if (Distance(pt, pt2()) < DELTA_DIST / viewScaleFactor())
	{
		setPt2(movedPt);
	}
}

bool xVerticalLine::isCtrlPoint(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	return (Distance(pt1(), p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(pt2(), p) < DELTA_DIST_2 / viewScaleFactor());
}

bool xVerticalLine::isFittingEntity(const QPointF& p)
{
	if (fabs(DistancePoint2Line(p, m_line)) < DELTA_DIST_2 / viewScaleFactor())
		return true;
	return false;
}

void xVerticalLine::calVLine()
{
	// 作出直线的垂线
		// 计算画线的起始点和结束点 根据点的直线方程 k = (y2-y1) / (x2-x1)
	double disX = pt2().x() - pt1().x();
	QPointF startPt, endPt;
	if (disX == 0) // 垂直时
	{
		startPt.setY(m_point.y()); // 平行
		startPt.setX(0);
		endPt.setY(m_point.y());
		endPt.setX(m_view->scene()->width());
		setLine(startPt, endPt);
		return;
	}

	double k = (pt2().y() - pt1().y()) / disX;
	if (k == 0)  // 平行时
	{

		startPt.setX(m_point.x()); // 垂直
		startPt.setY(0);
		endPt.setX(m_point.x());
		endPt.setY(m_view->scene()->width());
	}
	else
	{
		double k1 = -(1 / k); // 只要线段超出去就可以

		startPt.setY(m_point.y() + k1 * (m_view->scene()->width()));
		startPt.setX(m_point.x() + m_view->scene()->width());
		endPt.setY(m_point.y() - k1 * (m_view->scene()->width()));
		endPt.setX(m_point.x() - m_view->scene()->width());
	}
	setLine(startPt, endPt);
}

void xVerticalLine::getChangePoint()
{
	if (m_SLine->isVisible())
	{
		calVLine(); // 辅助线有变化，需重新计算垂线
	}
}
