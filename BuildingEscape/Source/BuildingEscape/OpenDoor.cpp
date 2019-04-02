// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
//#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	//OpenDoor();
	Owner = GetOwner();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *(GetOwner()->GetName()))
	}
	//ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	//Pawn inherits from AActor, which means pawn "is a" actor
	//Pawn is in derived class, AActor is base class?
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll trigger volume
	//if(PressurePlate->IsOverlappingActor(ActorThatOpens))//pressureplate's type is ATriggerVolume, which is a pointer, IsOverlappingActor is a bool var
	// if the ActorThatOpens is in the volume
	if(GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();//open door? using door event blueprint!
	//	LastDoorOpenTime = GetWorld()->GetTimeSeconds();//store time when door is opened
	}
	//time in seconds since world was brought up for play
	else
	{//if difference between current time and the exact time the door was opened is more than door close delay
		//CloseDoor();//close door after some time has passed
		OnClose.Broadcast();
	}
	//check if it's time to close the door

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;

	//Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);//it is an OUT parameter because it's a reference?
	//Iterate through them adding up their masses together
	for (const auto *Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}
