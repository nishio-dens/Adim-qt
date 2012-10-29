#ifndef VGAUGE_H_
#define VGAUGE_H_

#include "Gauge.h"

class VGauge : public Gauge
{
	Q_OBJECT;
public:
	VGauge(QWidget* parent=0) : Gauge(parent) {}
	VGauge(const QRect& rect, QWidget* parent = 0) : Gauge(rect, parent) {}

	void draw(QPainter* painter);
};

#endif
