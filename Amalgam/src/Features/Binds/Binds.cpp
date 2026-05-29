#include "Binds.h"

#include "../ImGui/Menu/Menu.h"
#include "../Configs/Configs.h"
#include <functional>

#define IsType(t) pBase->m_iType == typeid(t).hash_code()

template <class T>
static inline void SetMain(BaseVar*& pBase, int iBind)
{
	auto pVar = pBase->As<T>();

	if (pVar->Map.contains(iBind))
		pVar->Value = pVar->Map[iBind];
}
#define Set(t, b) if (IsType(t)) SetMain<t>(pBase, b);

static constexpr ETFCond k_CondLookup[] = {
	TF_COND_AFTERBURN_IMMUNE,
	TF_COND_AIMING,
	TF_COND_AIR_CURRENT,
	TF_COND_BALLOON_HEAD,
	TF_COND_BLAST_IMMUNE,
	TF_COND_BLASTJUMPING,
	TF_COND_BLEEDING,
	TF_COND_BULLET_IMMUNE,
	TF_COND_BURNING,
	TF_COND_BURNING_PYRO,
	TF_COND_CANNOT_SWITCH_FROM_MELEE,
	TF_COND_COMPETITIVE_LOSER,
	TF_COND_COMPETITIVE_WINNER,
	TF_COND_CRITBOOSTED,
	TF_COND_CRITBOOSTED_BONUS_TIME,
	TF_COND_CRITBOOSTED_CARD_EFFECT,
	TF_COND_CRITBOOSTED_CTF_CAPTURE,
	TF_COND_CRITBOOSTED_DEMO_CHARGE,
	TF_COND_CRITBOOSTED_FIRST_BLOOD,
	TF_COND_CRITBOOSTED_ON_KILL,
	TF_COND_CRITBOOSTED_PUMPKIN,
	TF_COND_CRITBOOSTED_RAGE_BUFF,
	TF_COND_CRITBOOSTED_RUNE_TEMP,
	TF_COND_CRITBOOSTED_USER_BUFF,
	TF_COND_DEFENSEBUFF,
	TF_COND_DEFENSEBUFF_HIGH,
	TF_COND_DEFENSEBUFF_NO_CRIT_BLOCK,
	TF_COND_DEMO_BUFF,
	TF_COND_DISGUISE_WEARINGOFF,
	TF_COND_DISGUISED,
	TF_COND_DISGUISED_AS_DISPENSER,
	TF_COND_DISGUISING,
	TF_COND_ENERGY_BUFF,
	TF_COND_FEIGN_DEATH,
	TF_COND_FIRE_IMMUNE,
	TF_COND_FREEZE_INPUT,
	TF_COND_GAS,
	TF_COND_GRAPPLED_BY_PLAYER,
	TF_COND_GRAPPLED_TO_PLAYER,
	TF_COND_GRAPPLINGHOOK,
	TF_COND_GRAPPLINGHOOK_BLEEDING,
	TF_COND_GRAPPLINGHOOK_LATCHED,
	TF_COND_GRAPPLINGHOOK_SAFEFALL,
	TF_COND_HALLOWEEN_BOMB_HEAD,
	TF_COND_HALLOWEEN_GHOST_MODE,
	TF_COND_HALLOWEEN_GIANT,
	TF_COND_HALLOWEEN_HELL_HEAL,
	TF_COND_HALLOWEEN_IN_HELL,
	TF_COND_HALLOWEEN_KART,
	TF_COND_HALLOWEEN_KART_CAGE,
	TF_COND_HALLOWEEN_KART_DASH,
	TF_COND_HALLOWEEN_QUICK_HEAL,
	TF_COND_HALLOWEEN_SPEED_BOOST,
	TF_COND_HALLOWEEN_THRILLER,
	TF_COND_HALLOWEEN_TINY,
	TF_COND_HASRUNE,
	TF_COND_HEALING_DEBUFF,
	TF_COND_HEALTH_BUFF,
	TF_COND_HEALTH_OVERHEALED,
	TF_COND_INVULNERABLE,
	TF_COND_INVULNERABLE_CARD_EFFECT,
	TF_COND_INVULNERABLE_HIDE_UNLESS_DAMAGED,
	TF_COND_INVULNERABLE_USER_BUFF,
	TF_COND_INVULNERABLE_WEARINGOFF,
	TF_COND_KING_BUFFED,
	TF_COND_KNOCKED_INTO_AIR,
	TF_COND_LOST_FOOTING,
	TF_COND_MAD_MILK,
	TF_COND_MARKEDFORDEATH,
	TF_COND_MARKEDFORDEATH_SILENT,
	TF_COND_MEDIGUN_DEBUFF,
	TF_COND_MEDIGUN_SMALL_BLAST_RESIST,
	TF_COND_MEDIGUN_SMALL_BULLET_RESIST,
	TF_COND_MEDIGUN_SMALL_FIRE_RESIST,
	TF_COND_MEDIGUN_UBER_BLAST_RESIST,
	TF_COND_MEDIGUN_UBER_BULLET_RESIST,
	TF_COND_MEDIGUN_UBER_FIRE_RESIST,
	TF_COND_MEGAHEAL,
	TF_COND_MELEE_ONLY,
	TF_COND_MINICRITBOOSTED_ON_KILL,
	TF_COND_MVM_BOT_STUN_RADIOWAVE,
	TF_COND_NOHEALINGDAMAGEBUFF,
	TF_COND_OBSCURED_SMOKE,
	TF_COND_OFFENSEBUFF,
	TF_COND_PARACHUTE_ACTIVE,
	TF_COND_PARACHUTE_DEPLOYED,
	TF_COND_PASSTIME_INTERCEPTION,
	TF_COND_PASSTIME_PENALTY_DEBUFF,
	TF_COND_PHASE,
	TF_COND_PLAGUE,
	TF_COND_POWERUPMODE_DOMINANT,
	TF_COND_PREVENT_DEATH,
	TF_COND_PURGATORY,
	TF_COND_RADIUSHEAL,
	TF_COND_RADIUSHEAL_ON_DAMAGE,
	TF_COND_REGENONDAMAGEBUFF,
	TF_COND_REPROGRAMMED,
	TF_COND_ROCKETPACK,
	TF_COND_RUNE_AGILITY,
	TF_COND_RUNE_HASTE,
	TF_COND_RUNE_IMBALANCE,
	TF_COND_RUNE_KING,
	TF_COND_RUNE_KNOCKOUT,
	TF_COND_RUNE_PLAGUE,
	TF_COND_RUNE_PRECISION,
	TF_COND_RUNE_REFLECT,
	TF_COND_RUNE_REGEN,
	TF_COND_RUNE_RESIST,
	TF_COND_RUNE_STRENGTH,
	TF_COND_RUNE_SUPERNOVA,
	TF_COND_RUNE_VAMPIRE,
	TF_COND_SAPPED,
	TF_COND_SELECTED_TO_TELEPORT,
	TF_COND_SHIELD_CHARGE,
	TF_COND_SNIPERCHARGE_RAGE_BUFF,
	TF_COND_SODAPOPPER_HYPE,
	TF_COND_SPEED_BOOST,
	TF_COND_STEALTHED,
	TF_COND_STEALTHED_BLINK,
	TF_COND_STEALTHED_USER_BUFF,
	TF_COND_STEALTHED_USER_BUFF_FADING,
	TF_COND_STUNNED,
	TF_COND_SWIMMING_CURSE,
	TF_COND_SWIMMING_NO_EFFECTS,
	TF_COND_TAUNTING,
	TF_COND_TEAM_GLOWS,
	TF_COND_TELEPORTED,
	TF_COND_TMPDAMAGEBONUS,
	TF_COND_URINE,
	TF_COND_ZOOMED,
};

