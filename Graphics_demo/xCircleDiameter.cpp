#include "xCircleDiameter.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <qfont.h>
#include <QFontMetricsF>

xCircleDiameter::xCircleDiameter(xGraphicView* view, QGraphicsItem* parent, qreal w, qreal y_x)
    :xEntity(view, parent),
    circle_width_(w)
{
    if (y_x > 0)
        y_x_ratio_ = 1.0;
}

int xCircleDiameter::type() const
{
    return Type;
}

void xCircleDiameter::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget); // 不使用这个参数
    auto style = m_style;
	if (style !=xDef::S_NoStyle)
	{
		// 选中状态
		if (option->state & QStyle::State_Selected)
		{
			style = xDef::S_Selected;
		}

		// 悬停状态
		if (option->state & QStyle::State_MouseOver)
		{
			if (style == xDef::S_Selected)
				style = xDef::S_HoverSelected;
			else
				style = xDef::S_Hovered;
		}

		const qreal f = viewScaleFactor();// 获取缩放倍数
		MakeStyle(style, &m_pen, nullptr, f);// 根据类型，设置对应的画笔和画刷
	}

	QPen rect_pen;
	rect_pen.setStyle(Qt::DotLine);
	rect_pen.setColor(Qt::blue);
	rect_pen.setWidthF(m_pen.widthF() / 2.0);

	painter->setPen(m_pen);

	if (is_circle_valid_)
	{
		if (option->state & QStyle::State_Selected && (flags() & ItemIsMovable) || this->m_style == xDef::S_Drawing)
		{
			// 边框路径 画一个外椭圆和一个内椭圆
			QPainterPath path;
			path.addEllipse(circle_cen_,circle_rx_ + circle_width_,circle_ry_ + circle_width_);
			path.addEllipse(circle_cen_, circle_rx_ - circle_width_, circle_ry_ - circle_width_);
			path.closeSubpath();
			painter->fillPath(path, QColor(112, 0, 0, 100));
			painter->setPen(rect_pen);
			painter->drawPath(path);
			painter->setPen(m_pen);
			painter->drawEllipse(circle_cen_, circle_rx_, circle_ry_);
		}
		if (is_circle_success_)
			painter->drawEllipse(entity_circle_cen_,entity_circle_rx_,entity_circle_ry_);
	}
	else
	{
		const qreal w = m_pen.widthF();
		if (is_first_cirle_p_1_)
			painter->fillRect(QRectF(circle_1_.x() - w, circle_1_.y() - w, w + w, w + w), Qt::yellow);
		if (is_first_cirle_p_2_)
			painter->fillRect(QRectF(circle_2_.x() - w, circle_2_.y() - w, w + w, w + w), Qt::yellow);
		if (is_first_cirle_p_3_)
			painter->fillRect(QRectF(circle_3_.x() - w, circle_3_.y() - w, w + w, w + w), Qt::yellow);
	}

	if (is_text_valid_)
	{
		painter->drawLine(m_line);
		painter->drawLine(m_diameter_line);
		painter->translate(mouse_pos_);
		painter->rotate(angle_);
		painter->drawText(0,-m_pen.widthF(),m_text_);
		painter->resetTransform();
	}

	// 选中时绘画控制点
	if ((option->state & QStyle::State_Selected) && (flags() & ItemIsMovable))
	{
		is_move_able = true;
		const qreal w = m_pen.widthF();
		if (is_circle_valid_)
		{
			painter->fillRect(QRectF(circle_1_.x() - w, circle_1_.y() - w, w + w, w + w), Qt::yellow);
			painter->fillRect(QRectF(circle_2_.x() - w, circle_2_.y() - w, w + w, w + w), Qt::yellow);
			painter->fillRect(QRectF(circle_3_.x() - w, circle_3_.y() - w, w + w, w + w), Qt::yellow);
		}
		if(is_text_valid_)
			painter->fillRect(QRectF(mouse_pos_.x() - w, mouse_pos_.y() - w, w + w, w + w), Qt::yellow);
	}
	else
	{
		is_move_able = false;
	}
}

