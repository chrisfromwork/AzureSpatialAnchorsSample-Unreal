#include "AzureSpatialAnchorPawnComponent.h"
#include "AzureSpatialAnchorManager.h"

UAzureSpatialAnchorPawnComponent::UAzureSpatialAnchorPawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAzureSpatialAnchorPawnComponent::Server_AddAnchor_Implementation(const FString& AnchorIdentifier, EAnchorMesh AnchorMesh)
{
	auto Manager = AAzureSpatialAnchorManager::GetInstance(GetOwner());
	Manager->AddAnchor(FAnchorDetails{ AnchorIdentifier, AnchorMesh });
}

bool UAzureSpatialAnchorPawnComponent::Server_AddAnchor_Validate(const FString& AnchorIdentifier, EAnchorMesh AnchorMesh)
{
	return true;
}

void UAzureSpatialAnchorPawnComponent::Server_RemoveAnchor_Implementation(const FString& AnchorIdentifier)
{
	auto Manager = AAzureSpatialAnchorManager::GetInstance(GetOwner());
	Manager->RemoveAnchor(AnchorIdentifier);
}

bool UAzureSpatialAnchorPawnComponent::Server_RemoveAnchor_Validate(const FString& AnchorIdentifier)
{
	return true;
}


