#pragma once
#include "../../../SDK/SDK.h"

#include <unordered_set>

inline bool g_bBJBSequenceActive = false;
inline bool g_bBJBHasExploded = false;

class CAutoBJB
{
private:
	std::unordered_set<int> m_vHandledRockets;

	bool m_bSequenceActive = false;
	bool m_bHasExploded = false;
	int m_iTrackedRocketIndex = -1;
	int m_iImpactTick = 0;
	int m_iCurrentTick = 0;

	bool m_bParachuteDeployedByBJB = false;
	int m_iParachuteDeployTick = 0;
	Vec3 m_vImpactPoint = {};

	[[nodiscard]] bool FindRocketInWorld(CTFPlayer* pLocal);
	[[nodiscard]] bool PredictRocketImpact(CBaseEntity* pRocket, int& iImpactTicks, Vec3& vImpactPoint);
	void Reset();

public:
	void Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd);
};

ADD_FEATURE(CAutoBJB, AutoBJB);