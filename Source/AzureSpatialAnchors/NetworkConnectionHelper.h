// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NetworkConnectionHelper.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZURESPATIALANCHORSSAMPLE_API UNetworkConnectionHelper : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNetworkConnectionHelper();

	UFUNCTION(BlueprintCallable, Category = "Azure Spatial Anchors Sample")
	void RunAsServer(FString Level);

	UFUNCTION(BlueprintCallable, Category = "Azure Spatial Anchors Sample")
	void RunAsClient(FString ServerIPAddress);

	UFUNCTION(BlueprintCallable, Category = "Azure Spatial Anchors Sample")
	bool IsInSetupMap();

	UPROPERTY(EditAnywhere, Category = "Azure Spatial Anchors Sample")
	FString SetupMapSubstring;
};
