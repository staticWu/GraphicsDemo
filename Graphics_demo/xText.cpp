#include "xText.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <qfont.h>
#include <QFontMetrics>


xText::xText(QPointF line_1, QPointF line_2, xGraphicView* view, QGraphicsItem* parent)
	: xEntity(view, parent),
	line_1_mid(line_1),
	line_2_mid(line_2)
{
	slope = QVector2D(line_2_mid - line_1_mid).normalized();// 中线方向向量
	angle = QLineF(line_1_mid, line_2_mid).angle();// 返回直线的角度，0~360度之内
	angle = 360 - angle;
	if (angle < 180 && angle>90)
		angle += 180;
	else if (angle < 270 && angle>180)
		angle -= 180;
	//qDebug() << angle;
	// 与方向向量垂直方向
	if (slope.x() == 0 && slope.y() == 0)
		return;
	else if (slope.x() != 0)
	{
		ver.setY(1.0);
		ver.setX(-slope.y() / slope.x());
	}
	else if (slope.y() != 0)
	{
		ver.setX(1.0);
		ver.setY(-slope.x() / slope.y());
	}
	ver.normalize();
}

int xText::type() const
{
	return Type;
}

void xText::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(widget); // 不使用这个参数

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

		const qreal f = viewScaleFactor();// 获取缩放倍数
		xStyle::makeStyle(style, &m_pen, nullptr, f);// 根据类型，设置对应的画笔和画刷
	}

	painter->setPen(m_pen);
	painter->drawLine(m_line_1);
	painter->drawLine(m_line_2);
	painter->drawLine(m_line_3);
	painter->translate(mouse_pos);
	painter->rotate(angle);
	painter->drawText(0,-m_pen.widthF(), m_text);
	painter->resetTransform();

	//// 选中时绘画控制点
	//if ((option->state & QStyle::State_Selected) && (flags() & ItemIsMovable))
	//{
	//	const qreal w = m_pen.widthF();
	//	painter->fillRect(QRectF(m_line.x1() - w, m_line.y1() - w, w + w, w + w), Qt::yellow);// 画两个点
	//	painter->fillRect(QRectF(m_line.x2() - w, m_line.y2() - w, w + w, w + w), Qt::yellow);
	//}
}

// 所占范围
QRectF xText::boundingRect() const
{
	if (m_line_1.isNull() || mouse_pos.isNull()|| m_line_2.isNull()|| m_line_3.isNull())
		return QRectF();

	QFontMetricsF my_fm = QFontMetricsF(QFont());
	float the_width = my_fm.width(m_text);
	float the_height = my_fm.height();
	the_width += the_height;

	// 计算图形在视场中的矩形，包括画笔的宽度，否则无法正确显示
	// Note：画笔宽度设置为2倍以便更容易被选中
	const qreal w = m_pen.widthF() * 2;
	const qreal sx = std::min({ m_line_1.x1(), m_line_1.x2(), m_line_2.x1(), m_line_2.x2(),m_line_3.x1(), m_line_3.x2(),mouse_pos.x() - the_width });
	const qreal bx = std::max({ m_line_1.x1(), m_line_1.x2(),  m_line_2.x1(), m_line_2.x2(),m_line_3.x1(), m_line_3.x2(),mouse_pos.x() + the_width });
	const qreal sy = std::min({ m_line_1.y1(), m_line_1.y2(), m_line_2.y1(), m_line_2.y2(),m_line_3.y1(), m_line_3.y2(),mouse_pos.y() - the_width });
	const qreal by = std::max({ m_line_1.y1(), m_line_1.y2(), m_line_2.y1(), m_line_2.y2(),m_line_3.y1(), m_line_3.y2(),mouse_pos.y() + the_width });
	return QRectF(sx - w, sy - w, bx - sx + w + w, by - sy + w + w);
}

QPainterPath xText::shape() const
{
	QPainterPath path;
	if (m_line_1.isNull() || mouse_pos.isNull() || m_line_2.isNull() || m_line_3.isNull())
		return path;
	path.moveTo(m_line_1.p1());
	path.lineTo(m_line_1.p2());
	QPainterPath path_2, path_3, path_4;
	path_2.moveTo(m_line_2.p1());
	path_2.lineTo(m_line_2.p2());
	path_3.moveTo(m_line_3.p1());
	path_3.lineTo(m_line_3.p2());
	path_4.moveTo(mouse_pos);
	path_4.addText(mouse_pos, QFont(), m_text);
	path.connectPath(path_2);
	path.connectPath(path_3);
	path.connectPath(path_4);
	
	return StrokeShapeFromPath(path, m_pen.widthF() * 2);
}

