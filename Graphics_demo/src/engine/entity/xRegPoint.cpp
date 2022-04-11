#include "xRegPoint.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

xRegPoint::xRegPoint(xGraphicView* view, QGraphicsItem* parent)
	:xRegionEntity(view,parent)
{
	m_subPoint = new xPoint(view, this);
	m_subPoint->setFlag(ItemIsMovable, false);
	m_subPoint->setFlag(ItemStacksBehindParent);
	m_subPoint->hide();
}

xRegPoint::xRegPoint(const QPoint& p, qreal width, xGraphicView* view, QGraphicsItem* parent)
	:xRegionEntity(view, parent)
{
	m_regPoint.setCenter(p);
	m_subPoint = new xPoint(view, this);
	m_subPoint->setFlag(ItemIsMovable, false);
	m_subPoint->setFlag(ItemStacksBehindParent);
	m_subPoint->hide();
}

xRegPoint::~xRegPoint()
{
}

int xRegPoint::type() const
{
	return Type;
}

void xRegPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget);
	const qreal r = m_regPoint.radius();
	QPainterPath path;
	path.addEllipse(m_regPoint.center(), r + m_width, r + m_width);

	auto style = m_style;
	const qreal f = viewScaleFactor();

	if (style != xStyle::NoStyle)
	{
		// ѡ��״̬
		if (option->state & QStyle::State_Selected)
		{
			style = xStyle::RegSelected;
			// ѡ��ʱ�滭�߿�
			painter->setPen(QPen(QColor(255, 255, 0, 255), 1.0 / f));
			painter->drawPath(path);
		}

		// ��ͣ״̬
		if (option->state & QStyle::State_MouseOver)
		{
			style = xStyle::RegHovered;
		}

		xStyle::makeStyle(style, &m_pen, &m_brush, f);
	}
	// ��䷶Χ
	painter->fillPath(path, m_brush);
	// �����ĵ� ��
	m_pen.setStyle(Qt::SolidLine);// Ĭ��ѡ��������
	painter->setPen(m_pen);
	// ��������߼���
	qreal w = 2.0 / f;
	QPointF startP1 = { m_regPoint.center().x() - w , m_regPoint.center().y() - w };
	QPointF startP2 = { m_regPoint.center().x() + w ,m_regPoint.center().y() + w };
	QPointF endP1 = { m_regPoint.center().x() + w ,m_regPoint.center().y() - w };
	QPointF endP2 = { m_regPoint.center().x() - w ,m_regPoint.center().y() + w };
	painter->drawLine(startP1, startP2);
	painter->drawLine(endP1, endP2);
	minWidth = w + 1 / f;

	// ѡ��ʱ�滭���Ƶ�
	if ((option->state & QStyle::State_Selected) && (flags() & ItemIsMovable))
	{
		painter->drawLine(startP1, startP2);
		painter->drawLine(endP1, endP2);
	}
}

QRectF xRegPoint::boundingRect() const
{
	if (m_regPoint.isNull())
		return QRectF();

	// ����ͼ�����ӳ��еľ��Σ��������ʵĿ�ȣ������޷���ȷ��ʾ
	// Note�����ʿ������Ϊ2���Ա�����ױ�ѡ��
	const qreal pw = m_pen.widthF() * 2;
	const qreal x = m_regPoint.center().x() - m_regPoint.radius() - m_width - pw;
	const qreal y = m_regPoint.center().y() - m_regPoint.radius() - m_width - pw;
	const qreal w = m_regPoint.radius() + m_width + pw;
	return QRectF(x, y, w + w, w + w);
}

QPainterPath xRegPoint::shape() const
{
	QPainterPath path;
	if (m_regPoint.isNull())
		return path;

	const qreal r = m_regPoint.radius();
	const qreal pw = m_pen.widthF();
	path.addEllipse(m_regPoint.center(), r + m_width + pw, r + m_width + pw);
	return path;
}

void xRegPoint::setSubPoint(const QPointF& p)
{
	m_subPoint->setPt(p);
	m_subPoint->setStyle(xStyle::Measured);
	m_subPoint->show();
}

void xRegPoint::setPoint(const QPointF& p, qreal width)
{
	if (m_regPoint.center() == p  && qFuzzyCompare(width, m_width))
		return;

	prepareGeometryChange();
	m_regPoint.setCenter(p);
	m_width = width;
	update();
}

void xRegPoint::setPt(const QPointF& p)
{
	if (m_regPoint.center() == p )
		return;

	prepareGeometryChange();
	m_regPoint.setCenter(p);
	update();
}

void xRegPoint::setRadius(qreal radius)
{
	if (qFuzzyCompare(radius, m_regPoint.radius()))
		return;

	prepareGeometryChange();
	m_regPoint.setRadius(radius);
	update();
}

void xRegPoint::moveBy(const QPointF& delta)
{
	if (delta.isNull())
		return;

	prepareGeometryChange();
	QPointF p =  m_regPoint.center() + delta;
	m_regPoint.setCenter(p);
	update();
}

QList<QPointF> xRegPoint::controlPoints() const
{
	return {pt()};
}

void xRegPoint::moveCtrlPoint(const QPointF& p, const QPointF& movedPt)
{
	if (Distance(p, pt()) < DELTA_DIST / viewScaleFactor())
	{
		setPt(movedPt);
	}
}

bool xRegPoint::isCtrlPoint(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;
	return  Distance(p, pt()) < DELTA_DIST_2 / viewScaleFactor();
}

void xRegPoint::changeEdgeByPoint(const QPointF& p)
{
	// �ж���С�뾶����С�ڻ��Ĳ��С
	const qreal dw = Distance(p, m_regPoint.center()) - m_regPoint.radius();
	if (minWidth > dw)
		return;
	setRegWidth(dw);
}

bool xRegPoint::isRegionEdge(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	const qreal dw = Distance(p, m_regPoint.center()) - m_regPoint.radius();
	if (fabs(dw - m_width) < DELTA_DIST_2 / viewScaleFactor())
		return true;
	return false;
}

