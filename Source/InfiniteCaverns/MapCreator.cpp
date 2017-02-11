// Daniel Alejandro Castro García <dandev237@gmail.com>

#include "InfiniteCaverns.h"
#include "MapCreator.h"

void AMapCreator::GenerateCavern(const int32 Width, const int32 Height, const int32 WallPercentage, const int32 SmoothingIterations)
{
	TArray<MapColumn> Map;
	
	//Randomly fill map with 0's and 1's
	//0 -> Floor (Passable terrain)
	//1 -> Wall (Impassable terrain)
	for (int i = 0; i < Width; ++i)
	{
		MapColumn column;
		for (int j = 0; j < Height; ++j)
		{
			if (i == 0 || i == Width - 1 || j == 0 || j == Height - 1)
			{
				column.Cells.Add(1);					//The borders of the map will always be walls
			}
			else
			{
				int RandomNumber = rand() % 100 + 1;	//Generate random number between 0 and 100
				column.Cells.Add((RandomNumber < WallPercentage) ? 1 : 0);
			}
		}
		Map.Add(column);
	}

	int32 SmoothingCounter = 0;
	while (SmoothingCounter < SmoothingIterations)
	{
		//Cellular automaton smoothing iterations
		for (int i = 0; i < Width; ++i)
		{
			for (int j = 0; j < Height; ++j)
			{
				int SorroundingWallsOfCell = GetSorroundingWallsOfCell(Map, i, j);

				if (SorroundingWallsOfCell > 4)
				{
					Map[i].Cells[j] = 1;	//Transform cell to wall
				}
				else if(SorroundingWallsOfCell < 4)
				{
					Map[i].Cells[j] = 0;	//Transform cell to floor
				}
			}
		}

		++SmoothingCounter;
	}

	for (int i = 0; i < Width; ++i)
	{
		for (int j = 0; j < Height; ++j)
		{
			DrawDebugBox(GetWorld(), FVector(100 * i, 100 * j, 0), FVector(45, 45, 45), Map[i].Cells[j] == 1 ? FColor::Black : FColor::White, true);
		}
	}
}

int32 AMapCreator::GetSorroundingWallsOfCell(const TArray<MapColumn> &Map, const int32 &CellX, const int32 &CellY)
{
	int32 MapPositionX = CellX - 1;
	int32 MapPositionY = CellY - 1;

	int32 SorroundingWalls = 0;

	//Moor neighbourhood -> 8-connected cells (Chebyshev distance = 1)
	while (MapPositionX <= CellX + 1)
	{
		//Check row
		if (MapPositionX >= 0 && MapPositionX < Map.Num() && MapPositionY >= 0 && MapPositionY < Map[MapPositionX].Cells.Num()) //MapPosition is inside of the map
		{
			if (MapPositionX != CellX || MapPositionY != CellY)
			{
				SorroundingWalls += Map[MapPositionX].Cells[MapPositionY];
			}
		}
		else
		{
			++SorroundingWalls;
		}

		++MapPositionY;
		if (MapPositionY > CellY + 1)
		{
			MapPositionY = CellY - 1;
			++MapPositionX;
		}
	}

	return SorroundingWalls;
}
