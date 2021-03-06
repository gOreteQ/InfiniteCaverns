// Daniel Alejandro Castro García <dandev237@gmail.com>

#pragma once

#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "RuntimeMeshActor.generated.h"

UCLASS()
class INFINITECAVERNS_API ARuntimeMeshActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	URuntimeMeshComponent* MeshComponent = nullptr;
	
	// Sets default values for this actor's properties
	ARuntimeMeshActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void CreateRuntimeMesh(const MeshData& Data);
	
};
