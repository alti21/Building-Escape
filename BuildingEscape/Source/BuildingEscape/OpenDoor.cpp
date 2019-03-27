// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
//#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

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

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	//Pawn inherits from AActor, which means pawn "is a" actor
	//Pawn is in derived class, AActor is base class?
}

void UOpenDoor::OpenDoor()
{

	//AActor* Owner = GetOwner();//Find the owning actor

							   //Make a rotator
	//FRotator NewRotation = FRotator(0.0f, -60.0f, 0.0f);//FRotator is a struct
														//Set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{

	//AActor* Owner = GetOwner();//Find the owning actor

							   //Make a rotator
	//FRotator NewRotation = FRotator(0.0f, -60.0f, 0.0f);//FRotator is a struct
														//Set the door rotation
	Owner->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll trigger volume
	if(PressurePlate->IsOverlappingActor(ActorThatOpens))//pressureplate's type is ATriggerVolume, which is a pointer, IsOverlappingActor is a bool var
	// if the ActorThatOpens is in the volume
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();//store time when door is opened
	}
	//time in seconds since world was brought up for play
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{//if difference between current time and the exact time the door was opened is more than door close delay
		CloseDoor();//close door after some time has passed
	}
	//check if it's time to close the door

}

