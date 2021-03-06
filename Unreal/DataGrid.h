// Copyright 2019 Jesse J. Van Vliet GRADUATION

#pragma once

#include "CoreMinimal.h"
#include "EditorTickActor.h"
#include "DataGrid.generated.h"

class AModuleAssignee;

UCLASS()
class TRAINING_API ADataGrid : public AEditorTickActor {
	GENERATED_BODY()

public:
	ADataGrid( const FObjectInitializer& ObjectInitializer );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Transform" )
		USceneComponent* Transform;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Grid Settings" )
		FIntVector NSize = FIntVector( 2, 2, 2 );

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Grid Settings" )
		int32 GridElementSize = 1000;

	UPROPERTY( EditAnywhere, BlueprintReadOnly, Category = "Grid Settings" )
		TMap<FIntVector, int32> PatternLocations;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Grid Debug" )
		FColor PatternStatus;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Grid Debug" )
		TMap<const AActor*, FString> Errors;

	void Training( );

protected:
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaTime ) override;

	void SetPatternLocations();

private:
	FIntVector DefineCoord( FIntVector RelativeValueLocation ) const;

	void AppendError( const AActor* Actor, FString Error );

	void RelativeValueLocationCheck( const AActor* Actor, const FIntVector RelativeValueLocation );
	void RelativeValueRotationCheck( const AActor* Actor, const FIntVector RelativeValueRotation );
	void ValueScaleCheck( const AActor* Actor, const FVector ValueScale );

	void MapChildren();
	void UpdateMatrix();

	bool SelectedCheck();

};
