#pragma once

#include <QGraphicsItem>
#include <QBrush>
#include "xTypeDef.h"
#include "xEntity.h"
#include <qvector2d.h>

class xText : public xEntity
{
	Q_OBJECT

public:
	Q_DISABLE_COPY(xText)
		explicit xText(QPointF line_1, QPointF line_2, xGraphicView* view, QGraphicsItem* parent = nullptr);
	virtual ~xText() = default;
	// 自定义实体类型枚举
	enum { Type = ET_Line };
	// 重写函数，返回当前的类型值
	int type() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	void moveBy(const QPointF& delta) override;
	QList<QPointF> controlPoints() const override;
	void moveCtrlPoint(const QPointF& pt, const QPointF& movedPt) override;
	bool isCtrlPoint(const QPointF& p) const override;
	bool isFittingEntity(const QPointF& p) override;

	void setMousePos(const QPointF& p);
	void setMyText(const QString& text);
	void updatePosition();// 更新位置

	void setMidPoint(QPointF l_1, QPointF l_2);

private:
	QPointF mouse_pos;// 鼠标位置点
	QString m_text;// 文字
	QLineF m_line_1;
	QLineF m_line_2;
	QLineF m_line_3;
	QPointF line_1_mid;// 第一条线的中点
	QPointF line_2_mid;// 第二条线的中点
	QVector2D slope;// 中线方向向量
	QVector2D ver;// 与方向向量垂直方向
	float angle;// 记录文字需要旋转的角度
};

