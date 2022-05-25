#include "xCircleToLine.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <qfont.h>
#include <QFontMetrics>

xCircleToLine::xCircleToLine(xGraphicView* view, QGraphicsItem* parent, qreal w)
	: xEntity(view, parent),
	circle_width_(w),
	line_2_width_(w)
{

}

int xCircleToLine::type() const
{
	return Type;
}

void xCircleToLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
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

	QPen rect_pen;
	rect_pen.setStyle(Qt::DotLine);
	rect_pen.setColor(Qt::blue);
	rect_pen.setWidthF(m_pen.widthF() / 2.0);

	painter->setPen(m_pen);

	if (is_circle_valid_)
	{
		if (((option->state & QStyle::State_Selected) && (flags() & ItemIsMovable)) || this->m_style == xStyle::Drawing)
		{
			// 边框路径
			QPainterPath path;
			path.addEllipse(circle_cen_, circle_rx_ + circle_width_, circle_ry_ + circle_width_);
			path.addEllipse(circle_cen_, circle_rx_ - circle_width_, circle_ry_ - circle_width_);
			path.closeSubpath();
			painter->fillPath(path, QColor(112, 0, 0));
			painter->setPen(rect_pen);
			painter->drawPath(path);
			painter->setPen(m_pen);
			painter->drawEllipse(circle_cen_, circle_rx_, circle_ry_);
		}
		painter->drawEllipse(entity_circle_cen_, entity_circle_rx_, entity_circle_ry_);
	}
		
	if (is_line_2_valid_)
	{
		if (((option->state & QStyle::State_Selected) && (flags() & ItemIsMovable)) || this->m_style == xStyle::Drawing)
		{
			QPainterPath path;
			path.moveTo(line_2_.p1() + (line_2_ver_ * line_2_width_).toPointF());
			path.lineTo(line_2_.p2() + (line_2_ver_ * line_2_width_).toPointF());
			path.lineTo(line_2_.p2() - (line_2_ver_ * line_2_width_).toPointF());
			path.lineTo(line_2_.p1() - (line_2_ver_ * line_2_width_).toPointF());
			path.closeSubpath();
			painter->fillPath(path, QColor(112, 0, 0));
			painter->setPen(rect_pen);
			painter->drawPath(path);
			painter->setPen(m_pen);
			painter->drawLine(line_2_);
		}
		painter->drawLine(entity_line_2_);

	}
	else
	{
		const qreal w = m_pen.widthF();
		if (is_first_cirle_p_1_)
		{
			painter->fillRect(QRectF(circle_1_.x() - w, circle_1_.y() - w, w + w, w + w), Qt::yellow);// 画两个点
		}
		if (is_first_cirle_p_2_)
		{
			painter->fillRect(QRectF(circle_2_.x() - w, circle_2_.y() - w, w + w, w + w), Qt::yellow);// 画两个点
		}
		if (is_first_cirle_p_3_)
		{
			painter->fillRect(QRectF(circle_3_.x() - w, circle_3_.y() - w, w + w, w + w), Qt::yellow);// 画两个点
		}
	}
	if (is_text_valid_)
	{
		painter->drawLine(m_line_1_);
		painter->drawLine(m_line_2_);
		painter->drawLine(m_line_3_);
		painter->translate(mouse_pos_);
		painter->rotate(angle_);
		painter->drawText(0, -m_pen.widthF(), m_text_);
		painter->resetTransform();
	}

	// 选中时绘画控制点
	if ((option->state & QStyle::State_Selected) && (flags() & ItemIsMovable))
	{
		is_move_able = true;
		const qreal w = m_pen.widthF();
		if (is_circle_valid_)
		{
			painter->fillRect(QRectF(circle_1_.x() - w, circle_1_.y() - w, w + w, w + w), Qt::yellow);// 画两个点
			painter->fillRect(QRectF(circle_2_.x() - w, circle_2_.y() - w, w + w, w + w), Qt::yellow);// 画两个点
			painter->fillRect(QRectF(circle_3_.x() - w, circle_3_.y() - w, w + w, w + w), Qt::yellow);// 画两个点
		}
		if (is_line_2_valid_)
		{
			painter->fillRect(QRectF(line_2_.x1() - w, line_2_.y1() - w, w + w, w + w), Qt::yellow);
			painter->fillRect(QRectF(line_2_.x2() - w, line_2_.y2() - w, w + w, w + w), Qt::yellow);
		}
		if (is_text_valid_)
		{
			painter->fillRect(QRectF(mouse_pos_.x() - w, mouse_pos_.y() - w, w + w, w + w), Qt::yellow);
		}
	}
	else
	{
		is_move_able = false;
	}
}

