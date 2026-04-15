// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassProcessor.h"
#include "SubsystemProcessors.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRESURVIVORLIKE_API  URegistrationProcessor : public UMassProcessor
{
	GENERATED_BODY()
	
	URegistrationProcessor();

protected:

	virtual void ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	
	FMassEntityQuery EntityQuery;
};


UCLASS()
class VAMPIRESURVIVORLIKE_API  UCollisionProcessor : public UMassProcessor
{
	GENERATED_BODY()
	
	UCollisionProcessor();

protected:

	virtual void ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager) override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	
	FMassEntityQuery EntityQuery;
};
