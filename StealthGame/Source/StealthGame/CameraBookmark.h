// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityBlueprint.h"
#include "CameraBookmark.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API UCameraBookmark : public UEditorUtilityBlueprint
{
	GENERATED_BODY()

	TArray<AActor*> TestArray;
	
};