QRectF xCircleToLine::boundingRect() const
{
	if (is_circle_valid_ == false && is_line_2_valid_ == false && is_text_valid_ == false
		&& is_first_cirle_p_1_ == false && is_first_cirle_p_2_ == false)
		return QRectF();

	// 计算图形在视场中的矩形，包括画笔的宽度，否则无法正确显示
	// Note：画笔宽度设置为2倍以便更容易被选中

	QFontMetricsF my_fm = QFontMetricsF(QFont());
	float the_width = my_fm.width(m_text_) + my_fm.height();

	std::vector<qreal> point_x;
	std::vector<qreal> point_y;

	if (is_first_cirle_p_1_)
	{
		point_x.push_back(circle_1_.x());
		point_y.push_back(circle_1_.y());
	}
	
	if (is_first_cirle_p_2_)
	{
		point_x.push_back(circle_2_.x());
		point_y.push_back(circle_2_.y());
	}

	if (is_first_cirle_p_3_)
	{
		point_x.push_back(circle_3_.x());
		point_y.push_back(circle_3_.y());
	}

	if (is_circle_valid_)
	{
		point_x.push_back(circle_cen_.x() + circle_width_ + circle_rx_);
		point_y.push_back(circle_cen_.y() + circle_width_ + circle_ry_);
		point_x.push_back(circle_cen_.x() - circle_width_ - circle_rx_);
		point_y.push_back(circle_cen_.y() - circle_width_ - circle_ry_);
		point_x.push_back(entity_circle_cen_.x()  + entity_circle_rx_);
		point_y.push_back(entity_circle_cen_.y()  + entity_circle_ry_);
		point_x.push_back(entity_circle_cen_.x() - entity_circle_rx_);
		point_y.push_back(entity_circle_cen_.y() - entity_circle_ry_);
	}
	if (is_line_2_valid_)
	{
		point_x.push_back(line_2_.x1());
		point_y.push_back(line_2_.y1());
		point_x.push_back(line_2_.x2());
		point_y.push_back(line_2_.y2());
		point_x.push_back(entity_line_2_.x1());
		point_y.push_back(entity_line_2_.y1());
		point_x.push_back(entity_line_2_.x2());
		point_y.push_back(entity_line_2_.y2());
		QVector2D line_p1 = QVector2D(line_2_.p1());
		QVector2D line_p2 = QVector2D(line_2_.p2());
		point_x.push_back((line_p1 + line_2_ver_ * line_2_width_).x());
		point_x.push_back((line_p1 - line_2_ver_ * line_2_width_).x());
		point_x.push_back((line_p2 + line_2_ver_ * line_2_width_).x());
		point_x.push_back((line_p2 - line_2_ver_ * line_2_width_).x());
		point_y.push_back((line_p1 + line_2_ver_ * line_2_width_).y());
		point_y.push_back((line_p1 - line_2_ver_ * line_2_width_).y());
		point_y.push_back((line_p2 + line_2_ver_ * line_2_width_).y());
		point_y.push_back((line_p2 - line_2_ver_ * line_2_width_).y());
	}
	if (is_text_valid_)
	{
		point_x.push_back(m_line_1_.x1());
		point_y.push_back(m_line_1_.y1());
		point_x.push_back(m_line_1_.x2());
		point_y.push_back(m_line_1_.y2());
		point_x.push_back(m_line_2_.x1());
		point_y.push_back(m_line_2_.y1());
		point_x.push_back(m_line_2_.x2());
		point_y.push_back(m_line_2_.y2());
		point_x.push_back(m_line_3_.x1());
		point_y.push_back(m_line_3_.y1());
		point_x.push_back(m_line_3_.x2());
		point_y.push_back(m_line_3_.y2());
		point_x.push_back(mouse_pos_.x() + the_width);
		point_x.push_back(mouse_pos_.x() - the_width);
		point_y.push_back(mouse_pos_.y() + the_width);
		point_y.push_back(mouse_pos_.y() - the_width);
	}

	const qreal w = m_pen.widthF() * 2;
	const qreal sx = *(std::min_element(point_x.begin(), point_x.end()));
	const qreal bx = *(std::max_element(point_x.begin(), point_x.end()));
	const qreal sy = *(std::min_element(point_y.begin(), point_y.end()));
	const qreal by = *(std::max_element(point_y.begin(), point_y.end()));
	// 线段的矩形大区域

	return QRectF(sx - w, sy - w, bx - sx + w + w, by - sy + w + w);
}

