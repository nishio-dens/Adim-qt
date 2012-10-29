#include <QtGui>
#include "HGauge.h"

void HGauge::draw(QPainter* painter)
{
	painter->fillRect(rect_, QBrush(baseColor_) );

	if( currentValue_ < minValue_ )
		currentValue_ = minValue_;
	else if( currentValue_ > maxValue_ )
		currentValue_ = maxValue_;

	QRect percentRect(rect_.x(), rect_.y(),
			(int)( ((double)((double)currentValue_ / (double)maxValue_)) * (double)rect_.width() ),
			rect_.height() );
	painter->fillRect(percentRect, QBrush(gaugeColor_) );
}

