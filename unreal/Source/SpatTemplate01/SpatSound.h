// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "UObject/UObjectBase.h"
#include "SpatSound.generated.h"

/**
 * 
 */
UCLASS()
class SPATTEMPLATE01_API ASpatSound : public ATriggerSphere
{
	GENERATED_BODY()
	
public:
	ASpatSound();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int uniqueID;

};


