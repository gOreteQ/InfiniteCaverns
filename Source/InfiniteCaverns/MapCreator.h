// Daniel Alejandro Castro García <dandev237@gmail.com>

#pragma once

#include "Engine/LevelScriptActor.h"
#include "MapCreator.generated.h"

/**
 * 
 */

struct MapColumn;

UCLASS()
class INFINITECAVERNS_API AMapCreator : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = CavernGenerator)
	void GenerateCavern(const int32 Width, const int32 Height, const int32 WallPercentage, const int32 SmoothingIterations);

private:
	int32 GetSorroundingWallsOfCell(const TArray<MapColumn> &Map, const int32 &CellX, const int32 &CellY);

};

struct MapColumn
{
	TArray<int> Cells;

	int &operator[](int i)
	{
		if (i > Cells.Num())
		{
			//Index out of bounds
			return Cells[0];
		}
		return Cells[i];
	}
};