static std::unordered_map<BaseVar*, bool> s_mVars = {};
static bool s_bUI = false, s_bMenu = false;

static inline void LoopVars(int iBind, std::vector<BaseVar*>& vVars = G::Vars)
{
	const bool bDefault = iBind == DEFAULT_BIND;
	for (auto pBase : vVars)
	{
		if (s_mVars.contains(pBase) || (pBase->m_iFlags & NOBIND || (pBase->m_iFlags & NOSAVE && !Vars::Config::LoadDebugSettings.Value)) && !bDefault)
			continue;

		s_mVars[pBase];
		Set(bool, iBind)
		else Set(int, iBind)
		else Set(float, iBind)
		else Set(IntRange_t, iBind)
		else Set(FloatRange_t, iBind)
		else Set(std::string, iBind)
		else Set(VA_LIST(std::vector<std::pair<std::string, Color_t>>), iBind)
		else Set(Color_t, iBind)
		else Set(Gradient_t, iBind)
		else Set(Vec3, iBind)
		else Set(DragBox_t, iBind)
		else Set(WindowBox_t, iBind)
	}
}

static inline void GetBinds(int iParent, CTFPlayer* pLocal, CTFWeaponBase* pWeapon, std::vector<Bind_t>& vBinds, bool bManage = true)
{
	if (vBinds.empty())
		return;

	for (int i = 0; i < vBinds.size(); i++)
	{
		auto& tBind = vBinds[i];
		if (iParent != tBind.m_iParent || !tBind.m_bEnabled)
			continue;

		if (bManage)
		{
			switch (tBind.m_iType)
			{
			case BindEnum::Key:
			{
				bool bKey = false;
				switch (tBind.m_iInfo)
				{
				case BindEnum::KeyEnum::Hold: bKey = U::KeyHandler.Down(tBind.m_iKey, false, &tBind.m_tKeyStorage); break;
				case BindEnum::KeyEnum::Toggle: bKey = U::KeyHandler.Pressed(tBind.m_iKey, false, &tBind.m_tKeyStorage); break;
				case BindEnum::KeyEnum::DoubleClick: bKey = U::KeyHandler.Double(tBind.m_iKey, false, &tBind.m_tKeyStorage); break;
				}
				bKey &= !s_bUI || s_bMenu && (!F::Menu.m_bWindowHovered || tBind.m_iKey != VK_LBUTTON && tBind.m_iKey != VK_RBUTTON); // allow in menu

				switch (tBind.m_iInfo)
				{
				case BindEnum::KeyEnum::Hold:
					tBind.m_bActive = bKey;
					if (tBind.m_bNot)
						tBind.m_bActive = !tBind.m_bActive;
					break;
				case BindEnum::KeyEnum::Toggle:
				case BindEnum::KeyEnum::DoubleClick:
					if (bKey)
						tBind.m_bActive = !tBind.m_bActive;
				}
				break;
			}
			case BindEnum::Class:
			{
				const int iClass = pLocal ? pLocal->m_iClass() : TF_CLASS_UNDEFINED;
				switch (tBind.m_iInfo)
				{
				case BindEnum::ClassEnum::Scout: { tBind.m_bActive = iClass == TF_CLASS_SCOUT; break; }
				case BindEnum::ClassEnum::Soldier: { tBind.m_bActive = iClass == TF_CLASS_SOLDIER; break; }
				case BindEnum::ClassEnum::Pyro: { tBind.m_bActive = iClass == TF_CLASS_PYRO; break; }
				case BindEnum::ClassEnum::Demoman: { tBind.m_bActive = iClass == TF_CLASS_DEMOMAN; break; }
				case BindEnum::ClassEnum::Heavy: { tBind.m_bActive = iClass == TF_CLASS_HEAVY; break; }
				case BindEnum::ClassEnum::Engineer: { tBind.m_bActive = iClass == TF_CLASS_ENGINEER; break; }
				case BindEnum::ClassEnum::Medic: { tBind.m_bActive = iClass == TF_CLASS_MEDIC; break; }
				case BindEnum::ClassEnum::Sniper: { tBind.m_bActive = iClass == TF_CLASS_SNIPER; break; }
				case BindEnum::ClassEnum::Spy: { tBind.m_bActive = iClass == TF_CLASS_SPY; break; }
				}
				if (tBind.m_bNot)
					tBind.m_bActive = !tBind.m_bActive;
				break;
			}
			case BindEnum::WeaponType:
			{
				tBind.m_bActive = tBind.m_iInfo == BindEnum::WeaponTypeEnum::Throwable ? G::Throwing : tBind.m_iInfo + 1 == int(SDK::GetWeaponType(pWeapon));
				if (tBind.m_bNot)
					tBind.m_bActive = !tBind.m_bActive;
				break;
			}
			case BindEnum::ItemSlot:
			{
				tBind.m_bActive = tBind.m_iInfo == (pWeapon ? pWeapon->GetSlot() : -1);
				if (tBind.m_bNot)
					tBind.m_bActive = !tBind.m_bActive;
				break;
			}
			case BindEnum::Misc:
			{
				switch (tBind.m_iInfo)
				{
				case BindEnum::MiscEnum::Spectated:
				case BindEnum::MiscEnum::SpectatedFirst:
				case BindEnum::MiscEnum::SpectatedThird:
				{
					bool bFirst = false, bThird = false;
					if (auto pResource = H::Entities.GetResource())
					{
						int iLocal = I::EngineClient->GetLocalPlayer();
						for (int n = 1; n <= I::EngineClient->GetMaxClients(); n++)
						{
							auto pPlayer = I::ClientEntityList->GetClientEntity(n)->As<CTFPlayer>();

							if (iLocal == n || pResource->IsFakePlayer(n)
								|| !pPlayer || !pPlayer->IsPlayer() || pPlayer->IsAlive() || pPlayer->IsDormant()
								|| pResource->m_iTeam(iLocal) != pResource->m_iTeam(n))
								continue;

							int iObserverTarget = pPlayer->m_hObserverTarget().GetEntryIndex();
							int iObserverMode = pPlayer->m_iObserverMode();
							if (iObserverTarget != iLocal)
								continue;

							switch (iObserverMode)
							{
							case OBS_MODE_FIRSTPERSON: bFirst = true; break;
							case OBS_MODE_THIRDPERSON: bThird = true; break;
							}
						}
					}

					switch (tBind.m_iInfo)
					{
					case BindEnum::MiscEnum::Spectated: tBind.m_bActive = bFirst || bThird; break;
					case BindEnum::MiscEnum::SpectatedFirst: tBind.m_bActive = bFirst; break;
					case BindEnum::MiscEnum::SpectatedThird: tBind.m_bActive = bThird; break;
					}
					break;
				}
				default:
					{
						int iIndex = tBind.m_iInfo - BindEnum::MiscEnum::ConditionStart;
						if (iIndex >= 0 && iIndex < std::size(k_CondLookup))
							tBind.m_bActive = pLocal ? pLocal->InCond(k_CondLookup[iIndex]) : false;
						break;
					}
				}
				if (tBind.m_bNot)
					tBind.m_bActive = !tBind.m_bActive;
				break;
			}
			}
		}

		if (tBind.m_bActive)
		{
			GetBinds(i, pLocal, pWeapon, vBinds, bManage);
			LoopVars(i, tBind.m_vVars);
		}
	}
}

