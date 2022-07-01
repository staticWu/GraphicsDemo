#include "xStraightLine.h"
#include <QStyleOptionGraphicsItem>
#include <QDebug>
xStraightLine::xStraightLine(xGraphicView* view, QGraphicsItem* parent)
	:xEntity(view, parent)
{
}

xStraightLine::xStraightLine(const QPointF& p1, const QPointF& p2, xGraphicView* view, QGraphicsItem* parent)
	: xEntity(view, parent), m_point1(p1), m_point2(p2)
{
}

xStraightLine::xStraightLine(const QLineF& line, xGraphicView* view, QGraphicsItem* parent)
	: xEntity(view, parent), m_line(line)
{
}

xStraightLine::~xStraightLine()
{
}

int xStraightLine::type() const
{
	return Type;
}

void xStraightLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);

	auto style = m_style;
	calStraighLinePoints();
	if (style != xDef::S_NoStyle)
	{
		// ѡ��״̬
		if (option->state & QStyle::State_Selected)
		{
			style = xDef::S_Selected;
		}

		// ��ͣ״̬
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
	m_pen.setStyle(Qt::SolidLine);// Ĭ��ѡ��������
	painter->setPen(m_pen);
	painter->drawLine(m_line);

	// ѡ��ʱ�滭���Ƶ�
	if ((option->state & QStyle::State_Selected) && (flags() & ItemIsMovable))
	{
		const qreal w = m_pen.widthF();
		painter->fillRect(QRectF(m_point1.x() - w, m_point1.y() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_point2.x() - w, m_point2.y() - w, w + w, w + w), Qt::yellow);
	}
}

QRectF xStraightLine::boundingRect() const
{

	if (m_line.isNull())
		return QRectF();

	// ����ͼ�����ӳ��еľ��Σ��������ʵĿ�ȣ������޷���ȷ��ʾ
	// Note�����ʿ������Ϊ2���Ա�����ױ�ѡ��
	const qreal w = m_pen.widthF() * 2;
	const qreal sx = std::min(m_line.x1(), m_line.x2());
	const qreal bx = std::max(m_line.x1(), m_line.x2());
	const qreal sy = std::min(m_line.y1(), m_line.y2());
	const qreal by = std::max(m_line.y1(), m_line.y2());
	return QRectF(sx - w, sy - w, bx - sx + w + w, by - sy + w + w);
}

QPainterPath xStraightLine::shape() const
{
	QPainterPath path;
	if (m_line.isNull())
		return path;

	path.moveTo(m_line.p1());
	path.lineTo(m_line.p2());
	return StrokeShapeFromPath(path, m_pen.widthF() * 2);
}

void xStraightLine::setLine(const QPointF& p1, const QPointF& p2)
{
	if (m_line.p1() == p1 && m_line.p2() == p2)
		return;

	prepareGeometryChange();
	m_line.setPoints(p1, p2);
	update();
}

void xStraightLine::setPt1(const QPointF& p)
{
	if (m_line.p1() == p)
		return;

	prepareGeometryChange();
	m_line.setP1(p);
	update();
}

void xStraightLine::setPt2(const QPointF& p)
{
	if (m_line.p2() == p)
		return;

	prepareGeometryChange();
	m_line.setP2(p);
	update();
}

void xStraightLine::setAnchorPoint1(const QPointF& p)
{
	if (m_point1 == p)
		return;

	prepareGeometryChange();
	m_point1 = p;
	calStraighLinePoints(); // ���¼���ֱ�߶˵�
	update();
}

void xStraightLine::setAnchorPoint2(const QPointF& p)
{
	if (m_point2 == p)
		return;

	prepareGeometryChange();
	m_point2 = p;
	calStraighLinePoints(); // ���¼���ֱ�߶˵�
	update();
}

void xStraightLine::moveBy(const QPointF& delta)
{
	if (delta.isNull())
		return;

	prepareGeometryChange();
	m_line.translate(delta);
	m_point1 += delta;
	m_point2 += delta;
	calStraighLinePoints(); // ���¼���ֱ�߶˵�
	update();
	emit pointChange();// ���Ƶ����
}

QList<QPointF> xStraightLine::controlPoints() const
{
	return { pt1(), pt2() };
}

void xStraightLine::moveCtrlPoint(const QPointF& p, const QPointF& movedPt)
{
	if (Distance(p, anchorPoint1()) < DELTA_DIST / viewScaleFactor())
	{
		setAnchorPoint1(movedPt);
		emit pointChange();// ���Ƶ����
	}
	else if (Distance(p, anchorPoint2()) < DELTA_DIST / viewScaleFactor())
	{
		setAnchorPoint2(movedPt);
		emit pointChange();// ���Ƶ����
	}
}

bool xStraightLine::isCtrlPoint(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	return (Distance(anchorPoint1(), p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(anchorPoint2(), p) < DELTA_DIST_2 / viewScaleFactor());
}

bool xStraightLine::isFittingEntity(const QPointF& p)
{
	if (fabs(DistancePoint2Line(p, m_line)) < DELTA_DIST_2 / viewScaleFactor())
		return true;
	return false;
}

void xStraightLine::calStraighLinePoints()
{
	// ���㻭�ߵ���ʼ��ͽ����� ���ݵ��ֱ�߷��� k = (y2-y1) / (x2-x1)
	if (anchorPoint1().isNull() || anchorPoint2().isNull())
		return;

	QPoint pt1, pt2;
	double disX = anchorPoint2().x() - anchorPoint1().x();
	if (disX == 0) // ��ֱʱ
	{
		pt1.setX(anchorPoint1().x());
		pt1.setY(-m_view->height() / viewScaleFactor());
		pt2.setX(anchorPoint1().x());
		pt2.setY(m_view->height() / viewScaleFactor());
		setLine(pt1, pt2);
		return;
	}

	double k = (anchorPoint2().y() - anchorPoint1().y()) / disX;
	if (k == 0)  // ƽ��ʱ
	{
		pt1.setX(-m_view->width() / viewScaleFactor());
		pt1.setY(anchorPoint1().y());
		pt2.setX(m_view->width() / viewScaleFactor());
		pt2.setY(anchorPoint1().y());
		setLine(pt1, pt2);
	}
	else // ��ʼ��С�ڽ�����
	{
		pt1.setY(-m_view->height() / viewScaleFactor());
		pt2.setY(m_view->height() / viewScaleFactor());
		double startX = (pt1.y() - anchorPoint1().y()) / k + anchorPoint1().x();
		double endX = (pt2.y() - anchorPoint1().y()) / k + anchorPoint1().x();
		pt1.setX(startX);
		pt2.setX(endX);
		setLine(pt1, pt2);
	}
}
