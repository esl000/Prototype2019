// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"
#include "PrototypeProjectCharacter.h"

void UInGameUI::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	AttackPersent = Player->CurrentAttackCoolTime / Player->AttackCoolTime;
	ChargePersent = Player->CurrentChargeCoolTime / Player->ChargeCoolTime;
	DashPersent = Player->CurrentDashCoolTime / Player->DashCoolTime;
	SkillPersent = Player->CurrentSkillCoolTime / Player->SkillCoolTime;
	HPPersent = Player->Stat.Hp / Player->Stat.MaxHp;
	StackPersent = Player->Stat.Stack / (float)Player->Stat.MaxStack;
}
