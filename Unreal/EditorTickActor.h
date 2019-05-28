// Copyright 2019 Jesse J. Van Vliet GRADUATION

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EditorTickActor.generated.h"

UCLASS()
class TRAINING_API AEditorTickActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEditorTickActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

	virtual bool ShouldTickIfViewportsOnly() const override;
};
