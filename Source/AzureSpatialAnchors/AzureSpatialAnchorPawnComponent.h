// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AzureSpatialAnchorPawnComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZURESPATIALANCHORSSAMPLE_API UAzureSpatialAnchorPawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAzureSpatialAnchorPawnComponent();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Azure Spatial Anchors Sample")
	void Server_AddAnchor(const FString& AnchorIdentifier, EAnchorMesh AnchorMesh);

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Azure Spatial Anchors Sample")
	void Server_RemoveAnchor(const FString& AnchorIdentifier);
};
