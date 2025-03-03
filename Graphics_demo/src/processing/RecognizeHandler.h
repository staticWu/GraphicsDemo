#pragma once

#include <QObject>

class xInterCircle;

/**
 * @brief 模拟计算、识别等任务的处理
*/
class RecognizeHandler : public QObject
{
	Q_OBJECT

public:
	explicit RecognizeHandler(QObject *parent = nullptr);
	~RecognizeHandler();

public slots:
	void calcCircle(xInterCircle *ic);

signals:
	void calcDone(bool suc);

private:
};