QRectF xCircleDiameter::boundingRect() const
{
	if (!is_circle_valid_ && !is_text_valid_ && !is_first_cirle_p_1_ && !is_first_cirle_p_2_ && !is_first_cirle_p_3_)
		return QRectF();
	// 计算图形在视场中的矩形，包括画笔的宽度，否则无法正确显示
	// Note：画笔宽度设置为2倍以便更容易被选中

	QFontMetricsF my_fm =  QFontMetrics(QFont());
	float the_width = my_fm.width(m_text_) + my_fm.height();

	// 将所有点存入容器计算最大最下x，y值
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
		point_x.push_back(entity_circle_cen_.x() + entity_circle_rx_);
		point_y.push_back(entity_circle_cen_.y() + entity_circle_ry_);
		point_x.push_back(entity_circle_cen_.x() - entity_circle_rx_);
		point_y.push_back(entity_circle_cen_.y() - entity_circle_ry_);
	}
	if (is_text_valid_)
	{
		point_x.push_back(m_line.x1());
		point_y.push_back(m_line.y1());
		point_x.push_back(m_line.x2());
		point_y.push_back(m_line.y2());
		point_x.push_back(m_diameter_line.x1());
		point_y.push_back(m_diameter_line.y1());
		point_x.push_back(m_diameter_line.x2());
		point_y.push_back(m_diameter_line.y2());
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
	// 线段的矩形区域

	return QRectF(sx - w, sy - w, bx - sx + w + w, by - sy + w + w);
}

QPainterPath xCircleDiameter::shape() const
{
	QPainterPath path;

	if (!is_circle_valid_  || !is_text_valid_)
		return path;

	if (is_circle_valid_)
	{
		QPainterPath path2;
		path2.addEllipse(entity_circle_cen_,entity_circle_rx_,entity_circle_ry_);
		path.connectPath(path2);

		if (this->is_move_able)
		{
			QPainterPath path1;
			path1.addEllipse(circle_cen_,circle_rx_,circle_ry_);
			path1.addEllipse(circle_cen_,circle_rx_ + circle_width_,circle_ry_ + circle_width_);
			path1.addEllipse(circle_cen_,circle_rx_ - circle_width_,circle_ry_ - circle_width_);
			path.connectPath(path1);
		}
	}
	if (is_text_valid_)
	{
		QPainterPath path3;
		path3.moveTo(m_diameter_line.p1());
		path3.lineTo(m_diameter_line.p2());
		QPainterPath path3_1, path3_2;
		path3_1.moveTo(m_line.p1());
		path3_1.lineTo(m_line.p2());

		path3_2.moveTo(mouse_pos_);
		path3_2.addText(mouse_pos_,QFont(),m_text_);
		path3.connectPath(path3_1);
		path3.connectPath(path3_2);
		path.connectPath(path3);
	}
	return StrokeShapeFromPath(path,m_pen.widthF() *2);
}

void xCircleDiameter::moveBy(const QPointF& delta)
{
}

void xCircleDiameter::moveBy2Point(const QPointF& p_after, const QPointF& p_before)
{
	if (is_circle_valid_)
	{
		QPointF delta = p_after - p_before;
		if (delta.isNull())
			return;
		if (fabs(DistancePoint2Ellipse(p_before, circle_cen_, circle_rx_, circle_ry_) < DELTA_DIST_2 / viewScaleFactor()))
		{
			prepareGeometryChange();
			circle_1_ += delta;
			circle_2_ += delta;
			circle_3_ += delta;
			updatePosition();
			update();
			emit posChanged(delta);
		}
		else if (fabs(DistancePoint2Ellipse(p_before, circle_cen_, circle_rx_ + circle_width_, circle_ry_ + circle_width_)) < DELTA_DIST_2 / viewScaleFactor())
		{
			prepareGeometryChange();
			setCircleWidth(fabs(DistancePoint2Ellipse(p_after, circle_cen_, circle_rx_, circle_ry_)));
			update();
			emit shapeChanged();
		}
		else if (fabs(DistancePoint2Ellipse(p_before, circle_cen_, circle_rx_ - circle_width_, circle_ry_ - circle_width_)) < DELTA_DIST_2 / viewScaleFactor())
		{
			prepareGeometryChange();
			setCircleWidth(fabs(DistancePoint2Ellipse(p_after, circle_cen_, circle_rx_, circle_ry_)));
			update();
			emit shapeChanged();
		}
	}
	if (is_text_valid_)
	{
		if (fabs(MyDistancePoint2Line(p_before, m_line)) < DELTA_DIST_2 / viewScaleFactor())
		{
			setMousePos(p_after);
		}
	}
}

