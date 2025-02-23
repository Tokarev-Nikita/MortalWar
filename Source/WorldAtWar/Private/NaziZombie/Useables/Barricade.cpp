// Fill out your copyright notice in the Description page of Project Settings.


#include "NaziZombie/Useables/Barricade.h"
#include "NaziZombie/Game/NaziZombieGameMode.h"
#include "Player/NaziZombieCharacter.h"
#include "Player/NaziZombiePlayerState.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

ABarricade::ABarricade()
{
	CollisionComp = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	SetRootComponent(CollisionComp);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	MeshComp->SetupAttachment(CollisionComp);
	
	//CollisionMesh->SetupAttachment(MeshComp);

	ObjectName = FText::FromString("Door");

	/*bIsUsed = false;

	Cost = 500;*/
	AccessZone = 0;

	//FString TestPassword = FMD5::HashAnsiString(*FString("HGHHdsfhjJH123"));
	
}


void ABarricade::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
		
	//UIMessage += ObjectName + "[Cost: " + FString::FromInt(Cost) + "]";

	/*const FString ResultMessage = UIMessage.ToString() + ObjectName.ToString() + "[Cost: " + FString::FromInt(Cost) + "]";

	UIMessage = FText::FromString(ResultMessage);*/
}




void ABarricade::OnRep_ObjectUsed()
{
	//SetActorEnableCollision(false);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!OpenAnimation) return;
	MeshComp->PlayAnimation(OpenAnimation, false);
}


void ABarricade::Use(ANaziZombieCharacter* Player)
{
	if (HasAuthority() && Player)
	{
		if (ANaziZombiePlayerState* PState = Player->GetPlayerState<ANaziZombiePlayerState>())
		{
			if (!PState->DecrementPoints(Cost)) return;

			bIsUsed = true;
			OnRep_ObjectUsed();

			OnObjectUsed.Broadcast();

			if (!GetWorld()) return;
			if (ANaziZombieGameMode* GM = GetWorld()->GetAuthGameMode<ANaziZombieGameMode>())
			{
				GM->NewZoneActive(AccessZone);
			}
		}
	}
	
}