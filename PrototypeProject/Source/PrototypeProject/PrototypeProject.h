// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPrototypeProject, Log, All);


inline float GetPositiveAngle(float angle)
{
	return angle < 0.f ? 360.f - angle : angle;
}