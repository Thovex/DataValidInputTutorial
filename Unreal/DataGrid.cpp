// Copyright 2019 Jesse J. Van Vliet GRADUATION

#include "DataGrid.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

ADataGrid::ADataGrid( const FObjectInitializer& ObjectInitializer ) {

	Transform = ObjectInitializer.CreateDefaultSubobject<USceneComponent>( this, TEXT( "Transform" ) );
	RootComponent = Transform;

	PrimaryActorTick.bCanEverTick = true;
}

void ADataGrid::BeginPlay() {
	Super::BeginPlay();

}

void ADataGrid::Training() {
	SetPatternLocations();
	MapChildren();

}

bool ADataGrid::SelectedCheck() {

	if ( Errors.Num() > 0 ) return true;
	if ( IsSelectedInEditor() ) return true;

	TArray<AActor*> AttachedActors;
	GetAttachedActors( AttachedActors );

	for ( AActor* ChildActor : AttachedActors ) {
		if ( ChildActor->IsSelectedInEditor() ) return true;
	}

	return false;
}

void ADataGrid::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

	if ( !SelectedCheck()) return;

	Errors.Empty();
	Training();

	PatternStatus = Errors.Num() == 0 ? FColor::Green : FColor::Red;

	const UWorld* World = GetWorld();

	if ( World ) {
		for ( auto& ErrorPair : Errors ) {
			DrawDebugLine( World, GetActorLocation() + ( FVector::UpVector * GridElementSize ), ErrorPair.Key->GetActorLocation(), FColor::Red, false, -1.F, 0, 10.F );
			DrawDebugBox( World, ErrorPair.Key->GetActorLocation(), ErrorPair.Key->GetActorScale3D() * UUtilityLibrary::Divide_VectorByIntVector( FVector( GridElementSize ), NSize ), FColor::Red, false, -1.F, 0, 10.F );
		}

		DrawDebugBox( World, GetActorLocation(), FVector( NSize * GridElementSize / 2 ), PatternStatus, false, -1.F, 0, PatternStatus == FColor::Green ? 10.F : Errors.Num() * 50.F );
	}
}

void ADataGrid::SetPatternLocations() {
	PatternLocations.Empty( NSize.X * NSize.Y * NSize.Z );

	for ( int32 Z = 0; Z < NSize.Z; Z++ ) {
		for ( int32 Y = 0; Y < NSize.Y; Y++ ) {
			for ( int32 X = 0; X < NSize.X; X++ ) {
				PatternLocations.Add( FIntVector( X, Y, Z ), 0 );
			}
		}
	}
}

FIntVector ADataGrid::DefineCoord( FIntVector RelativeValueLocation ) const {
	FIntVector Coord;
	const int32 HalfGridElement = GridElementSize / 2;

	Coord.X = ( HalfGridElement + RelativeValueLocation.X ) / GridElementSize;
	Coord.Y = ( HalfGridElement + RelativeValueLocation.Y ) / GridElementSize;
	Coord.Z = ( HalfGridElement + RelativeValueLocation.Z ) / GridElementSize;

	return Coord;
}

void ADataGrid::AppendError( const AActor * Actor, FString Error ) {
	if ( Errors.Contains( Actor ) ) {
		FString CurrentErrors = Errors.FindRef( Actor );
		CurrentErrors += ", " + Error;

		Errors.Add( Actor, CurrentErrors );
	} else {
		Errors.Add( Actor, Error );
	}
}

void ADataGrid::RelativeValueLocationCheck( const AActor * Actor, const FIntVector RelativeValueLocation ) {
	if ( RelativeValueLocation.X % GridElementSize == 0 ) {
		AppendError( Actor, "Inconsistency at RelativeValueLocation.X" );
	}

	if ( RelativeValueLocation.Y % GridElementSize == 0 ) {
		AppendError( Actor, "Inconsistency at RelativeValueLocation.Y" );
	}

	if ( RelativeValueLocation.Z % GridElementSize == 0 ) {
		AppendError( Actor, "Inconsistency at RelativeValueLocation.Z" );
	}
}

void ADataGrid::RelativeValueRotationCheck( const AActor * Actor, const FIntVector RelativeValueRotation ) {

	if ( RelativeValueRotation.X != 0 ) {
		AppendError( Actor, "Inconsistency at RelativeValueRotation.X" );
	}

	if ( RelativeValueRotation.Y != 0 ) {
		AppendError( Actor, "Inconsistency at RelativeValueRotation.Y" );
	}

	if ( RelativeValueRotation.Z % 90 != 0 ) {
		AppendError( Actor, "Inconsistency at RelativeValueRotation.Z" );
	}
}

void ADataGrid::ValueScaleCheck( const AActor * Actor, const FVector ValueScale ) {
	const TArray<FVector> AllowedScales = TArray<FVector> {
		FVector( 1, 1, 1 ), FVector( -1, 1, 1 ), FVector( 1, -1, 1 )
	};

	if ( !AllowedScales.Contains( ValueScale ) ) {
		AppendError( Actor, "Inconsistent Scaling on Element" );
	}

}

void ADataGrid::MapChildren() {

	TArray<AActor*> ChildActors;
	GetAttachedActors( ChildActors );

	ModulesMap.Empty();

	for ( int32 i = 0; i < ChildActors.Num(); i++ ) {

		if ( ChildActors[i] ) {
			if ( ChildActors[i]->IsValidLowLevel() ) {
				if ( !ChildActors[i]->Tags.Contains( FName( TEXT( "DisplayOnly" ) ) ) ) {

					const FIntVector RelativeValueLocation = UUtilityLibrary::Conv_VectorToIntVector( ChildActors[i]->GetActorLocation() - GetActorLocation() );
					const FIntVector RelativeValueRotation = UUtilityLibrary::Conv_RotatorToIntVector( ChildActors[i]->GetActorRotation() - GetActorRotation() );
					const FVector ValueScale = ChildActors[i]->GetActorScale3D();

					const FIntVector Coord = DefineCoord( RelativeValueLocation );

					RelativeValueLocationCheck( ChildActors[i], RelativeValueLocation );
					RelativeValueRotationCheck( ChildActors[i], RelativeValueRotation );
					ValueScaleCheck( ChildActors[i], ValueScale );

					if ( PatternLocations.Contains( Coord ) ) {

						const int32 Count = PatternLocations.FindOrAdd( Coord );
						PatternLocations.Add( Coord, Count + 1 );

						if ( Count + 1 == 1 ) {
							ModulesMap.Add( Coord, Cast<AModule>( ChildActors[i] ) );
						} else {
							AppendError( ChildActors[i], "Multiple elements at one allowed location" );

						}
					} else {
						AppendError( ChildActors[i], "Invalid element location" );
					}
				}
			}
		}
	}
}