void xText::moveBy(const QPointF& delta)
{
	if (delta.isNull())
		return;

	prepareGeometryChange();
	mouse_pos += delta;// 更新鼠标点
	updatePosition();
	update();
	emit posChanged(delta);
}

QList<QPointF> xText::controlPoints() const
{
	return  { mouse_pos };
}

void xText::moveCtrlPoint(const QPointF& pt, const QPointF& movedPt)
{
	if (Distance(pt, mouse_pos) < DELTA_DIST / viewScaleFactor())
	{
		prepareGeometryChange();
		mouse_pos = movedPt;
		updatePosition();
		update();
	}
}

bool xText::isCtrlPoint(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	return (Distance(mouse_pos, p) < DELTA_DIST_2 / viewScaleFactor());
}

bool xText::isFittingEntity(const QPointF& p)
{
	const qreal dw = fabs(Distance(p, mouse_pos));
	if (fabs(dw) < DELTA_DIST_2 / viewScaleFactor())
		return true;
	return false;
}

void xText::setMousePos(const QPointF& p)
{
	prepareGeometryChange();
	mouse_pos = p;
	updatePosition();
	update();
}

void xText::setMyText(const QString& text)
{
	m_text = text;
}

void xText::updatePosition()
{
	m_line_1.setP1(line_1_mid);
	m_line_2.setP1(line_2_mid);
	QVector2D mid_1 = QVector2D((float)line_1_mid.x(), (float)line_1_mid.y());
	QVector2D mid_2 = QVector2D((float)line_2_mid.x(), (float)line_2_mid.y());
	QVector2D pos_temp = QVector2D((float)mouse_pos.x(), (float)mouse_pos.y());
	float dis = pos_temp.distanceToLine(mid_1, slope);
	QVector2D v_temp = ver;
	if (QVector2D::dotProduct(v_temp, pos_temp - mid_1) < 0)
		v_temp = -v_temp;
	QVector2D end_1 = mid_1 + dis * v_temp;
	QVector2D end_2 = mid_2 + dis * v_temp;
	m_line_1.setP2(end_1.toPointF());
	m_line_2.setP2(end_2.toPointF());
	float p_dis_1 = pos_temp.distanceToLine(end_1, v_temp);
	float p_dis_2 = pos_temp.distanceToLine(end_2, v_temp);
	float l_dis = end_1.distanceToPoint(end_2) + m_pen.widthF();
	if ((p_dis_1 + p_dis_2) <= l_dis)
	{
		m_line_3.setP1(end_1.toPointF());
		m_line_3.setP2(end_2.toPointF());
	}
	else
	{
		if (p_dis_1 < p_dis_2)
		{
			m_line_3.setP1(pos_temp.toPointF());
			m_line_3.setP2(end_2.toPointF());
		}
		else
		{
			m_line_3.setP1(end_1.toPointF());
			m_line_3.setP2(pos_temp.toPointF());
		}
	}
}

void xText::setMidPoint(QPointF l_1, QPointF l_2)
{
	prepareGeometryChange();
	line_1_mid = l_1;
	line_2_mid = l_2;
	slope = QVector2D(line_2_mid - line_1_mid).normalized();// 中线方向向量
	angle = QLineF(line_1_mid, line_2_mid).angle();// 返回直线的角度，0~360度之内
	angle = 360 - angle;
	if (angle < 180 && angle>90)
		angle += 180;
	else if (angle < 270 && angle>180)
		angle -= 180;
	//qDebug() << angle;
	// 与方向向量垂直方向
	if (slope.x() == 0 && slope.y() == 0)
		return;
	else if (slope.x() != 0)
	{
		ver.setY(1.0);
		ver.setX(-slope.y() / slope.x());
	}
	else if (slope.y() != 0)
	{
		ver.setX(1.0);
		ver.setY(-slope.x() / slope.y());
	}
	ver.normalize();
	updatePosition();
	update();
}
