#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AzureSpatialAnchorManager.generated.h"

UENUM(BlueprintType)
enum class EAnchorMesh : uint8
{
	Cylinder,
	Sphere,
	Cone
};

USTRUCT(BlueprintType)
struct FAnchorDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString AnchorIdentifier;

	UPROPERTY(BlueprintReadWrite)
	EAnchorMesh AnchorMesh;

	FAnchorDetails(FString Identifier, EAnchorMesh Mesh)
	{
		AnchorIdentifier = Identifier;
		AnchorMesh = Mesh;
	}

	FAnchorDetails()
	{
		AnchorIdentifier = FString{};
		AnchorMesh = EAnchorMesh::Cylinder;
	}

	bool operator==(const FAnchorDetails& rhs) const
	{
		return (AnchorIdentifier == rhs.AnchorIdentifier);
	}

	bool operator!=(const FAnchorDetails& rhs) const
	{
		return (AnchorIdentifier != rhs.AnchorIdentifier);
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnchorAddedDelegate, const FAnchorDetails&, AnchorDetails);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAnchorRemovedDelegate, const FString&, AnchorIdentifier);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZURESPATIALANCHORSSAMPLE_API AAzureSpatialAnchorManager : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Azure Spatial Anchors Sample")
	static AAzureSpatialAnchorManager* GetInstance(AActor* Actor);

	AAzureSpatialAnchorManager();
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	void AddAnchor(const FAnchorDetails& Details);
	void RemoveAnchor(const FString& AnchorIdentifier);

	UFUNCTION(BlueprintCallable, Category = "Azure Spatial Anchors Sample")
	TArray<FString> GetAnchorIdentifiers();

	UFUNCTION(BlueprintCallable, Category = "Azure Spatial Anchors Sample")
	TMap<FString, FAnchorDetails> GetAnchorDetails();

	UPROPERTY(BlueprintAssignable, Category = "Azure Spatial Anchors Sample")
	FAnchorAddedDelegate AnchorAdded;

	UPROPERTY(BlueprintAssignable, Category = "Azure Spatial Anchors Sample")
	FAnchorRemovedDelegate AnchorRemoved;

protected:
	virtual void BeginDestroy() override;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_AnchorAdded(const FAnchorDetails& Details);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multicast_AnchorRemoved(const FString& Identifier);

	UFUNCTION()
	void OnReplicate_AnchorIdentifiers(TArray<FAnchorDetails> Details);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnReplicate_AnchorIdentifiers)
	TArray<FAnchorDetails> AnchorDetails;

	static TMap<UWorld*, AAzureSpatialAnchorManager*> InstanceMap;
};
