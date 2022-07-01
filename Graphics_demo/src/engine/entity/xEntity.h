#pragma once

#include <QList>
#include <QGraphicsObject>
#include "xTypeDef.h"
#include "xGraphicView.h"

/**
 * @brief ����QGraphicsItem�Ĵ��л��ʺ���ʽ��ͼԪ�����
*/
class xEntity : public QGraphicsObject
{
	Q_OBJECT

public:
	enum {
		ET_Unknown = UserType,
		// ������Χ��ͼԪ
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
		// ����Χ��ͼԪ
		ET_Region_Start,
		ET_RegLine,
		ET_RegCircle,
		ET_RegArc,
		ET_RegPoint,
		ET_RegStraightLine,
		ET_Region_End,
		// �󶨵���ͼԪ
		ET_InterSingle_Start,
		ET_InterLine,
		ET_InterCircle,
		ET_InterArc,
		ET_InterPoint,
		ET_InterSingle_End,
		// ������ͼԪ
		ET_InterCouple_Start,
		ET_InterLineAndLine,
		ET_InterLineAndCircle,
		ET_InterLineAndArc,
		ET_InterLineAndPoint,
		ET_InterCircleAndCircle,
		ET_InterCircleAndArc,
		ET_InterCircleAndPoint,
		ET_InterPointAndPoint,
		ET_InterCouple_End,
		ET_RegCircleDiameter,
	};

	Q_DISABLE_COPY(xEntity)
	explicit xEntity(xGraphicView *view, QGraphicsItem *parent = nullptr);
	virtual ~xEntity() = default;

	// �ƶ�ͼԪ��deltaΪ�ƶ�����
	virtual void moveBy(const QPointF &delta) = 0;
	// �ƶ�ͼԪ�������ƶ�������λ��
	virtual void moveBy2Point(const QPointF& p_after, const QPointF& p_before) {}
	// ����ͼԪ�Ļ滭���Ƶ�
	virtual QList<QPointF> controlPoints() const = 0;
	/**
	 * @brief �ƶ�ͼԪ��һ�����Ƶ�
	 * @param pt ͼԪ�ϵĿ��Ƶ�
	 * @param movedPt �ƶ���ĵ�λ��
	*/
	virtual void moveCtrlPoint(const QPointF &pt, const QPointF &movedPt) = 0;
	// �ж��Ƿ��ǿ��Ƶ�
	virtual bool isCtrlPoint(const QPointF &p) const = 0;
	// �ж��Ƿ�����ϵ�ʵ��
	virtual bool isFittingEntity(const QPointF& p) { return false; };

	QPen pen() const { return m_pen; }
	void setPen(const QPen &pen);
	xDef::Style style() const { return m_style; }
	void setStyle(xDef::Style style);
	// �Ƿ�ɼ���͸����
	bool isThisVisible() const { return isVisible() && opacity() > 0; }

public slots:
	// �����ô�ͼԪ�Ŀɼ��ԣ���Ӱ�캢�ӵĿɼ���
	void setThisVisible(bool visible);
	void showThis() { setThisVisible(true); }
	void hideThis() { setThisVisible(false); }

signals:
	void selectedChanged(bool selected);
	void cursorChanged(const QCursor &newCursor);
	void flagsChanged(const GraphicsItemFlags &newFlags);
	// λ�øı��źţ���moveBy��������
	void posChanged(const QPointF &delta);
	// ��״�ı��ź�
	void shapeChanged();

protected:
	// ���ڴ������QGraphicsItem���ݵĸı䣬������Ӧ�ź�
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
	// ��ȡ��ͼ������ϵ��
	inline qreal viewScaleFactor() const;

	QPen m_pen;
	xDef::Style m_style = xDef::S_NoStyle;
	xGraphicView *m_view = nullptr;

private:
	inline void init();
};

inline void xEntity::init()
{
	// ���ñ�־��Ĭ��Ϊ����Ϊʹ��
	setFlag(ItemIsMovable);// �ܹ��ƶ�
	setFlag(ItemIsSelectable);
	// ���Ը�ĸ��͸���ȣ���ʵ�ָ�ĸ���ɼ������ӿɼ�
	setFlag(ItemIgnoresParentOpacity);
	setAcceptHoverEvents(true);// ������ͣ�¼�
}

inline qreal xEntity::viewScaleFactor() const
{
	// ͨ��view��ת�������ȡ����ϵ����m11��ŵ�ֵ��ʾ���ű���
	return m_view->transform().m11();
}
