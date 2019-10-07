// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "PrototypeProjectCharacter.h"

void UInGameUI::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//AttackPersent = (Player->AttackCoolTime - Player->CurrentAttackCoolTime) / Player->AttackCoolTime;
	//ChargePersent = (Player->ChargeCoolTime - Player->CurrentChargeCoolTime) / Player->ChargeCoolTime;
	//DashPersent = (Player->DashCoolTime - Player->CurrentDashCoolTime) / Player->DashCoolTime;
	//SkillPersent = (Player->SkillCoolTime - Player->CurrentSkillCoolTime) / Player->SkillCoolTime;
	//HPPersent = Player->Stat.Hp / Player->Stat.MaxHp;
	//StackPersent = Player->Stat.Stack / (float)Player->Stat.MaxStack;
}
