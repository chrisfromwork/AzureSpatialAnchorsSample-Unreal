#include "AzureSpatialAnchorManager.h"
#include "Net/UnrealNetwork.h"
#include "AzureSpatialAnchorsSample.h"
#include "Kismet/GameplayStatics.h"

TMap<UWorld*, AAzureSpatialAnchorManager*> AAzureSpatialAnchorManager::InstanceMap;

AAzureSpatialAnchorManager* AAzureSpatialAnchorManager::GetInstance(AActor* Actor)
{
	if (Actor == nullptr)
	{
		return nullptr;
	}

	auto World = Actor->GetWorld();
	if(!InstanceMap.Contains(World))
	{
		TArray<AActor*> AnchorManagers;
		UGameplayStatics::GetAllActorsOfClass(World, AAzureSpatialAnchorManager::StaticClass(), AnchorManagers);
		if (AnchorManagers.Num() != 1)
		{
			UE_LOG(LogAzureSpatialAnchorsSample, Error, TEXT("Expected 1 AzureSpatialAnchorManager in scene but found: %d"), AnchorManagers.Num());
		}

		if (AnchorManagers.Num() > 0)
		{
			InstanceMap.Add(World, dynamic_cast<AAzureSpatialAnchorManager*>(AnchorManagers[0]));
		}
	}

	if (!InstanceMap.Contains(World))
	{
		return nullptr;
	}

	return InstanceMap[World];
}

AAzureSpatialAnchorManager::AAzureSpatialAnchorManager() : Super()
{
	PrimaryActorTick.bCanEverTick = false;
	bAlwaysRelevant = true;
	SetReplicates(true);
}

void AAzureSpatialAnchorManager::BeginDestroy()
{
	Super::BeginDestroy();
	auto World = GetWorld();
	if (InstanceMap.Contains(World) &&
		InstanceMap[World] == this)
	{
		InstanceMap.Remove(World);
	}
}

void AAzureSpatialAnchorManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAzureSpatialAnchorManager, AnchorDetails);
}

void AAzureSpatialAnchorManager::AddAnchor(const FAnchorDetails& Details)
{
	if (!AnchorDetails.Contains(Details))
	{
		UE_LOG(LogAzureSpatialAnchorsSample, Log, TEXT("Adding new anchor: %s"), *Details.AnchorIdentifier);
		AnchorDetails.Add(Details);
		Multicast_AnchorAdded(Details);
	}
}

void AAzureSpatialAnchorManager::RemoveAnchor(const FString& AnchorIdentifier)
{
	TArray<FAnchorDetails> AnchorDetailsToRemove;
	for (auto Details : AnchorDetails)
	{
		if (Details.AnchorIdentifier == AnchorIdentifier)
		{
			AnchorDetailsToRemove.Add(Details);
		}
	}

	for (auto DetailToRemove : AnchorDetailsToRemove)
	{
		AnchorDetails.Remove(DetailToRemove);
		UE_LOG(LogAzureSpatialAnchorsSample, Log, TEXT("Removed anchor: %s"), *DetailToRemove.AnchorIdentifier);
		Multicast_AnchorRemoved(DetailToRemove.AnchorIdentifier);
	}
}

TArray<FString> AAzureSpatialAnchorManager::GetAnchorIdentifiers()
{
	TArray<FString> Identifiers;
	for (auto Details : AnchorDetails)
	{
		Identifiers.Add(Details.AnchorIdentifier);
	}

	return Identifiers;
}

TMap<FString, FAnchorDetails> AAzureSpatialAnchorManager::GetAnchorDetails()
{
	TMap<FString, FAnchorDetails> DetailMap;
	for (auto Details : AnchorDetails)
	{
		DetailMap.Add(Details.AnchorIdentifier, Details);
	}

	return DetailMap;
}

void AAzureSpatialAnchorManager::Multicast_AnchorAdded_Implementation(const FAnchorDetails& Details)
{
	AnchorAdded.Broadcast(Details);
}

bool AAzureSpatialAnchorManager::Multicast_AnchorAdded_Validate(const FAnchorDetails& Details)
{
	return true;
}

void AAzureSpatialAnchorManager::Multicast_AnchorRemoved_Implementation(const FString& Identifier)
{
	AnchorRemoved.Broadcast(Identifier);
}

bool AAzureSpatialAnchorManager::Multicast_AnchorRemoved_Validate(const FString& Identifier)
{
	return true;
}

void AAzureSpatialAnchorManager::OnReplicate_AnchorIdentifiers(TArray<FAnchorDetails> Details)
{
	UE_LOG(LogAzureSpatialAnchorsSample, Log, TEXT("Anchor Identifiers replication occurred, number of anchors: %d"), Details.Num());
}
