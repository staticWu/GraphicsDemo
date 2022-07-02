#include "xRegArc.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

xRegArc::xRegArc(xGraphicView* view, QGraphicsItem* parent)
	:xRegionEntity(view,parent)
{
	m_midArc = new xMyArc(view);
	m_outArc = new xMyArc(view);
	m_inArc = new xMyArc(view);
	m_fittingArc = new xMyArc(view, this);
}

xRegArc::xRegArc(const xArcData& arc, qreal width, xGraphicView* view, QGraphicsItem* parent)
	: xRegionEntity(view, parent)
{
	m_width = width;
	m_midArc = new xMyArc(arc,view);
	updateRegArc();
	m_fittingArc = new xMyArc(view, this);
}

xRegArc::xRegArc(const QPointF& center, qreal radius, qreal startAngle, qreal spanAngle, qreal width, xGraphicView* view, QGraphicsItem* parent)
	: xRegionEntity(view, parent)
{
	m_width = width;
	m_midArc = new xMyArc(center, radius, startAngle, spanAngle, view);
	updateRegArc();
	m_fittingArc = new xMyArc(view, this);
}

xRegArc::xRegArc(const QPointF& p1, const QPointF& p2, const QPointF& p3, qreal width, xGraphicView* view, QGraphicsItem* parent)
	: xRegionEntity(view, parent)
{
	m_width = width;
	m_midArc = new xMyArc(p1, p2, p3, view);
	updateRegArc();
	m_fittingArc = new xMyArc(view,this);
}

xRegArc::~xRegArc()
{}

int xRegArc::type() const
{
	return Type;
}

void xRegArc::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);
	QPainterPath path;
	// ��Բ����
	path.addPath(fillAnnular(m_midArc->center(),m_midArc->radius(),m_width,m_midArc->startAngle(),m_midArc->spanAngle()));
	auto style = m_style;
	const qreal f = viewScaleFactor();

	if (style != xDef::S_NoStyle)
	{
		// ѡ��״̬
		if (option->state & QStyle::State_Selected)
		{
			style = xDef::S_RegSelected;
			// ѡ��ʱ�滭�߿�
			painter->setPen(QPen(QColor(255, 255, 0, 255), 1.0 / f));
			painter->drawPath(path);
		}

		// ��ͣ״̬
		if (option->state & QStyle::State_MouseOver)
		{
			style = xDef::S_RegHovered;
		}

		MakeStyle(style, &m_pen, &m_brush, f);
	}

	// ��䷶Χ
	painter->fillPath(path, m_brush);
	// ������Բ
	painter->setPen(m_pen);
	if(m_midArc->isValid())
		painter->drawPolyline(drawArcPath(m_midArc->center(), m_midArc->radius(), m_midArc->startAngle(), m_midArc->startAngle()+ m_midArc->spanAngle(), m_midArc->spanAngle() < 0));
	// ѡ��ʱ�滭���Ƶ�
	if ((option->state & QStyle::State_Selected) && (flags() & ItemIsMovable))
	{
		const qreal w = m_pen.widthF();
		painter->fillRect(QRectF(m_midArc->center().x() - w, m_midArc->center().y() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_midArc->pt1().x() - w, m_midArc->pt1().y() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_midArc->pt2().x() - w, m_midArc->pt2().y() - w, w + w, w + w), Qt::yellow);
		painter->fillRect(QRectF(m_midArc->pt3().x() - w, m_midArc->pt3().y() - w, w + w, w + w), Qt::yellow);
	}
}

QRectF xRegArc::boundingRect() const
{
	if (!m_midArc->isValid())
		return QRectF();

	// ����ͼ�����ӳ��еľ��Σ��������ʵĿ�ȣ������޷���ȷ��ʾ
	// Note�����ʿ������Ϊ2���Ա�����ױ�ѡ��
	const qreal pw = m_pen.widthF() * 2;
	const qreal x = m_midArc->center().x() - m_midArc->radius() - m_width - pw;
	const qreal y = m_midArc->center().y() - m_midArc->radius() - m_width - pw;
	const qreal w = m_midArc->radius() + m_width + pw;
	return QRectF(x, y, w + w, w + w);
}

QPainterPath xRegArc::shape() const
{
	QPainterPath path;
	if (!m_midArc->isValid())
		return path;

	const qreal pw = m_pen.widthF();
	// ��Բ����
	path.addPath(fillAnnular(m_midArc->center(), m_midArc->radius() + pw, m_width, m_midArc->startAngle(), m_midArc->spanAngle()));
	return path;
}

void xRegArc::setSubArc(const QPointF& p1, const QPointF& p2, const QPointF& p3)
{
	m_fittingArc ->setArc(p1,p2,p3);
	m_fittingArc->setFlag(ItemIsMovable, false);
	m_fittingArc->setFlag(ItemStacksBehindParent);
	m_fittingArc->setStyle(xDef::S_Measured);
}

void xRegArc::setSubArc(const QPointF& center, qreal radius, qreal startAngle, qreal spanAngle)
{
	m_fittingArc->setArc(center, radius, startAngle, spanAngle);	
	m_fittingArc->setFlag(ItemIsMovable, false);
	m_fittingArc->setFlag(ItemStacksBehindParent);
	m_fittingArc->setStyle(xDef::S_Measured);
}

void xRegArc::setArc(const QPointF& center, qreal radius, qreal startAngle, qreal spanAngle,qreal width)
{
	if (center == m_midArc->center() && qFuzzyCompare(radius, m_midArc->radius()) && qFuzzyCompare(width, m_width))
		return;

	prepareGeometryChange();
	m_midArc->setArc(center, radius, startAngle, spanAngle);
	m_width = width;
	update();
	emit shapeChanged();
}

