#include "stdafx.h"
#include "ExpirationTimer.h"

// Purpose:     Constructs a new ExpirationTimer.
//
// Parameters:	duration	Length of time before timer expires
ExpirationTimer::ExpirationTimer(const float duration)
{
	timerDuration = duration;
	startTime = 0;
}

// Purpose:     Starts the ExpirationTimer.
void ExpirationTimer::Start()
{
	startTime = clock();
}

// Purpose:     Determine if the ExpirationTimer has expired (duration has passed).
//
// Returns:		True if the timer has expired (duration has passed).
bool ExpirationTimer::HasExpired() const
{
	int endTime = startTime + (timerDuration * CLOCKS_PER_SEC);
	return endTime < clock();
}

// Purpose:     Change the duration of the timer.
//
// Parameters:	duration	The duration to change the length of the timer to
void ExpirationTimer::SetDuration(const float duration)
{
	timerDuration = duration;
}