void CBinds::SetVars(CTFPlayer* pLocal, CTFWeaponBase* pWeapon, bool bManage)
{
	s_mVars.clear();
	s_bUI = I::EngineVGui->IsGameUIVisible() || I::MatSystemSurface->IsCursorVisible() && !I::EngineClient->IsPlayingDemo();
#ifndef TEXTMODE
	s_bMenu = F::Menu.m_bIsOpen && !ImGui::GetIO().WantTextInput && !F::Menu.m_bInKeybind;
#else
	s_bMenu = F::Menu.m_bIsOpen && !F::Menu.m_bInKeybind;
#endif
	GetBinds(DEFAULT_BIND, pLocal, pWeapon, m_vBinds, bManage);
	LoopVars(DEFAULT_BIND);

	m_bDisplay = F::Menu.m_bIsOpen || Vars::Menu::BindWindow.Value && !s_bUI;
}

void CBinds::Run()
{
	if (G::Unload)
		return;

	auto pLocal = H::Entities.GetLocal();
	auto pWeapon = H::Entities.GetWeapon();

	for (auto& tBind : m_vBinds)
	{	// don't delay inputs for binds
		if (tBind.m_iType != BindEnum::Key)
			continue;

		auto& tKey = tBind.m_tKeyStorage;

		bool bOldIsDown = tKey.m_bIsDown;
		bool bOldIsPressed = tKey.m_bIsPressed;
		bool bOldIsDouble = tKey.m_bIsDouble;
		bool bOldIsReleased = tKey.m_bIsReleased;

		U::KeyHandler.StoreKey(tBind.m_iKey, &tKey);

		tKey.m_bIsDown = tKey.m_bIsDown || bOldIsDown;
		tKey.m_bIsPressed = tKey.m_bIsPressed || bOldIsPressed;
		tKey.m_bIsDouble = tKey.m_bIsDouble || bOldIsDouble;
		tKey.m_bIsReleased = tKey.m_bIsReleased || bOldIsReleased;
	}

	SetVars(pLocal, pWeapon);

	for (auto& tBind : m_vBinds)
	{	// clear inputs for binds
		if (tBind.m_iType != BindEnum::Key)
			continue;

		auto& tKey = tBind.m_tKeyStorage;

		U::KeyHandler.StoreKey(tBind.m_iKey, &tKey);
	}
}



