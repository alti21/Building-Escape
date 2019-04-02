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
	Super::BeginPlay();//goes up the heirarchy, goes to class UActorComponent
	FindPhysicsHandleComponent();
	//UE_LOG(LogTemp, Warning, TEXT("grabber reporting"));
	SetupInputComponent();

}

// Look for attached Physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	//GetOwner() get's the owner and FindComponentByClass looks down the owner (i.e., the default pawn/player components)
	//and looks for the component of type UphysicsHandleComponent, which is the PhysicsHandle
	if (PhysicsHandle == nullptr)
	{
		//Physics handle is not found
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *(GetOwner()->GetName()))
	}
}

// Look for attached input component, only appears at run time
void UGrabber::SetupInputComponent()
{
	
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	//GetOwner() get's the owner and FindComponentByClass looks down the owner (i.e., the default pawn/player components)
	//and looks for the component of type UInputComponent, which is the InputComponent, InputComponent only appears at runtime, when game is played
	if (InputComponent)
	{
		//InputComponent is found
		//UE_LOG(LogTemp, Warning, TEXT("Input component found"))
			// Bind the input axis, this pointer is a reference to the grabber component itself
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		//here we are binding any key that maps to "Grab" (under project settings/input/action mappings), when key is pressed, we look at "this", which is 
		//the Grabber then we look for a method called Grab, Grab function is called when we push the key(s)
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *(GetOwner()->GetName()))
	}
}




// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
//	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;//use this for reaching and grabbing chair?
	if (!PhysicsHandle)
	{
		return;
	}
	//if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{//move object that we are holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
		

	
}

void UGrabber::Grab()
{

	//Line trace and see if we reach any actors with physics body collision set
	auto HitResult = GetFirstPhysicsBodyInReach();
	//GetComponent() is a member of struct HitResult
	auto ComponentToGrab = HitResult.GetComponent();//GetComponent is of type UPrimitiveComponent, which is what we need in PhysicsHandle->GrabComponent(...)
	//left side: type is UPrimitiveComponent, same as right side

	auto ActorHit = HitResult.GetActor();

	//if we hit something attach a physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle)
		{
			return;
		}
		// attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}

}

void UGrabber::Release()
{
	if (!PhysicsHandle)
	{
		return;
	}
		PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get Player view point this tick
	
	// Draw a red trace in the world to visualize grabber reach
	//DrawDebugLine(
	//	GetWorld(),
	//	PlayerViewPointLocation, //line start
	//	LineTraceEnd,//endpoint will be 20 cm above player's head
	//	FColor(255, 0, 0),//color of line is red
	//	false,//don't want line to persist
	//	0.0f,//lifetime is not relevant since line is not persisting
	//	0.0f,
	//	10.0f
	//);
	// Setup query parameters
	FCollisionQueryParams TraceParameters(
		FName(TEXT("")),
		false, //we want to do a simple trace into the simple colliders, not a complex trace, true if you want to, i.e. collide with arms of chair
		GetOwner()// we want to ignore ourselves because the ray starts from the center of the sphere(defaultpawn, ourself) so we don't want the first 
		//hit to be ourself
	);
	// Line trace (ray-cast) out to reach distance, objectType because physicsBody is the objectType
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),//use :: to access enum members
		TraceParameters
	);



	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	// Get Player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(//OUT is a macro
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);//for now, OUT does nothing. 

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	// Get Player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(//OUT is a macro
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);//for now, OUT does nothing. 

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;//use this for reaching and grabbing chair?

}