QPainterPath xCircleToLine::shape() const
{
	QPainterPath path;

	if (is_circle_valid_==false || is_line_2_valid_==false || is_text_valid_==false)
		return path;
	if (is_circle_valid_)
	{
		QPainterPath path_2;
		path_2.addEllipse(entity_circle_cen_, entity_circle_rx_, entity_circle_ry_);
		path.connectPath(path_2);

		if (this->is_move_able)
		{
			QPainterPath path_1;
			path_1.addEllipse(circle_cen_, circle_rx_, circle_ry_);
			path_1.addEllipse(circle_cen_, circle_rx_ + circle_width_, circle_ry_ + circle_width_);
			path_1.addEllipse(circle_cen_, circle_rx_ - circle_width_, circle_ry_ - circle_width_);
			path.connectPath(path_1);
		}
	}
	if (is_line_2_valid_)
	{
		QPainterPath path_1;
		path_1.moveTo(entity_line_2_.p1());
		path_1.lineTo(entity_line_2_.p2());
		path.connectPath(path_1);
		if (this->is_move_able)
		{
			QPainterPath path_2;
			path_2.moveTo(line_2_.p1());
			path_2.lineTo(line_2_.p2());
			QPolygonF rect;
			rect.append(line_2_.p1() + (line_2_ver_ * line_2_width_).toPointF());
			rect.append(line_2_.p2() + (line_2_ver_ * line_2_width_).toPointF());
			rect.append(line_2_.p2() - (line_2_ver_ * line_2_width_).toPointF());
			rect.append(line_2_.p1() - (line_2_ver_ * line_2_width_).toPointF());
			path_2.addPolygon(rect);
			path.connectPath(path_2);
		}
	}
	if (is_text_valid_)
	{
		QPainterPath path_3;
		path_3.moveTo(m_line_1_.p1());
		path_3.lineTo(m_line_1_.p2());
		QPainterPath path_2_t, path_3_t, path_4_t;
		path_2_t.moveTo(m_line_2_.p1());
		path_2_t.lineTo(m_line_2_.p2());
		path_3_t.moveTo(m_line_3_.p1());
		path_3_t.lineTo(m_line_3_.p2());
		path_4_t.moveTo(mouse_pos_);
		path_4_t.addText(mouse_pos_, QFont(), m_text_);
		path_3.connectPath(path_2_t);
		path_3.connectPath(path_3_t);
		path_3.connectPath(path_4_t);
		path.connectPath(path_3);
	}
	return StrokeShapeFromPath(path, m_pen.widthF() * 2);
}

void xCircleToLine::moveBy(const QPointF& delta)
{
}

