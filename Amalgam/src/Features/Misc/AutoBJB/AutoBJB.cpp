#include "AutoBJB.h"

#include "../../Simulation/ProjectileSimulation/ProjectileSimulation.h"
#include "../../Simulation/MovementSimulation/MovementSimulation.h"

void CAutoBJB::Reset()
{
	g_bBJBSequenceActive = false;
	g_bBJBHasExploded = false;
	m_bSequenceActive = false;
	m_bParachuteDeployedByBJB = false;
	m_iParachuteDeployTick = 0;
	m_iTrackedRocketIndex = -1;
	m_iCurrentTick = 0;
	m_iImpactTick = 0;
	m_bHasExploded = false;
	m_vImpactPoint = {};
}

bool CAutoBJB::FindRocketInWorld(CTFPlayer* pLocal)
{
	if (!pLocal)
		return false;

	auto& vProjectiles = H::Entities.GetGroup(EntityEnum::WorldProjectile);
	for (auto pEntity : vProjectiles)
	{
		auto nClassID = pEntity->GetClassID();
		if (nClassID != ETFClassID::CTFProjectile_Rocket &&
			nClassID != ETFClassID::CTFProjectile_EnergyBall &&
			nClassID != ETFClassID::CTFBaseRocket &&
			nClassID != ETFClassID::CTFFlameRocket)
			continue;

		auto pRocket = pEntity->As<CTFBaseRocket>();
		if (!pRocket)
			continue;

		auto pLauncher = pRocket->m_hLauncher()->As<CTFWeaponBase>();
		if (!pLauncher)
			continue;

		auto pOwner = pLauncher->m_hOwner()->As<CTFPlayer>();
		if (pOwner != pLocal)
			continue;

		int iIndex = pEntity->entindex();
		if (m_vHandledRockets.contains(iIndex))
			continue;

		m_iTrackedRocketIndex = iIndex;
		m_vHandledRockets.insert(iIndex);
		return true;
	}
	return false;
}

bool CAutoBJB::PredictRocketImpact(CBaseEntity* pRocket, int& iImpactTicks, Vec3& vImpactPoint)
{
	if (!pRocket)
		return false;

	auto pRocketEnt = pRocket->As<CTFBaseRocket>();
	if (!pRocketEnt)
		return false;

	const Vec3 vOrigin = pRocketEnt->m_vecOrigin();
	const Vec3 vVel = pRocketEnt->m_vInitialVelocity();
	const float flSpeed = vVel.Length();
	if (flSpeed < 0.1f)
		return false;

	const Vec3 vDir = vVel / flSpeed;

	CGameTrace trace = {};
	CTraceFilterCollideable filter(pRocket);
	const Vec3 vHull = Vec3::Get(2.f);
	SDK::TraceHull(vOrigin, vOrigin + vDir * 8192.f, -vHull, vHull, MASK_SOLID, &filter, &trace);

	if (!trace.DidHit())
		return false;

	vImpactPoint = trace.endpos;
	const float flDist = vOrigin.DistTo(vImpactPoint);
	iImpactTicks = TIME_TO_TICKS(flDist / flSpeed);
	return iImpactTicks > 0;
}

