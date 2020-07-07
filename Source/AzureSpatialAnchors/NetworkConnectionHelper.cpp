#include "NetworkConnectionHelper.h"
#include "Kismet/GameplayStatics.h"

UNetworkConnectionHelper::UNetworkConnectionHelper()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNetworkConnectionHelper::RunAsServer(FString Level)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*Level), true, "listen");
}

void UNetworkConnectionHelper::RunAsClient(FString ServerIPAddress)
{
	GetWorld()->GetFirstPlayerController()->ClientTravel(ServerIPAddress, TRAVEL_MAX, true, FGuid());
}

bool UNetworkConnectionHelper::IsInSetupMap()
{
	return GetWorld()->GetOutermost()->GetName().Contains(SetupMapSubstring, ESearchCase::CaseSensitive, ESearchDir::FromEnd);
}