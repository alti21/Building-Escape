// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionReport.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UPositionReport::UPositionReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPositionReport::BeginPlay()
{//pointers tells which memory address to visit to find the actual value!!!
	Super::BeginPlay();//do whatever happens up the inheritance tree
	FString ObjectName = GetOwner()->GetName();//get name of object in game world, i.e. chair or rock
	//use -> when GetOwner() is a pointer, use . when GetOwner is not a pointer
	//FString ObjectPos = GetOwner()->GetActorLocation().ToString();//get x,y,z position of each object and convert to string
	FString ObjectPos = GetOwner()->GetTransform().GetLocation().ToString();//alternative way for above comment
	//NOTE: since GetOwner() has a * before it, use -> after it, GetTransform() has no *, use . operator
	UE_LOG(LogTemp, Warning, TEXT("%s is at %s"), *ObjectName, *ObjectPos);//log, warning, or error
	
}


// Called every frame
void UPositionReport::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

