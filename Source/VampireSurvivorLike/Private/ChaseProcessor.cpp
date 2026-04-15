
// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseProcessor.h"

#include "ChaseTrait.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "MassExecutionContext.h"
#include "MassMovementFragments.h"
#include "MassRepresentationTypes.h"

UChaseProcessor::UChaseProcessor()
	: EntityQuery(*this)
{
	ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::Client | EProcessorExecutionFlags::Standalone);
	ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Tasks;
	ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::Representation);
}

void UChaseProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FMassDesiredMovementFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSharedRequirement<FChaseFragment>(EMassFragmentAccess::ReadWrite);
}

void UChaseProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	float DeltaTime = Context.GetDeltaTimeSeconds(); 
	EntityQuery.ParallelForEachEntityChunk(Context, [this, &EntityManager, DeltaTime](FMassExecutionContext& Context)
	{
		TArrayView<FTransformFragment> TransformFragmentView = Context.GetMutableFragmentView<FTransformFragment>();
		TArrayView<FMassDesiredMovementFragment> MassDesiredMovementFragmentsView = Context.GetMutableFragmentView<FMassDesiredMovementFragment>();
		FChaseFragment& ChaseFragment = Context.GetMutableSharedFragment<FChaseFragment>();
		for (FMassExecutionContext::FEntityIterator EntityIt = Context.CreateEntityIterator(); EntityIt; ++EntityIt)
		{
			FTransformFragment& TransformFragment = TransformFragmentView[EntityIt];
			FMassDesiredMovementFragment& MassDesiredMovementFragment = MassDesiredMovementFragmentsView[EntityIt];
			FVector Direction = (ChaseFragment.PlayerLocation - TransformFragment.GetTransform().GetLocation()).GetSafeNormal();
			Direction.Z = 0.f;
			MassDesiredMovementFragment.DesiredVelocity = Direction*100.f;
		}
	});
}
