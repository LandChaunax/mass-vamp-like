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

USTRUCT()
struct FLocationRegistrationFragment : public FMassFragment 
{
	GENERATED_BODY()
	
	UPROPERTY()
	FVector PreviousLocation = FVector::ZeroVector;
	
	/* How long in seconds until the average registration */
	UPROPERTY(EditAnywhere, Category = "Vamp")
	float RegistrationInterval = 1.f;
	
	/* Random value added on top of Registration interval each time to not have entities register same tick */
	UPROPERTY(EditAnywhere, Category = "Vamp")
	float RegistrationDeviation = 0.1f;
	
	UPROPERTY()
	float CurrentRegistrationTimer = 0.f;
	
	FLocationRegistrationFragment GetValidated() const
	{
		FLocationRegistrationFragment Copy = *this;
		Copy.PreviousLocation = FVector::ZeroVector;
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
	
	UPROPERTY(EditAnywhere, Category = "Vamp")
	FLocationRegistrationFragment InLocationRegistration;
};

