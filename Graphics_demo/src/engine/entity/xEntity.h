#pragma once

#include <QList>
#include <QGraphicsObject>
#include "xTypeDef.h"
#include "xStyle.h"
#include "xGraphicView.h"

/**
 * @brief 基于QGraphicsItem的带有画笔和样式的图元虚基类
*/
class xEntity : public QGraphicsObject
{
	Q_OBJECT

public:
	enum {
		ET_Unknown = UserType,
		// 不带范围的图元
		ET_Entity_Start,
		ET_Line,
		ET_Circle,
		ET_Arc,
		ET_Point,
		ET_StraightLine,
		ET_ConcentricCircle,
		ET_VerticalLine,
		ET_Text,
		ET_Entity_End,
		// 带范围的图元
		ET_Region_Start,
		ET_RegLine,
		ET_RegCircle,
		ET_RegArc,
		ET_RegPoint,
		ET_RegStraightLine,
		ET_Region_End,
		// 绑定单个图元
		ET_InterSingle_Start,
		ET_InterLine,
		ET_InterCircle,
		ET_InterArc,
		ET_InterPoint,
		ET_InterSingle_End,
		// 绑定两个图元
		ET_InterCouple_Start,
		ET_InterLineAndLine,
		ET_InterLineAndCircle,
		ET_InterLineAndArc,
		ET_InterLineAndPoint,
		ET_InterCircleAndCircle,
		ET_InterCircleAndArc,
		ET_InterCircleAndPoint,
		ET_InterPointAndPoint,
		ET_InterCouple_End
	};

	Q_DISABLE_COPY(xEntity)
	explicit xEntity(xGraphicView *view, QGraphicsItem *parent = nullptr);
	virtual ~xEntity() = default;

	// 移动图元，delta为移动增量
	virtual void moveBy(const QPointF &delta) = 0;
	// 返回图元的绘画控制点
	virtual QList<QPointF> controlPoints() const = 0;
	/**
	 * @brief 移动图元的一个控制点
	 * @param pt 图元上的控制点
	 * @param movedPt 移动后的点位置
	*/
	virtual void moveCtrlPoint(const QPointF &pt, const QPointF &movedPt) = 0;
	// 判断是否是控制点
	virtual bool isCtrlPoint(const QPointF &p) const = 0;
	// 判断是否是拟合的实体
	virtual bool isFittingEntity(const QPointF& p) { return false; };

	QPen pen() const { return m_pen; }
	void setPen(const QPen &pen);
	xStyle::Style style() const { return m_style; }
	void setStyle(xStyle::Style style);
	// 是否可见与透明度
	bool isThisVisible() const { return isVisible() && opacity() > 0; }

public slots:
	// 仅设置此图元的可见性，不影响孩子的可见性
	void setThisVisible(bool visible);
	void showThis() { setThisVisible(true); }
	void hideThis() { setThisVisible(false); }

signals:
	void selectedChanged(bool selected);
	void cursorChanged(const QCursor &newCursor);
	void flagsChanged(const GraphicsItemFlags &newFlags);
	// 位置改变信号，由moveBy函数发送
	void posChanged(const QPointF &delta);
	// 形状改变信号
	void shapeChanged();

protected:
	// 用于处理基类QGraphicsItem传递的改变，发送相应信号
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	// 获取视图的缩放系数
	inline qreal viewScaleFactor() const;

	QPen m_pen;
	xStyle::Style m_style = xStyle::NoStyle;
	xGraphicView *m_view = nullptr;

private:
	inline void init();
};

inline void xEntity::init()
{
	// 设置标志，默认为设置为使能
	setFlag(ItemIsMovable);// 能够移动
	setFlag(ItemIsSelectable);
	// 忽略父母的透明度，以实现父母不可见而孩子可见
	setFlag(ItemIgnoresParentOpacity);
	setAcceptHoverEvents(true);// 接受悬停事件
}

inline qreal xEntity::viewScaleFactor() const
{
	// 通过view的转换矩阵获取缩放系数，m11存放的值表示缩放倍数
	return m_view->transform().m11();
}
