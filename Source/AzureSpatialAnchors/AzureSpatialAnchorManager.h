#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AzureSpatialAnchorManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnchorsUpdatedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZURESPATIALANCHORSSAMPLE_API AAzureSpatialAnchorManager : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Azure Spatial Anchors Sample")
	static AAzureSpatialAnchorManager* GetInstance() { return Instance; }

	AAzureSpatialAnchorManager();
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Azure Spatial Anchors Sample")
	void Server_AddAnchor(const FString& AnchorIdentifier);

	UFUNCTION(BlueprintCallable, Category = "Azure Spatial Anchors Sample")
	TArray<FString> GetAnchorIdentifiers();

	UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Azure Spatial Anchors Sample")
	FAnchorsUpdatedDelegate AnchorsUpdated;

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_AnchorsUpdated();

	UPROPERTY(EditAnywhere, Replicated)
	TArray<FString> AnchorIdentifiers;

	static AAzureSpatialAnchorManager* Instance;
};
