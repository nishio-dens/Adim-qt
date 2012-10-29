#ifndef GAUGE_H_
#define GAUGE_H_

#include <QWidget>
#include <QRect>
#include <QPainter>
#include <QColor>

class Gauge : public QWidget
{
	Q_OBJECT
public:
	Gauge(QWidget* parent=0);
	Gauge(const QRect& rect, QWidget* parent=0);

	void setRange(int minValue, int maxValue, int currentValue = 0);
	void setPosition(const QRect& rect);
	void setGaugeColor(const QColor& baseColor, const QColor& gaugeColor);
	void setCurrentValue(int currentValue);
	int  getCurrentValue();
	virtual void draw(QPainter* painter) = 0;
protected:
	QRect rect_;
	int currentValue_;
	int minValue_;
	int maxValue_;
	QColor baseColor_;
	QColor gaugeColor_;
};

#endif
