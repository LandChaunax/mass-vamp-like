// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseTrait.h"

#include "MassEntityTemplateRegistry.h"


void UChaseTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	FMassEntityManager& MassEntityManager = UE::Mass::Utils::GetEntityManagerChecked(World);
	
	FChaseFragment ChaseFragment = InChaseFragment.GetValidated();
	const FSharedStruct& SharedStruct = MassEntityManager.GetOrCreateSharedFragment(ChaseFragment);
	BuildContext.AddSharedFragment(SharedStruct);
}
