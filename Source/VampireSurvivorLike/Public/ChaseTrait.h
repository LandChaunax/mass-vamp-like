// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityElementTypes.h"
#include "MassEntityTraitBase.h"
#include "ChaseTrait.generated.h"

/**
 * 
 */

USTRUCT()
struct FChaseFragment : public FMassSharedFragment 
{
	GENERATED_BODY()
	
	UPROPERTY()
	FVector PlayerLocation = FVector::ZeroVector;
	
	FChaseFragment GetValidated() const
	{
		FChaseFragment Copy = *this;
		Copy.PlayerLocation = FVector::ZeroVector;
		return Copy;
	}
};

UCLASS()
class VAMPIRESURVIVORLIKE_API UChaseTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()
	
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
	
	UPROPERTY(EditAnywhere, Category = "Vamp")
	FChaseFragment InChaseFragment;
};

