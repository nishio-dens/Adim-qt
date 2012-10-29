#ifndef HGAUGE_H_
#define HGAUGE_H_

#include "Gauge.h"

class HGauge : public Gauge
{
	Q_OBJECT;
public:
	HGauge(QWidget* parent=0) : Gauge(parent) {}
	HGauge(const QRect& rect, QWidget* parent = 0) : Gauge(rect, parent) {}

	void draw(QPainter* painter);
};

#endif

