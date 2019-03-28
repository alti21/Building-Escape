// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("grabber reporting"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get Player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(//OUT is a macro
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);//for now, OUT does nothing. 
	
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;//use this for reaching and grabbing chair?
	// Draw a red trace in the world to visualize grabber reach
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation, //line start
		LineTraceEnd,//endpoint will be 20 cm above player's head
		FColor(255, 0, 0),//color of line is red
		false,//don't want line to persist
		0.0f,//lifetime is not relevant since line is not persisting
		0.0f,
		10.0f
	);
	// Setup query parameters
	FCollisionQueryParams TraceParameters(
		FName(TEXT("")), 
		false, //we want to do a simple trace into the simple colliders, not a complex trace, true if you want to, i.e. collide with arms of chair
		GetOwner()// we want to ignore ourselves because the ray starts from the center of the sphere(defaultpawn, ourself) so we don't want the first 
		//hit to be ourself
		);
	// Line trace (ray-cast) out to reach distance, objectType because physicsBody is the objectType
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),//use :: to access enum members
		TraceParameters
	);
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit %s"), *(ActorHit->GetName()));
	}//when moving around, if you hit the table or chair, this will be written to console
		
	// See what we hit
}

