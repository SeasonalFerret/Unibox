#include "AutoParachute.h"

void CAutoParachute::Run(CTFPlayer* pLocal, CUserCmd* pCmd)
{
	if (!Vars::Misc::Movement::AutoParachute.Value || !pLocal || !pLocal->IsAlive() || pLocal->IsAGhost())
		return;

	if (!pLocal->m_bParachuteEquipped() || pLocal->IsOnGround())
		return;

	const Vec3 vVel = pLocal->m_vecVelocity();
	if (pCmd->buttons & IN_JUMP)
		pCmd->buttons &= ~IN_JUMP;

	if (pLocal->InCond(TF_COND_PARACHUTE_ACTIVE))
	{
		if (vVel.z < 20.f)
		{
			pCmd->buttons |= IN_JUMP;
			SDK::Output("Auto Parachute", "Closed chute while falling", { 255, 100, 0 }, Vars::Debug::Logging.Value);
		}
	}
}