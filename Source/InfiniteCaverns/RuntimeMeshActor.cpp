// Daniel Alejandro Castro García <dandev237@gmail.com>

#include "InfiniteCaverns.h"
#include "RuntimeMeshActor.h"


// Sets default values
ARuntimeMeshActor::ARuntimeMeshActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("Runtime Mesh"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void ARuntimeMeshActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARuntimeMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Creates the Runtime Mesh with the calculated vertices and triangles received as parameter
void ARuntimeMeshActor::CreateRuntimeMesh(const MeshData& Data)
{
	TArray<FRuntimeMeshVertexSimple> Vertices;
	TArray<int32> Triangles;

	Vertices.Reserve(Data.Vertices.Num());
	Triangles.Reserve(Data.Triangles.Num());

	for (FVector Vertex : Data.Vertices)
	{
		FRuntimeMeshVertexSimple VertexToAdd(FRuntimeMeshVertexSimple(Vertex, FVector(0, 0, 1)));
		Vertices.Add(VertexToAdd);
	}

	for (int32 TriangleVertex : Data.Triangles)
	{
		Triangles.Add(TriangleVertex);
	}

	// Create the mesh section and enables collision
	MeshComponent->CreateMeshSection(0, Vertices, Triangles, true);
}