void xCircleToLine::moveBy2Point(const QPointF& p_after, const QPointF& p_before)
{

	if (is_circle_valid_)
	{

		QPointF delta = p_after - p_before;
		if (delta.isNull())
			return;

		if (fabs(DistancePoint2Ellipse(p_before, circle_cen_, circle_rx_, circle_ry_)) < DELTA_DIST_2 / viewScaleFactor())
		{

			prepareGeometryChange();
			circle_1_ += delta;
			circle_2_ += delta;
			circle_3_ += delta;
			updatePosition();
			update();
			emit posChanged(delta);

		}
		else	if (fabs(DistancePoint2Ellipse(p_before, circle_cen_, circle_rx_ + circle_width_, circle_ry_ + circle_width_)) < DELTA_DIST_2 / viewScaleFactor())
		{

			prepareGeometryChange();
			setFirstCircleWidth(fabs(DistancePoint2Ellipse(p_after, circle_cen_, circle_rx_, circle_ry_)));
			update();
			emit shapeChanged();

		}
		else	if (fabs(DistancePoint2Ellipse(p_before, circle_cen_, circle_rx_ - circle_width_, circle_ry_ - circle_width_)) < DELTA_DIST_2 / viewScaleFactor())
		{

			prepareGeometryChange();
			setFirstCircleWidth(fabs(DistancePoint2Ellipse(p_after, circle_cen_, circle_rx_, circle_ry_)));
			update();
			emit shapeChanged();

		}
	}
	if (is_line_2_valid_)
	{
		QPolygonF rect;
		rect.append(line_2_.p1() + (line_2_ver_ * line_2_width_).toPointF());
		rect.append(line_2_.p2() + (line_2_ver_ * line_2_width_).toPointF());
		rect.append(line_2_.p2() - (line_2_ver_ * line_2_width_).toPointF());
		rect.append(line_2_.p1() - (line_2_ver_ * line_2_width_).toPointF());
		QPointF delta = p_after - p_before;
		if (delta.isNull())
			return;
		QLineF rect_line_1 = QLineF(rect[0], rect[1]);
		QLineF rect_line_2 = QLineF(rect[2], rect[3]);
		if (fabs(DistancePoint2Line(p_before, line_2_)) < DELTA_DIST_2 / viewScaleFactor())
		{

			prepareGeometryChange();
			line_2_.translate(delta);
			updatePosition();
			update();
			emit posChanged(delta);

		}

		else if (fabs(DistancePoint2Line(p_before, rect_line_1)) < DELTA_DIST_2 / viewScaleFactor())
		{

			prepareGeometryChange();
			setSecondLineWidth(fabs(DistancePoint2Line(p_after, line_2_)));
			update();
			emit shapeChanged();

		}
		else if (fabs(DistancePoint2Line(p_before, rect_line_2)) < DELTA_DIST_2 / viewScaleFactor())
		{

			prepareGeometryChange();
			setSecondLineWidth(fabs(DistancePoint2Line(p_after, line_2_)));
			update();
			emit shapeChanged();

		}
	}
	if (is_text_valid_)
	{
		if (fabs(DistancePoint2Line(p_before, m_line_3_)) < DELTA_DIST_2 / viewScaleFactor())
		{

			setMousePos(p_after);

		}
	}
}

QList<QPointF> xCircleToLine::controlPoints() const
{
	return QList{ circle_1_, circle_2_, circle_3_, line_2_.p1(), line_2_.p2(), mouse_pos_ };
}

