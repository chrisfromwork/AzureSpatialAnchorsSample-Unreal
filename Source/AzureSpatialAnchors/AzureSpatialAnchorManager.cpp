#include "AzureSpatialAnchorManager.h"
#include "Net/UnrealNetwork.h"
#include "AzureSpatialAnchorsSample.h"

AAzureSpatialAnchorManager* AAzureSpatialAnchorManager::Instance = nullptr;

AAzureSpatialAnchorManager::AAzureSpatialAnchorManager() : Super()
{
	PrimaryActorTick.bCanEverTick = false;
	bAlwaysRelevant = true;
	SetReplicates(true);
}

void AAzureSpatialAnchorManager::BeginPlay()
{
	Super::BeginPlay();
	if (Instance != nullptr)
	{
		UE_LOG(LogAzureSpatialAnchorsSample, Fatal, TEXT("Multiple AAzureSpatialAnchorManagers were discovered in the application when only one is supported"));
	}

	Instance = this;
}

void AAzureSpatialAnchorManager::BeginDestroy()
{
	Super::BeginDestroy();
	if (Instance != this)
	{
		UE_LOG(LogAzureSpatialAnchorsSample, Warning, TEXT("AAzureSpatialAnchorsManager was being destryoed but was not found to be singleton instance"));
		return;
	}

	Instance = nullptr;
}

void AAzureSpatialAnchorManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAzureSpatialAnchorManager, AnchorIdentifiers);
}

void AAzureSpatialAnchorManager::Server_AddAnchor_Implementation(const FString& AnchorIdentifier)
{
	if (!AnchorIdentifiers.Contains(AnchorIdentifier))
	{
		AnchorIdentifiers.Add(AnchorIdentifier);
		Multicast_AnchorsUpdated();
	}
}

bool AAzureSpatialAnchorManager::Server_AddAnchor_Validate(const FString& AnchorIdentifier)
{
	return true;
}

TArray<FString> AAzureSpatialAnchorManager::GetAnchorIdentifiers()
{
	return AnchorIdentifiers;
}

void AAzureSpatialAnchorManager::Multicast_AnchorsUpdated_Implementation()
{
	AnchorsUpdated.Broadcast();
}

bool AAzureSpatialAnchorManager::Multicast_AnchorsUpdated_Validate()
{
	return true;
}