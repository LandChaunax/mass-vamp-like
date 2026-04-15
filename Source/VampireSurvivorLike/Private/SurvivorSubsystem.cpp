// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivorSubsystem.h"

void USurvivorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USurvivorSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void USurvivorSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void USurvivorSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<FMassEntityHandle> USurvivorSubsystem::Query(FVector WorldPos, float Radius) const
{
	TArray<FMassEntityHandle> Result;
	FGridCell Center = WorldToCell(WorldPos);
	int32 CellRadius = FMath::CeilToInt(Radius / CellSize);

	for (int32 X = Center.X - CellRadius; X <= Center.X + CellRadius; X++)
	{
		for (int32 Y = Center.Y - CellRadius; Y <= Center.Y + CellRadius; Y++)
		{
			const TSet<FMassEntityHandle>* Cell = Grid.Find({(int16)X, (int16)Y});
			if (Cell)
				Result.Append(Cell->Array());
		}
	}

	return Result;
}