void xRegArc::setArc(const QPointF& p1, const QPointF& p2, const QPointF& p3, qreal width)
{
	xMyArc c = xMyArc(p1, p2, p3, m_view);
	if (c.center() == m_midArc->center() && qFuzzyCompare(c.radius(), m_midArc->radius()) && qFuzzyCompare(width, m_width))
		return;

	prepareGeometryChange();
	m_midArc ->setArc(p1,p2,p3);
	m_width = width;
	updateRegArc();
	update();
	emit shapeChanged();
}

void xRegArc::setRadius(qreal radius)
{
	if (qFuzzyCompare(radius, m_midArc->radius()))
		return;

	prepareGeometryChange();
	m_midArc->setRadius(radius);
	updateRegArc();
	update();
	emit shapeChanged();
}

void xRegArc::setPt1(const QPointF& p)
{
	if (p == m_midArc->pt1())
		return;

	prepareGeometryChange();
	m_midArc->setArc(p, m_midArc->pt2(), m_midArc->pt3());
	updateRegArc();
	update();
	emit shapeChanged();
}

void xRegArc::setPt2(const QPointF& p)
{
	if (p == m_midArc->pt2())
		return;

	prepareGeometryChange();
	m_midArc->setArc( m_midArc->pt1(),p, m_midArc->pt3());
	updateRegArc();
	update();
	emit shapeChanged();
}

void xRegArc::setPt3(const QPointF& p)
{
	if (p == m_midArc->pt3())
		return;

	prepareGeometryChange();
	m_midArc->setArc(m_midArc->pt1(), m_midArc->pt2(), p);
	updateRegArc();
	update();
	emit shapeChanged();
}

void xRegArc::setStartAngle(qreal startAngle)
{
	if (qFuzzyCompare(startAngle, m_midArc->startAngle()))
		return;

	prepareGeometryChange();
	m_midArc->setStartAngle(startAngle);
	updateRegArc();
	update();
	emit shapeChanged();
}

void xRegArc::setSpanAngle(qreal spanAngle)
{
	if (qFuzzyCompare(spanAngle, m_midArc->spanAngle()))
		return;

	prepareGeometryChange();
	m_midArc->setSpanAngle(spanAngle);
	updateRegArc();
	update();
	emit shapeChanged();
}

void xRegArc::moveBy(const QPointF& delta)
{
	if (delta.isNull())
		return;

	prepareGeometryChange();
	m_midArc->setArc(m_midArc->pt1()+=delta, m_midArc->pt2() += delta, m_midArc->pt3() += delta);
	updateRegArc();
	update();
	emit posChanged(delta);
}

QList<QPointF> xRegArc::controlPoints() const
{
	return { pt1(), pt2(), pt3() };
}

void xRegArc::moveCtrlPoint(const QPointF& pt, const QPointF& movedPt)
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

bool xRegArc::isCtrlPoint(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	return (Distance(pt1(), p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(pt2(), p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(pt3(), p) < DELTA_DIST_2 / viewScaleFactor());
}

void xRegArc::changeEdgeByPoint(const QPointF& p)
{
	const qreal dw = fabs(Distance(p, m_midArc->center()) - m_midArc->radius());
	setRegWidth(dw);
	updateRegArc();
}

bool xRegArc::isRegionEdge(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	const qreal dw = fabs(Distance(p, m_midArc->center()) - m_midArc->radius());
	if (fabs(dw - m_width) < DELTA_DIST_2 / viewScaleFactor())
		return true;
	return false;
}

QPolygonF xRegArc::drawArcPath(QPointF c, qreal radius, qreal startAngle, qreal endAngle, bool reversed)const
{
	QPolygonF path;
	if (radius < 1.0e-6) {
		qDebug("xPainter::createArc: invalid radius: %f", radius);
		return path;
	}
	double aStep = fabs(2.0 / radius); // �ǶȲ��������ȣ�
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
			break;// ����ջ���
		path << QPointF(c.x() + cos(a) * radius, c.y() - sin(a) * radius);
	}
	QPointF pt2(c.x() + cos(endAngle) * radius, c.y() - sin(endAngle) * radius);
	if (path.size() > 0 && path.last() != pt2)
		path << pt2;

	return path;
}


void xRegArc::updateRegArc()
{
	m_outArc = new xMyArc(m_midArc->center(),m_midArc->radius() + m_width,m_midArc->startAngle(),m_midArc->spanAngle(),m_view);
	if (m_midArc->radius() > m_width)// ��ֹ�뾶Ϊ��ֵ
		m_inArc = new xMyArc(m_midArc->center(), m_midArc->radius() - m_width, m_midArc->startAngle(), m_midArc->spanAngle(), m_view);
}

QPainterPath xRegArc::fillAnnular(const QPointF& c, qreal r, qreal w, qreal startAngle, double spanAngle)const
{
	double sa = rad2deg(startAngle);
	double alen = rad2deg(spanAngle);
	QRectF arc1;
	if (r > w)
	{
		arc1.setRect(c.x() - (r - w), c.y() - (r - w), 2 * (r - w), 2 * (r - w)); // inner arc
	}
	QRectF arc2(c.x() - (r + w), c.y() - (r + w), 2 * (r + w), 2 * (r + w)); // outer arc
	QPainterPath pp;
	if (r > w)
	{
		pp.moveTo(PointFromPolar(r - w, startAngle) + c);
		pp.arcTo(arc1, sa, alen);
	}
	else
	{
		pp.moveTo(c.x(), c.y());
	}
	pp.arcTo(arc2, sa + alen, -alen);

	if (r > w)
	{
		pp.lineTo(PointFromPolar(r - w, startAngle) + c);
	}
	else
	{
		pp.lineTo(c.x(), c.y());
	}

	return pp;
}