void CAutoBJB::Run(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, CUserCmd* pCmd)
{
	if (!Vars::Misc::Movement::AutoBJB.Value || !pLocal || !pLocal->IsAlive() || pLocal->IsAGhost())
	{
		Reset();
		return;
	}

	if (!pLocal->m_bParachuteEquipped())
	{
		Reset();
		return;
	}

	if (!m_bSequenceActive)
	{
		bool bFound = FindRocketInWorld(pLocal);

		if (!bFound && pWeapon && G::Attacking)
		{
			switch (pWeapon->GetWeaponID())
			{
			case TF_WEAPON_ROCKETLAUNCHER:
			case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
			case TF_WEAPON_PARTICLE_CANNON:
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			auto& vProjectiles = H::Entities.GetGroup(EntityEnum::WorldProjectile);
			std::unordered_set<int> vActive;
			for (auto pEntity : vProjectiles)
				vActive.insert(pEntity->entindex());

			for (auto it = m_vHandledRockets.begin(); it != m_vHandledRockets.end(); )
			{
				if (!vActive.contains(*it))
					it = m_vHandledRockets.erase(it);
				else
					++it;
			}
			return;
		}

		Vec3 vImpactPoint = {};
		int iImpactTicks = 0;
		bool bPredicted = false;

		if (m_iTrackedRocketIndex != -1)
		{
			auto& vProjectiles = H::Entities.GetGroup(EntityEnum::WorldProjectile);
			for (auto pEntity : vProjectiles)
			{
				if (pEntity->entindex() == m_iTrackedRocketIndex)
				{
					bPredicted = PredictRocketImpact(pEntity, iImpactTicks, vImpactPoint);
					break;
				}
			}
		}
		else if (pWeapon && G::Attacking)
		{
			ProjectileInfo tProjInfo = {};
			if (F::ProjSim.GetInfo(pLocal, pWeapon, pCmd->viewangles, tProjInfo, ProjSimEnum::Redirect | ProjSimEnum::NoRandomAngles) &&
				F::ProjSim.Initialize(tProjInfo, false))
			{
				const Vec3 vOrigin = F::ProjSim.GetOrigin();
				const Vec3 vVel = F::ProjSim.GetVelocity();
				const float flSpeed = vVel.Length();
				if (flSpeed > 0.1f)
				{
					const Vec3 vDir = vVel / flSpeed;
					CGameTrace trace = {};
					CTraceFilterCollideable filter(pLocal);
					SDK::Trace(vOrigin, vOrigin + vDir * 8192.f, MASK_SOLID, &filter, &trace);

					if (trace.DidHit())
					{
						vImpactPoint = trace.endpos;
						const float flDist = vOrigin.DistTo(vImpactPoint);
						iImpactTicks = TIME_TO_TICKS(flDist / flSpeed);
						bPredicted = iImpactTicks > 0;
					}
				}
			}
		}

		if (!bPredicted)
		{
			m_iTrackedRocketIndex = -1;
			return;
		}

		m_vImpactPoint = vImpactPoint;
		m_iImpactTick = iImpactTicks;
		m_iCurrentTick = 0;
		m_bSequenceActive = true;
		m_bHasExploded = false;
		g_bBJBHasExploded = false;
		m_bParachuteDeployedByBJB = false;
		m_iParachuteDeployTick = 0;
		g_bBJBSequenceActive = true;

		SDK::Output("Auto BJB", std::format("Impact in {} ticks, dist {:.0f} Hu", iImpactTicks, vImpactPoint.DistTo(pLocal->GetAbsOrigin())).c_str(), { 0, 255, 0 }, Vars::Debug::Logging.Value);
		return;
	}

	m_iCurrentTick++;

	if (m_iTrackedRocketIndex != -1)
	{
		auto& vProjectiles = H::Entities.GetGroup(EntityEnum::WorldProjectile);
		bool bRocketExists = false;
		for (auto pEntity : vProjectiles)
		{
			if (pEntity->entindex() == m_iTrackedRocketIndex)
			{
				bRocketExists = true;
				break;
			}
		}

		if (!bRocketExists)
		{
			if (m_iCurrentTick >= m_iImpactTick)
			{
				m_bHasExploded = true;
				g_bBJBHasExploded = true;
			}
			else
			{
				if (m_bParachuteDeployedByBJB && pLocal->InCond(TF_COND_PARACHUTE_ACTIVE))
					pCmd->buttons |= IN_JUMP;
				SDK::Output("Auto BJB", "Rocket lost before impact, aborting", { 255, 50, 50 }, Vars::Debug::Logging.Value);
				Reset();
				return;
			}
			m_iTrackedRocketIndex = -1;
		}
	}
	else if (m_iCurrentTick >= m_iImpactTick)
	{
		m_bHasExploded = true;
		g_bBJBHasExploded = true;
	}

	int iOpenTick = m_iImpactTick + Vars::Misc::Movement::BJBOpenBefore.Value;
	if (!m_bParachuteDeployedByBJB && m_iCurrentTick >= iOpenTick)
	{
		const int iTicksUntilExplosion = std::max(m_iImpactTick - m_iCurrentTick, 0);

		MoveStorage tMoveStorage;
		if (F::MoveSim.Initialize(pLocal, tMoveStorage, false))
		{
			for (int i = 0; i < iTicksUntilExplosion; i++)
				F::MoveSim.RunTick(tMoveStorage);

			if (pLocal->IsOnGround() || pLocal->m_vecOrigin().DistTo(m_vImpactPoint) > TF_ROCKET_RADIUS)
			{
				F::MoveSim.Restore(tMoveStorage);
				Reset();
				return;
			}
			F::MoveSim.Restore(tMoveStorage);
		}

		pCmd->buttons |= IN_JUMP;
		m_bParachuteDeployedByBJB = true;
		m_iParachuteDeployTick = m_iCurrentTick;
		SDK::Output("Auto BJB", std::format("Deployed at tick {} (impact at {})", m_iCurrentTick, m_iImpactTick).c_str(), { 0, 255, 0 }, Vars::Debug::Logging.Value);
	}

	if (m_bParachuteDeployedByBJB && m_iParachuteDeployTick > 0
		&& m_iCurrentTick - m_iParachuteDeployTick >= Vars::Misc::Movement::BJBMaxCloseTicks.Value)
	{
		if (pLocal->InCond(TF_COND_PARACHUTE_ACTIVE))
		{
			pCmd->buttons |= IN_JUMP;
			SDK::Output("Auto BJB", std::format("Closed via open duration ({})", Vars::Misc::Movement::BJBMaxCloseTicks.Value).c_str(), { 255, 100, 0 }, Vars::Debug::Logging.Value);
		}
		m_bParachuteDeployedByBJB = false;
		Reset();
		return;
	}

	if (m_iCurrentTick > m_iImpactTick + 20)
	{
		if (m_bParachuteDeployedByBJB && pLocal->InCond(TF_COND_PARACHUTE_ACTIVE))
		{
			pCmd->buttons |= IN_JUMP;
			m_bParachuteDeployedByBJB = false;
		}
		else
			m_bParachuteDeployedByBJB = false;
		Reset();
	}
}