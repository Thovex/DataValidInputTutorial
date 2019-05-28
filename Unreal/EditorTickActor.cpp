// Copyright 2019 Jesse J. Van Vliet GRADUATION

#include "EditorTickActor.h"

AEditorTickActor::AEditorTickActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEditorTickActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled( false );
	
}

void AEditorTickActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AEditorTickActor::ShouldTickIfViewportsOnly() const {
	return true;
}
