#pragma once
#include "../../../SDK/SDK.h"

class CAutoParachute
{
public:
	void Run(CTFPlayer* pLocal, CUserCmd* pCmd);
};

ADD_FEATURE(CAutoParachute, AutoParachute);