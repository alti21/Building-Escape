// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Classes/Components/PrimitiveComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"
//use this when making an event blueprint assignable, such as opening a door, event is a subset of a dynamic multicast delegate
//we are declaring something that is serializable (dynamic) and it's also multicast (one to many?), delegate means something else will handle BP
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);//makes a new class called FOnOpenRequest
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpen;//this will allow us to handle the door open event

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnClose;

private:
	//UPROPERTY(EditAnywhere)     //this is a macro, it will do a cut and paste before the program compiles, for this one, line after it will use it
	//float OpenAngle = -10.0f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	//UPROPERTY(EditAnywhere)
	//float DoorCloseDelay = 0.05f;
	UPROPERTY(EditAnywhere)
	float TriggerMass = 30.0f;

	//float LastDoorOpenTime;
	//UPROPERTY(EditAnywhere)
	//AActor* ActorThatOpens; //pawn inherites from actor
	AActor* Owner = nullptr; //the owning door

	// Returns total mass in kg
	float GetTotalMassOfActorsOnPlate();
};
