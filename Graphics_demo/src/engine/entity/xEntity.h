#pragma once

#include <QList>
#include <QGraphicsObject>
#include "xTypeDef.h"
#include "xGraphicView.h"

/**
 * @brief 基于QGraphicsItem的带有画笔和样式的图元虚基类
*/
class xEntity : public QGraphicsObject
{
	Q_OBJECT

public:
	enum EntityType {
		ET_Unknown = UserType,
		// 不带范围的图元
		ET_Entity_Start,
		ET_Line,
		ET_Circle,
		ET_Arc,
		ET_Point,
		ET_Entity_End,
		// 带范围的图元
		ET_Region_Start,
		ET_RegLine,
		ET_RegCircle,
		ET_RegArc,
		ET_RegPoint,
		ET_RegRect,
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
	Q_ENUM(EntityType)

	Q_DISABLE_COPY(xEntity)
	explicit xEntity(xGraphicView *view, QGraphicsItem *parent = nullptr);
	virtual ~xEntity() = default;
	
	/**
	 * @brief 移动图元
	 * @param delta 移动增量，需传入scene坐标中的值
	*/
	virtual void moveBy(const QPointF &delta) = 0;
	// 返回图元的绘画控制点
	virtual QList<QPointF> controlPoints() const = 0;
	/**
	 * @brief 移动图元的一个控制点
	 * @param pt 控制点位置，用于判断哪个控制点，需传入scene坐标中的值
	 * @param movedPt 移动后的点位置，需传入scene坐标中的值
	*/
	virtual void moveCtrlPoint(const QPointF &pt, const QPointF &movedPt) = 0;
	// 判断是否是控制点，需传入scene坐标中的值
	virtual bool isCtrlPoint(const QPointF &p) const = 0;

	xDef::EntityStatus status() const { return m_status; }
	void setStatus(xDef::EntityStatus status) { m_status = status; }
	QPen pen() const { return m_pen; }
	void setPen(const QPen &pen);
	xDef::Style style() const { return m_style; }
	void setStyle(xDef::Style style);

	bool isThisVisible() const { return isVisible() && opacity() > 0; }

public slots:
	// 仅设置此图元的可见性，不影响children的可见性
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
	xDef::Style m_style = xDef::S_NoStyle;
	xDef::EntityStatus m_status = xDef::ES_Init;
	xGraphicView *m_view = nullptr;

private:
	inline void init();
};

inline void xEntity::init()
{
	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	// 忽略parent的透明度，以实现parent不可见而children可见
	setFlag(ItemIgnoresParentOpacity);
	setAcceptHoverEvents(true);
}

inline qreal xEntity::viewScaleFactor() const
{
	if (m_view == nullptr) return 1.0;
	return m_view->scaleFactor();
}
