// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "UObject/UObjectBase.h"
#include "SpatRoom.generated.h"

/**
 * 
 */
UCLASS()
class SPATTEMPLATE01_API ASpatRoom : public ATriggerBox
{
	GENERATED_BODY()

public:
	ASpatRoom();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int uniqueRoomID;

		
};
