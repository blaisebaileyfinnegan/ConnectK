#include "stdafx.h"
#include "ExpirationTimer.h"

ExpirationTimer::ExpirationTimer(const float duration)
{
	timerDuration = duration;
	startTime = 0;
}

void ExpirationTimer::Start()
{
	startTime = clock();
}

bool ExpirationTimer::HasExpired() const
{
	int endTime = startTime + (timerDuration * CLOCKS_PER_SEC);
	return endTime < clock();
}

void ExpirationTimer::SetDuration(const float duration)
{
	timerDuration = duration;
}