void xCircleToLine::moveCtrlPoint(const QPointF& pt, const QPointF& movedPt)
{
	if (Distance(pt, circle_1_) < DELTA_DIST / viewScaleFactor())
	{
		setFirstCirclePt1(movedPt);
	}
	else if (Distance(pt, circle_2_) < DELTA_DIST / viewScaleFactor())
	{
		setFirstCirclePt2(movedPt);
	}
	else if (Distance(pt, circle_3_) < DELTA_DIST / viewScaleFactor())
	{
		setFirstCirclePt3(movedPt);
	}
	else if (Distance(pt, line_2_.p1()) < DELTA_DIST / viewScaleFactor())
	{
		setSecondLinePt1(movedPt);
	}
	else if (Distance(pt, line_2_.p2()) < DELTA_DIST / viewScaleFactor())
	{
		setSecondLinePt2(movedPt);
	}
	else if (Distance(pt, mouse_pos_) < DELTA_DIST / viewScaleFactor())
	{
		setMousePos(movedPt);
	}
}

bool xCircleToLine::isCtrlPoint(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	return (Distance(circle_1_, p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(circle_2_, p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(circle_3_, p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(line_2_.p1(), p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(line_2_.p2(), p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(mouse_pos_, p) < DELTA_DIST_2 / viewScaleFactor());
}

bool xCircleToLine::isFittingEntity(const QPointF& p)
{
	if (fabs(DistancePoint2Ellipse(p, entity_circle_cen_, entity_circle_rx_, entity_circle_ry_)) < DELTA_DIST_2 / viewScaleFactor() ||
		fabs(DistancePoint2Line(p, entity_line_2_)) < DELTA_DIST_2 / viewScaleFactor())
		return true;
	return false;
}

void xCircleToLine::setTwoWidth(qreal w)
{
	qreal w_a = qAbs(w);
	if (w_a < 5)
		return;
	if (is_first_cirle_p_1_ && is_first_cirle_p_2_ && is_first_cirle_p_3_)
	{
		if (w_a < circle_rx_ || w_a < circle_ry_)
		{
			w_a = qMin(circle_rx_, circle_ry_);
		}
	}
	circle_width_ = w_a;
	line_2_width_ = w_a;
}

void xCircleToLine::setFirstCircleWidth(qreal w)
{
	qreal w_a = qAbs(w);
	if (w_a < 5)
		return;
	circle_width_ = w_a;
}

void xCircleToLine::setSecondLineWidth(qreal w)
{
	qreal w_a = qAbs(w);
	if (w_a < 5)
		return;
	line_2_width_ = w_a;
}

void xCircleToLine::setFirstCirclePt1AndPt2(const QPointF& p1, const QPointF& p2)
{
	
	if (circle_1_== p1 && circle_2_ == p2)
		return;
	is_first_cirle_p_1_ = true;
	is_first_cirle_p_2_ = true;
	prepareGeometryChange();
	circle_1_ = p1;
	circle_2_ = p2;
	getLineVer();
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleToLine::setFirstCirclePt1(const QPointF& p)
{
	if (circle_1_ == p || is_first_cirle_p_1_ == false)
		return;

	prepareGeometryChange();
	circle_1_ = p;
	getLineVer();
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleToLine::setFirstCirclePt2(const QPointF& p)
{
	if (circle_2_ == p || is_first_cirle_p_2_ == false)
		return;

	prepareGeometryChange();
	circle_2_ = p;
	getLineVer();
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleToLine::setFirstCirclePt3(const QPointF& p)
{
	if (circle_3_ == p)
		return;
	is_circle_valid_ = true;
	is_first_cirle_p_3_ = true;
	prepareGeometryChange();
	circle_3_ = p;
	getLineVer();
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleToLine::setSecondLine(const QLineF& line)
{
	setSecondLine(line.p1(), line.p2());
}

void xCircleToLine::setSecondLine(const QPointF& p1, const QPointF& p2)
{
	is_line_2_valid_ = true;
	if (line_2_.p1() == p1 && line_2_.p2() == p2)
		return;

	prepareGeometryChange();
	line_2_.setPoints(p1, p2);
	getLineVer();
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleToLine::setSecondLinePt1(const QPointF& p)
{
	if (line_2_.p1() == p || is_line_2_valid_ == false)
		return;

	prepareGeometryChange();
	line_2_.setP1(p);
	getLineVer();
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleToLine::setSecondLinePt2(const QPointF& p)
{
	if (line_2_.p2() == p || is_line_2_valid_ == false)
		return;

	prepareGeometryChange();
	line_2_.setP2(p);
	getLineVer();
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleToLine::setSecondLineEntity(const QLineF& line)
{
	setSecondLineEntity(line.p1(), line.p2());
}

void xCircleToLine::setSecondLineEntity(const QPointF& p1, const QPointF& p2)
{
	is_line_2_valid_ = true;
	if (entity_line_2_.p1() == p1 && entity_line_2_.p2() == p2)
		return;

	prepareGeometryChange();
	entity_line_2_.setPoints(p1, p2);
	getLineVer();
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleToLine::setSecondLineEntityPt1(const QPointF& p)
{
	if (entity_line_2_.p1() == p || is_line_2_valid_ == false)
		return;

	prepareGeometryChange();
	entity_line_2_.setP1(p);
	getLineVer();
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleToLine::setSecondLineEntityPt2(const QPointF& p)
{
	if (entity_line_2_.p2() == p || is_line_2_valid_ == false)
		return;

	prepareGeometryChange();
	entity_line_2_.setP2(p);
	getLineVer();
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleToLine::setMousePos(const QPointF& p)
{
	is_text_valid_ = true;
	prepareGeometryChange();
	mouse_pos_ = p;
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleToLine::setMyText(const QString& text)
{
	m_text_ = text;
}

double xCircleToLine::DistancePoint2Ellipse(const QPointF& p, const QPointF& e_cen, double rx, double ry)
{
	if (rx == 0.0 || ry == 0.0)
		return 100000.0;
	double y_x = ry / rx;
	QVector2D e_tran = QVector2D(e_cen), p_tran = QVector2D(p);
	e_tran.setX(e_tran.x() * y_x);
	p_tran.setX(p_tran.x() * y_x);
	QVector2D dir = (p_tran - e_tran).normalized();
	QVector2D intersection = e_tran + ry * dir;
	intersection.setX(intersection.x() / y_x);
	return intersection.distanceToPoint(QVector2D(p));
}

void xCircleToLine::updatePosition()
{
	
	if (is_circle_valid_)
		updateEllipse();
	if (is_circle_valid_ == false || is_line_2_valid_ == false)
		return;
	QLineF line_temp_2 = entity_line_2_;
	if (entity_line_2_.isNull())
	{
		line_temp_2 = line_2_;
	}
	QVector2D line_1_mid;
	if (entity_circle_cen_.isNull())
		line_1_mid = QVector2D(circle_cen_);
	else
		line_1_mid = QVector2D(entity_circle_cen_);
	QVector2D line_2_mid = QVector2D((line_temp_2.p1() + line_temp_2.p2()) / 2.0);

	slope_ = (line_1_mid - line_2_mid).normalized();// 中线方向向量
	angle_ = QLineF(line_1_mid.toPointF(), line_2_mid.toPointF()).angle();// 返回直线的角度，0~360度之内
	angle_ = 360 - angle_;
	if (angle_ < 180 && angle_>90)
		angle_ += 180;
	else if (angle_ < 270 && angle_>180)
		angle_ -= 180;
	//qDebug() << angle;
	// 与方向向量垂直方向
	if (slope_.x() == 0 && slope_.y() == 0)
		return;
	else if (slope_.x() != 0)
	{
		ver_.setY(1.0);
		ver_.setX(-slope_.y() / slope_.x());
	}
	else if (slope_.y() != 0)
	{
		ver_.setX(1.0);
		ver_.setY(-slope_.x() / slope_.y());
	}
	ver_.normalize();

	if (is_text_valid_ == false)
		return;

	m_line_1_.setP1(line_1_mid.toPointF());
	m_line_2_.setP1(line_2_mid.toPointF());
	QVector2D mid_1 = line_1_mid;
	QVector2D mid_2 = line_2_mid;
	QVector2D pos_temp = QVector2D((float)mouse_pos_.x(), (float)mouse_pos_.y());
	float dis = pos_temp.distanceToLine(mid_1, slope_);
	QVector2D v_temp = ver_;
	if (QVector2D::dotProduct(v_temp, pos_temp - mid_1) < 0)
		v_temp = -v_temp;
	QVector2D end_1 = mid_1 + dis * v_temp;
	QVector2D end_2 = mid_2 + dis * v_temp;
	m_line_1_.setP2(end_1.toPointF());
	m_line_2_.setP2(end_2.toPointF());
	float p_dis_1 = pos_temp.distanceToLine(end_1, v_temp);
	float p_dis_2 = pos_temp.distanceToLine(end_2, v_temp);
	float l_dis = end_1.distanceToPoint(end_2) + m_pen.widthF();
	if ((p_dis_1 + p_dis_2) <= l_dis)
	{
		m_line_3_.setP1(end_1.toPointF());
		m_line_3_.setP2(end_2.toPointF());
	}
	else
	{
		if (p_dis_1 < p_dis_2)
		{
			m_line_3_.setP1(pos_temp.toPointF());
			m_line_3_.setP2(end_2.toPointF());
		}
		else
		{
			m_line_3_.setP1(end_1.toPointF());
			m_line_3_.setP2(pos_temp.toPointF());
		}
	}
}

void xCircleToLine::updateEllipse()
{
	if (is_first_cirle_p_1_==false || is_first_cirle_p_2_==false || is_first_cirle_p_3_==false || y_x_ratio_ == 0.0)
		return;
	QPointF p1_tran = circle_1_;
	QPointF p2_tran = circle_2_;
	QPointF p3_tran = circle_3_;
	p1_tran.setX(p1_tran.x() / y_x_ratio_);
	p2_tran.setX(p2_tran.x() / y_x_ratio_);
	p3_tran.setX(p3_tran.x() / y_x_ratio_);

	if (p1_tran == p2_tran || p2_tran == p3_tran || p3_tran == p1_tran)
	{
		return;
	}

	auto va = p2_tran - p1_tran;
	auto vb = p3_tran - p1_tran;
	qreal ra2 = QPointF::dotProduct(va, va) * 0.5;
	qreal rb2 = QPointF::dotProduct(vb, vb) * 0.5;
	qreal crossp = va.x() * vb.y() - va.y() * vb.x();
	// crossp为0则3点在同一直线上
	QPointF c_tran;
	qreal r_tran;
	if (qFuzzyCompare(crossp, 0.0))
	{
		c_tran = QPointF();
		r_tran = 0;
		
	}
	else
	{
		crossp = 1.0 / crossp;
		c_tran.setX((ra2 * vb.y() - rb2 * va.y()) * crossp);
		c_tran.setY((rb2 * va.x() - ra2 * vb.x()) * crossp);
		r_tran = QLineF(c_tran, QPointF(0, 0)).length();
		c_tran += p1_tran;
	}

	c_tran.setX(c_tran.x() * y_x_ratio_);
	circle_cen_ = c_tran;
	circle_ry_ = r_tran;
	circle_rx_ = r_tran * y_x_ratio_;
}

void xCircleToLine::getLineVer()
{
	if (is_line_2_valid_ == false)
		return;
	QVector2D slope_2 = QVector2D(line_2_.p2() - line_2_.p1()).normalized();

	if (slope_2.x() == 0 && slope_2.y() == 0)
		return;
	else if (slope_2.x() != 0)
	{
		line_2_ver_.setY(1.0);
		line_2_ver_.setX(-slope_2.y() / slope_2.x());
	}
	else if (slope_2.y() != 0)
	{
		line_2_ver_.setX(1.0);
		line_2_ver_.setY(-slope_2.x() / slope_2.y());
	}
	line_2_ver_.normalize();
}
