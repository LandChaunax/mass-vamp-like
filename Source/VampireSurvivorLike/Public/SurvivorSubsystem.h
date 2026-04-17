// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassSubsystemBase.h"
#include "SurvivorSubsystem.generated.h"


USTRUCT()
struct FGridCell
{
	GENERATED_BODY()
	int16 X, Y;

	//How to compare two FGridCells
	bool operator==(const FGridCell& Other) const { return X == Other.X && Y == Other.Y; }
};

//Combined Hash for HashMap (TMap), single unique identifier for each grid location.
FORCEINLINE uint32 GetTypeHash(const FGridCell& Cell)
{
	//X location gets put on top of Y location.
	return ((uint32)(uint16)Cell.X << 16) | (uint16)Cell.Y;
}

template<>
struct TMassExternalSubsystemTraits<USurvivorSubsystem>
{
	static constexpr bool GameThreadOnly = false;
	static constexpr bool ThreadSafeWrite = false;
};

/**
 * 
 */
UCLASS()
class USurvivorSubsystem : public UMassTickableSubsystemBase
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void PostInitialize() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(USurvivorSubsystem, STATGROUP_Tickables); };

public:
	//Change CellSize for larger worlds than 6.5km, performance may differ.
	const float CellSize = 200.f;

	FGridCell WorldToCell(FVector WorldPos) const
	{
		return { (int16)FMath::FloorToInt(WorldPos.X / CellSize),
				 (int16)FMath::FloorToInt(WorldPos.Y / CellSize) };
	}

	void Register(FMassEntityHandle Handle, FVector NewLocation, FVector OldLocation)
	{
		Grid.FindOrAdd(WorldToCell(OldLocation)).Remove(Handle);
		Grid.FindOrAdd(WorldToCell(NewLocation)).Add(Handle);
	}

	void Unregister(FMassEntityHandle Handle, FVector WorldPos)
	{
		Grid.FindOrAdd(WorldToCell(WorldPos)).Remove(Handle);
	}
	
	TArray<FMassEntityHandle> Query(FVector WorldPos, float Radius) const;
	
private:
	TMap<FGridCell, TSet<FMassEntityHandle>> Grid;
};