QList<QPointF> xCircleDiameter::controlPoints() const
{
	return QList<QPointF> { circle_1_, circle_2_, circle_3_,  mouse_pos_ };
}

void xCircleDiameter::moveCtrlPoint(const QPointF& pt, const QPointF& movedPt)
{
	if (Distance(pt, circle_1_) < DELTA_DIST / viewScaleFactor())
	{
		setCirclePt1(movedPt);
	}
	else if (Distance(pt, circle_2_) < DELTA_DIST / viewScaleFactor())
	{
		setCirclePt2(movedPt);
	}
	else if (Distance(pt, circle_3_) < DELTA_DIST / viewScaleFactor())
	{
		setCirclePt3(movedPt);
	}
	else if (Distance(pt, mouse_pos_) < DELTA_DIST / viewScaleFactor())
	{
		setMousePos(movedPt);
	}
}

bool xCircleDiameter::isCtrlPoint(const QPointF& p) const
{
	if (!(flags() & ItemIsMovable))
		return false;

	return (Distance(circle_1_, p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(circle_2_, p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(circle_3_, p) < DELTA_DIST_2 / viewScaleFactor()
		|| Distance(mouse_pos_, p) < DELTA_DIST_2 / viewScaleFactor());
}

bool xCircleDiameter::isFittingEntity(const QPointF& p)
{
	if (fabs(DistancePoint2Ellipse(p, entity_circle_cen_, entity_circle_rx_, entity_circle_ry_)) < DELTA_DIST_2 / viewScaleFactor() )
		return true;
	return false;
}

void xCircleDiameter::setTwoWidth(qreal w)
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
}

void xCircleDiameter::setCircleWidth(qreal w)
{
	qreal w_a = qAbs(w);
	if (w_a < 5)
		return;
	circle_width_ = w_a;
}

void xCircleDiameter::setCirclePt1(const QPointF& p)
{
	if (circle_1_ == p || is_first_cirle_p_1_ == false)
		return;

	prepareGeometryChange();
	circle_1_ = p;
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleDiameter::setCirclePt2(const QPointF& p)
{
	if (circle_2_ == p || is_first_cirle_p_2_ == false)
		return;

	prepareGeometryChange();
	circle_2_ = p;
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleDiameter::setCirclePt3(const QPointF& p)
{
	if (circle_3_ == p)
		return;
	is_circle_valid_ = true;
	is_first_cirle_p_3_ = true;
	prepareGeometryChange();
	circle_3_ = p;
	//emit CircleChange(circle_1_, circle_2_, circle_3_, 1);
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleDiameter::setCircleEntity(QPointF cen_draw, QPointF cen_x_draw, QPointF cen_y_draw)
{
	if (is_first_cirle_p_1_ == false)
		return;

	prepareGeometryChange();
	entity_circle_cen_ = cen_draw;
	entity_circle_rx_ = cen_x_draw.x() - cen_draw.x();
	entity_circle_ry_ = cen_y_draw.y() - cen_draw.y();
	updatePosition();
	update();
	emit shapeChanged();
}



void xCircleDiameter::setCirclePt1AndPt2(const QPointF& p1, const QPointF& p2)
{
	if (circle_1_ == p1 && circle_2_ == p2)
		return;
	is_first_cirle_p_1_ = true;
	is_first_cirle_p_2_ = true;
	prepareGeometryChange();
	circle_1_ = p1;
	circle_2_ = p2;
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleDiameter::setMyCircle(QPointF cen, qreal r, QPointF cen_draw, QPointF cen_x_draw, QPointF cen_y_draw)
{
	circle_cen_dt_1 = cen;
	r_dt_1 = r;
	setCircleEntity(cen_draw, cen_x_draw, cen_y_draw);

}

void xCircleDiameter::setMousePos(const QPointF& p)
{
	is_text_valid_ = true;
	prepareGeometryChange();
	mouse_pos_ = p;
	updatePosition();
	update();
	emit shapeChanged();
}

void xCircleDiameter::setMyText(const QString& text)
{
	if (is_circle_success_ )
		m_text_ = text;
	else
	{
		m_text_ = "抽取失败";
	}
	update();
}

void xCircleDiameter::setIsSuccess(bool is_s)
{
	is_circle_success_ = is_s;
	setMyText(m_text_);
}

qreal xCircleDiameter::getRadius(const QPointF& point1, const QPointF& point2, const QPointF& point3)
{
	QPointF P1 = point1;
	QPointF P2 = point2;
	QPointF P3 = point3;
	P1.setX(P1.x() / y_x_ratio_); // 转换椭圆
	P2.setX(P2.x() / y_x_ratio_);
	P3.setX(P3.x() / y_x_ratio_);

	QPointF vra = P2 - P1;
	QPointF vrb = P3 - P1;
	double ra2 = (vra.x() * vra.x() + vra.y() * vra.y()) * 0.5;
	double rb2 = (vrb.x() * vrb.x() + vrb.y() * vrb.y()) * 0.5;

	double crossp = vra.x() * vrb.y() - vra.y() * vrb.x();
	if (fabs(crossp) < 0.0000000001)
	{
		qDebug() << "三点共线拟合不了";
		return -1;
	}
	crossp = 1 / crossp;

	circle_centerPt.setX((ra2 * vrb.y() - rb2 * vra.y()) * crossp + P1.x());
	circle_centerPt.setY((rb2 * vra.x() - ra2 * vrb.x()) * crossp + P1.y());
	return sqrt(pow(P1.x() - circle_centerPt.x(), 2) + pow(P1.y() - circle_centerPt.y(), 2));
}

//void xCircleDiameter::sendFitting()
//{
//	if (is_circle_valid_)
//	{
//		emit CircleChange(circle_1_, circle_2_, circle_3_, 1);
//	}
//}

double xCircleDiameter::DistancePoint2Ellipse(const QPointF& p, const QPointF& e_cen, double rx, double ry)
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

void xCircleDiameter::updatePosition()
{
	if (is_circle_valid_)
		updateEllipse();
	if (!is_circle_valid_)
		return;

	qreal radius = getRadius(circle_1_, circle_2_, circle_3_);

	QVector2D mousePt = QVector2D(mouse_pos_ ); // 鼠标点
	mousePt.setX(mousePt.x() / y_x_ratio_); // 转为椭圆点
	QVector2D centerPt = QVector2D(circle_centerPt);

	QVector2D normal = (mousePt - centerPt).normalized(); // 法向量
	normal.setX(normal.x() *y_x_ratio_); // 还原回圆的点
	QVector2D normalV ;// 垂直于法向量

	if (normal.x() == 0 && normal.y() == 0)
		return;
	else if (normal.x() != 0)
	{
		normalV.setY(1.0);
		normalV.setX(-normal.y() / normal.x());
	}
	else if (normal.y() != 0)
	{
		normalV.setX(1.0);
		normalV.setY(-normal.x() / normal.y());
	}
	normalV.normalize();


	angle_ = QLineF(mouse_pos_, circle_cen_).angle(); // 逆时针0-360度  椭圆的中心点和鼠标点
	angle_ = 360 - angle_;
	if (angle_ < 180 && angle_ > 90)
		angle_ += 180;
	if (angle_ < 270 && angle_>180)
		angle_ -= 180;

	// 计算圆的直径两个点
	// 开始点为圆心与鼠标点的连线获得一个单位向量，在以圆心+向量*半径得出直径
	QVector2D circleStartPoint = centerPt + normalV * radius;
	QVector2D circleEndPoint = centerPt - normalV * radius;

	// 还原回圆的点
	circleStartPoint.setX(circleStartPoint.x() * y_x_ratio_);
	circleEndPoint.setX(circleEndPoint.x() * y_x_ratio_);

	m_diameter_line.setP1(circleStartPoint.toPointF());
	m_diameter_line.setP2(circleEndPoint.toPointF());

	m_line.setP1(circle_cen_);
	m_line.setP2(mouse_pos_);

	emit ComputeDistance(circle_cen_dt_1, r_dt_1);

}

void xCircleDiameter::updateEllipse()
{
	if (is_first_cirle_p_1_ == false || is_first_cirle_p_2_ == false || is_first_cirle_p_3_ == false || y_x_ratio_ == 0.0)
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

