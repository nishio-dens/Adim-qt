#include <QtGui>
#include "Gauge.h"

Gauge::Gauge(QWidget* parent) : QWidget(parent), rect_(QRect(0,0,0,0))
								,currentValue_(0), minValue_(0), maxValue_(100)
								,baseColor_(QColor(255,255,255,255)), gaugeColor_(QColor(0,0,0,255)) 
{
}

Gauge::Gauge(const QRect& rect, QWidget* parent) : rect_(rect), QWidget(parent)
								,currentValue_(0), minValue_(0), maxValue_(100)
								,baseColor_(QColor(255,255,255,255)), gaugeColor_(QColor(0,0,0,255)) 
{
}

void Gauge::setRange(int minValue, int maxValue, int currentValue)
{
	minValue_ = minValue;
	maxValue_ = maxValue;
	currentValue_ = currentValue;
}

void Gauge::setPosition(const QRect& rect)
{
	rect_ = rect;
}

void Gauge::setCurrentValue(int currentValue)
{
	currentValue_ = currentValue;
}

int Gauge::getCurrentValue()
{
	return currentValue_;
}