bool CBinds::GetBind(int iID, Bind_t* pBind)
{
	if (iID > DEFAULT_BIND && iID < m_vBinds.size())
	{
		*pBind = m_vBinds[iID];
		return true;
	}

	return false;
}

void CBinds::AddBind(int iBind, Bind_t& tBind)
{
	if (iBind == DEFAULT_BIND || iBind >= m_vBinds.size())
		m_vBinds.push_back(tBind);
	else
		m_vBinds[iBind] = tBind;
}

#define HasType(t, b) IsType(t) && pBase->As<t>()->contains(b)

template <class T>
static inline void RemoveMain(BaseVar*& pBase, int iBind)
{
	auto pVar = pBase->As<T>();

	std::unordered_map<int, T> mMap = {};
	for (auto it = pVar->Map.begin(); it != pVar->Map.end(); it++)
	{
		int iKey = it->first;
		auto tVal = it->second;
		if (iBind == iKey)
			continue;
		else if (iBind < iKey)
			iKey--;
		mMap[iKey] = tVal;
	}
	pVar->Map = mMap;
}
#define Remove(t, b) if (IsType(t)) RemoveMain<t>(pBase, b);

void CBinds::RemoveBind(int iBind, bool bForce)
{
	if (!bForce)
	{
		for (auto& pBase : G::Vars)
		{
			if (HasType(bool, iBind)
				|| HasType(int, iBind)
				|| HasType(float, iBind)
				|| HasType(IntRange_t, iBind)
				|| HasType(FloatRange_t, iBind)
				|| HasType(std::string, iBind)
				|| HasType(VA_LIST(std::vector<std::pair<std::string, Color_t>>), iBind)
				|| HasType(Color_t, iBind)
				|| HasType(Gradient_t, iBind)
				|| HasType(Vec3, iBind)
				|| HasType(DragBox_t, iBind)
				|| HasType(WindowBox_t, iBind))
				return;
		}
		for (auto& tBind : F::Binds.m_vBinds)
		{
			if (tBind.m_iParent == iBind)
				return;
		}
	}

	std::vector<int> vErases = {};
	std::function<void(int)> fSearchBinds = [&](int iIndex)
	{
		for (int iBind = 0; iBind < m_vBinds.size(); iBind++)
		{
			auto& tBind = m_vBinds[iBind];
			if (iIndex == tBind.m_iParent && iIndex != iBind)
				fSearchBinds(iBind);
		}
		vErases.push_back(iIndex);
	};
	auto fRemoveBind = [&](int iIndex)
	{
		if (iIndex < m_vBinds.size())
			m_vBinds.erase(std::next(m_vBinds.begin(), iIndex));
		for (auto& tBind : m_vBinds)
		{
			if (tBind.m_iParent != DEFAULT_BIND && tBind.m_iParent > iIndex)
				tBind.m_iParent--;
		}

		for (auto& pBase : G::Vars)
		{
			Remove(bool, iIndex)
			else Remove(int, iIndex)
			else Remove(float, iIndex)
			else Remove(IntRange_t, iIndex)
			else Remove(FloatRange_t, iIndex)
			else Remove(std::string, iIndex)
			else Remove(VA_LIST(std::vector<std::pair<std::string, Color_t>>), iIndex)
			else Remove(Color_t, iIndex)
			else Remove(Gradient_t, iIndex)
			else Remove(Vec3, iIndex)
			else Remove(DragBox_t, iIndex)
			else Remove(WindowBox_t, iIndex)
		}
	};
	fSearchBinds(iBind);
	std::sort(vErases.begin(), vErases.end(), [&](const int a, const int b) -> bool
	{
		return a > b;
	});
	for (auto iIndex : vErases)
		fRemoveBind(iIndex);
}

