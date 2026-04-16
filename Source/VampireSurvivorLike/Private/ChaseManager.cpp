#include "ChaseManager.h"

#include "ChaseTrait.h"
#include "MassEntityManager.h"
#include "MassEntityUtils.h"

// Sets default values
AChaseManager::AChaseManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AChaseManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AChaseManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FMassEntityManager* EntityManger = UE::Mass::Utils::GetEntityManager(GetWorld());
	if (!EntityManger) return;
	if (!EntityManger->IsEntityValid(FMassEntityHandle(2,1))) return;
	FChaseFragment* ChaseFragment = EntityManger->GetSharedFragmentDataPtr<FChaseFragment>(FMassEntityHandle(1,1));
	if (ChaseFragment)
	{
		if (Player)
		{
			ChaseFragment->PlayerLocation = Player->GetActorLocation();
		}
	}
}
