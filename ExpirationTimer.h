#include "stdafx.h"


#ifndef EXPIRATION_TIMER_H
#define EXPIRATION_TIMER_H

class ExpirationTimer
{
private:
	int startTime;
	float timerDuration;
public:
	ExpirationTimer::ExpirationTimer(const float duration);
	void ExpirationTimer::Start();
	bool ExpirationTimer::HasExpired() const;
	void ExpirationTimer::SetDuration(const float duration);
};

#endif