int CBinds::GetParent(int iBind)
{
	if (iBind > DEFAULT_BIND && iBind < m_vBinds.size())
		return m_vBinds[iBind].m_iParent;
	return DEFAULT_BIND;
}

bool CBinds::HasChildren(int iBind)
{
	auto it = std::ranges::find_if(m_vBinds, [&](const auto& tBind) { return iBind == tBind.m_iParent; });
	return it != m_vBinds.end();
}

bool CBinds::WillBeEnabled(int iBind)
{
	Bind_t tBind;
	while (GetBind(iBind, &tBind))
	{
		if (!tBind.m_bEnabled)
			return false;
		iBind = tBind.m_iParent;
	}
	return true;
}

template <class T>
static inline void SwapMain(BaseVar*& pBase, int iBind1, int iBind2)
{
	auto pVar = pBase->As<T>();

	bool bHas1 = pVar->contains(iBind1), bHas2 = pVar->contains(iBind2);
	if (bHas1 && bHas2)
	{
		auto& tVal1 = pVar->Map[iBind1];
		auto& tVal2 = pVar->Map[iBind2];
		auto tTemp = tVal1;
		tVal1 = tVal2;
		tVal2 = tTemp;
	}
	else if (bHas1)
	{
		pVar->Map[iBind2] = pVar->Map[iBind1];
		pVar->Map.erase(iBind1);
	}
	else if (bHas2)
	{
		pVar->Map[iBind1] = pVar->Map[iBind2];
		pVar->Map.erase(iBind2);
	}
}
#define Swap(t, i1, i2) if (IsType(t)) SwapMain<t>(pBase, i1, i2);

