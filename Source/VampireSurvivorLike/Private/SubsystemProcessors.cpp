// Fill out your copyright notice in the Description page of Project Settings.


#include "SubsystemProcessors.h"

#include "SurvivorSubsystem.h"
#include "MassCommonFragments.h"
#include "MassCommonTypes.h"
#include "ChaseTrait.h"
#include "MassExecutionContext.h"
#include "MassRepresentationTypes.h"


URegistrationProcessor::URegistrationProcessor()
	: EntityQuery(*this)
{
	ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::Client | EProcessorExecutionFlags::Standalone);
	//ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Tasks;
	ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::Representation);
}

void URegistrationProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FLocationRegistrationFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSubsystemRequirement<USurvivorSubsystem>(EMassFragmentAccess::ReadWrite);
}

void URegistrationProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	float DeltaTime = Context.GetDeltaTimeSeconds(); 
	EntityQuery.ForEachEntityChunk(Context, [this, &EntityManager, DeltaTime](FMassExecutionContext& Context)
	{
		USurvivorSubsystem& SmbSubsystem = Context.GetMutableSubsystemChecked<USurvivorSubsystem>();
		
		//ConstArrayView is used for read only, which allows Mass to decide when to run what more efficiently.
		const TConstArrayView<FTransformFragment> TransformFragmentView = Context.GetFragmentView<FTransformFragment>();
		TArrayView<FLocationRegistrationFragment> LocationRegistrationView = Context.GetMutableFragmentView<FLocationRegistrationFragment>();
		
		for (FMassExecutionContext::FEntityIterator EntityIt = Context.CreateEntityIterator(); EntityIt; ++EntityIt)
		{
			FLocationRegistrationFragment& PosRegFragment = LocationRegistrationView[EntityIt];
			
			//Time passed needs to be over current timer.
			PosRegFragment.CurrentRegistrationTimer += DeltaTime;
			if (PosRegFragment.RegistrationInterval > PosRegFragment.CurrentRegistrationTimer) continue;
			PosRegFragment.CurrentRegistrationTimer = FMath::FRandRange
				(-PosRegFragment.RegistrationDeviation,
				PosRegFragment.RegistrationDeviation);
			
			
			FTransformFragment TransformFragment = TransformFragmentView[EntityIt];
			FVector Location = TransformFragment.GetTransform().GetLocation();
			
			FVector OldLocation = PosRegFragment.PreviousLocation;
			SmbSubsystem.Register(Context.GetEntity(EntityIt),Location,OldLocation);
			PosRegFragment.PreviousLocation = Location;
		}
	});
}

UCollisionProcessor::UCollisionProcessor()
: EntityQuery(*this)
{
		ExecutionFlags = static_cast<int32>(EProcessorExecutionFlags::Client | EProcessorExecutionFlags::Standalone);
		//ExecutionOrder.ExecuteInGroup = UE::Mass::ProcessorGroupNames::Tasks;
		ExecutionOrder.ExecuteAfter.Add(UE::Mass::ProcessorGroupNames::Representation);
}

void UCollisionProcessor::ConfigureQueries(const TSharedRef<FMassEntityManager>& EntityManager)
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddSubsystemRequirement<USurvivorSubsystem>(EMassFragmentAccess::ReadOnly);
	EntityQuery.AddRequirement<FLocationRegistrationFragment>(EMassFragmentAccess::ReadOnly);
}

void UCollisionProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	float DeltaTime = Context.GetDeltaTimeSeconds(); 
	EntityQuery.ForEachEntityChunk(Context, [this, &EntityManager, DeltaTime](FMassExecutionContext& Context)
	{
		const USurvivorSubsystem& SmbSubsystem = Context.GetSubsystemChecked<USurvivorSubsystem>();
		
		//ConstArrayView is used for read only, which allows Mass to decide when to run what more efficiently.
		TArrayView<FTransformFragment> TransformFragmentView = Context.GetMutableFragmentView<FTransformFragment>();
		const TConstArrayView<FLocationRegistrationFragment> LocationRegistrationView = Context.GetFragmentView<FLocationRegistrationFragment>();
		
		for (FMassExecutionContext::FEntityIterator EntityIt = Context.CreateEntityIterator(); EntityIt; ++EntityIt)
		{
			FTransformFragment TransformFragment = TransformFragmentView[EntityIt];
			FVector Location = TransformFragment.GetTransform().GetLocation();
			TArray<FMassEntityHandle> NearEntities = SmbSubsystem.Query(Location,510);
			
			//UE_LOG(LogTemp, Display, TEXT("NearEntities: %i"), NearEntities.Num())
			
			for (FMassEntityHandle OtherEntity : NearEntities)
			{
				FTransformFragment* OtherTransformFragment = EntityManager.GetFragmentDataPtr<FTransformFragment>(OtherEntity);
				if (!OtherTransformFragment)
				{
					continue;
				}
				
				FLocationRegistrationFragment LocationRegistrationFragment = LocationRegistrationView[EntityIt];
				
				FVector OtherLocation = OtherTransformFragment->GetTransform().GetLocation();
				FVector RelativeLocation = (OtherLocation-Location);
				float Distance = RelativeLocation.Size();
				if (Distance >= LocationRegistrationFragment.PushRadius)
				{
					continue;
				}
				RelativeLocation.Z = 0.f;
				FVector PushDistance = RelativeLocation*(LocationRegistrationFragment.PushStrength/FMath::Max(Distance,0.1f));
				PushDistance *= DeltaTime;
				
				OtherTransformFragment->GetMutableTransform().SetLocation(OtherLocation+PushDistance);
			}
		}
	});
}