void CBinds::Move(int i1, int i2)
{
	auto& tBind1 = m_vBinds[i1];
	auto& tBind2 = m_vBinds[i2];
	auto tTemp = tBind1;
	tBind1 = tBind2;
	tBind2 = tTemp;

	std::vector<Bind_t*> vBinds1, vBinds2;
	for (auto& tBind : m_vBinds)
	{
		if (tBind.m_iParent == i1)
			vBinds1.push_back(&tBind);
		else if (tBind.m_iParent == i2)
			vBinds2.push_back(&tBind);
	}
	for (auto pBind : vBinds1) pBind->m_iParent = i2;
	for (auto pBind : vBinds2) pBind->m_iParent = i1;

	for (auto& pBase : G::Vars)
	{
		Swap(bool, i1, i2)
		else Swap(int, i1, i2)
		else Swap(float, i1, i2)
		else Swap(IntRange_t, i1, i2)
		else Swap(FloatRange_t, i1, i2)
		else Swap(std::string, i1, i2)
		else Swap(VA_LIST(std::vector<std::pair<std::string, Color_t>>), i1, i2)
		else Swap(Color_t, i1, i2)
		else Swap(Gradient_t, i1, i2)
		else Swap(Vec3, i1, i2)
		else Swap(DragBox_t, i1, i2)
		else Swap(WindowBox_t, i1, i2)
	}
}