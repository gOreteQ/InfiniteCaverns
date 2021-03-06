// Daniel Alejandro Castro Garc�a <dandev237@gmail.com>

#include "InfiniteCaverns.h"
#include "MeshCreator.h"

Node::Node(FVector _Pos)
{
	Position = _Pos;
}

ControlNode::ControlNode(FVector _Pos, bool _Active, float Size) : Node(_Pos)
{
	Active = _Active;
	Above = Node(_Pos + (FVector::UpVector * (Size/2.0f)));
	Right = Node(_Pos + (FVector::ForwardVector * (Size/2.0f)));
}

Square::Square(ControlNode _TopLeft, ControlNode _TopRight, ControlNode _BottomLeft, ControlNode _BottomRight)
{
	TopLeft = _TopLeft;
	TopRight = _TopRight;
	BottomLeft = _BottomLeft;
	BottomRight = _BottomRight;

	Left = BottomLeft.Above;
	Right = BottomRight.Above;
	Top = TopLeft.Right;
	Bottom = BottomLeft.Right;

	//Marching squares configuration
	//Reference Image: http://jamie-wong.com/images/14-08-11/marching-squares-mapping.png
	if (TopLeft.Active)
	{
		SquareConfiguration += 8;
	}
	if (TopRight.Active)
	{
		SquareConfiguration += 4;
	}
	if (BottomRight.Active)
	{
		SquareConfiguration += 2;
	}
	if (BottomLeft.Active)
	{
		SquareConfiguration += 1;
	}
}

void MeshCreator::CalculateTrianglesForMesh(const TArray<int32> &Map, const float &Size, const int32 &Width, const int32 &Height)
{
	MeshGrid = SquareGrid(Map, Size, Width, Height);

	//Worst-case scenario estimation for number of vertices contained in Vertices array
	int32 HorizontalVertices = (Width * 2 + 1) * (Height + 1);
	int32 VerticalVertices = (Height + 1) * Height;
	Vertices.Reserve(HorizontalVertices + VerticalVertices);

	for (int x = 0; x < MeshGrid.SquaresGrid.Num(); ++x)
	{
		for (int y = 0; y < MeshGrid.SquaresGrid[x].Num(); ++y)
		{
			TriangulateSquare(MeshGrid.SquaresGrid[x][y]);
		}
	}

	Vertices.Shrink();
}

SquareGrid::SquareGrid(const TArray<int32, FDefaultAllocator> &Map, const float &Size, const int32 &Width, const int32 &Height)
{
	float MapWidth = Width * Size;
	float MapHeight = Height * Size;

	//Create all necessary ControlNodes
	TArray<TArray<ControlNode>> GridControlNodes;
	GridControlNodes.Reserve(Width);
	{
		TArray<ControlNode> Nodes;
		Nodes.Reserve(Height);
		for (int x = 0; x < Width; ++x)
		{
			for (int y = 0; y < Height; ++y)
			{
				FVector NodePosition = FVector{ -MapWidth / 2.0f + x*Size + Size / 2.0f, 0, -MapHeight / 2.0f + y*Size + Size / 2.0f };
				Nodes.Add(ControlNode(NodePosition, Map[y*Width + x] == 1, Size));
			}
			GridControlNodes.Add(Nodes);
			Nodes.Reset();
		}
	}

	//Initialize SquaresGrid
	TArray<Square> SquareColumn;
	SquareColumn.Init(Square(), Height);
	SquaresGrid.Init(SquareColumn, Width);

	//Create every Square contained in SquaresGrid with the ControlNodes created before.
	for (int x = 0; x < Width - 1; ++x)
	{
		for (int y = 0; y < Height - 1; ++y)
		{
			SquaresGrid[x][y] = Square(GridControlNodes[x][y + 1], GridControlNodes[x + 1][y + 1], GridControlNodes[x + 1][y], GridControlNodes[x][y]);
		}
	}
}

void MeshCreator::TriangulateSquare(const Square &Square)
{
	TArray<Node> Points;
	Points.Reserve(6);

	//Marching squares algorithm
	//Reference image: http://jamie-wong.com/images/14-08-11/marching-squares-mapping.png
	switch (Square.SquareConfiguration)
	{
	case 1:
		Points.Add(Square.Bottom);
		Points.Add(Square.BottomLeft);
		Points.Add(Square.Left);
		break;

	case 2:
		Points.Add(Square.Right);
		Points.Add(Square.BottomRight);
		Points.Add(Square.Bottom);
		break;

	case 3:
		Points.Add(Square.Right);
		Points.Add(Square.BottomRight);
		Points.Add(Square.BottomLeft);
		Points.Add(Square.Left);
		break;

	case 4:
		Points.Add(Square.Top);
		Points.Add(Square.TopRight);
		Points.Add(Square.Right);
		break;

	case 5:
		Points.Add(Square.Top);
		Points.Add(Square.TopRight);
		Points.Add(Square.Right);
		Points.Add(Square.Bottom);
		Points.Add(Square.BottomLeft);
		Points.Add(Square.Left);
		break;

	case 6:
		Points.Add(Square.Top);
		Points.Add(Square.TopRight);
		Points.Add(Square.BottomRight);
		Points.Add(Square.Bottom);
		break;

	case 7:
		Points.Add(Square.Top);
		Points.Add(Square.TopRight);
		Points.Add(Square.BottomRight);
		Points.Add(Square.BottomLeft);
		Points.Add(Square.Left);
		break;

	case 8:
		Points.Add(Square.TopLeft);
		Points.Add(Square.Top);
		Points.Add(Square.Left);
		break;

	case 9:
		Points.Add(Square.TopLeft);
		Points.Add(Square.Top);
		Points.Add(Square.Bottom);
		Points.Add(Square.BottomLeft);
		break;

	case 10:
		Points.Add(Square.TopLeft);
		Points.Add(Square.Top);
		Points.Add(Square.Right);
		Points.Add(Square.BottomRight);
		Points.Add(Square.Bottom);
		Points.Add(Square.Left);
		break;

	case 11:
		Points.Add(Square.TopLeft);
		Points.Add(Square.Top);
		Points.Add(Square.Right);
		Points.Add(Square.BottomRight);
		Points.Add(Square.BottomLeft);
		break;

	case 12:
		Points.Add(Square.TopLeft);
		Points.Add(Square.TopRight);
		Points.Add(Square.Right);
		Points.Add(Square.Left);
		break;

	case 13:
		Points.Add(Square.TopLeft);
		Points.Add(Square.TopRight);
		Points.Add(Square.Right);
		Points.Add(Square.Bottom);
		Points.Add(Square.BottomLeft);
		break;

	case 14:
		Points.Add(Square.TopLeft);
		Points.Add(Square.TopRight);
		Points.Add(Square.BottomRight);
		Points.Add(Square.Bottom);
		Points.Add(Square.Left);
		break;

	case 15:
		Points.Add(Square.TopLeft);
		Points.Add(Square.TopRight);
		Points.Add(Square.BottomRight);
		Points.Add(Square.BottomLeft);
		break;

	default:	//Case 0 included here
		break;
	}

	Points.Shrink();
	MeshFromPoints(Points);
}

void MeshCreator::MeshFromPoints(TArray<Node> &Points)
{
	for (int i = 0; i < Points.Num(); ++i)
	{
		if (Points[i].Index == -1) //Vertex is not initialized
		{
			//Points[i].Index = (Vertices.Add(Points[i].Position)).AsInteger();	//Vertices is a set - Add returns pointer to the index of the added element in the Vertices set
			Points[i].Index = Vertices.AddUnique(Points[i].Position);
			GLog->Log("Vertice a�adido!!");
		}
	}

	//Triangulation part!
	//We want to make a number of triangles based on the number of vertices
	//3 Vertices -> 1 Triangle  (0,1,2)
	//4 Vertices -> 2 Triangles (0,1,2) , (0,2,3)
	//5 Vertices -> 3 Triangles (0,1,2) , (0,2,3) , (0,3,4)
	//6 Vertices -> 4 Triangles (0,1,2) , (0,2,3) , (0,3,4) , (0,4,5)

	switch (Points.Num())
	{
	case 6:
		TriangleFromVertices(Points[0].Index, Points[4].Index, Points[5].Index);

	case 5:
		TriangleFromVertices(Points[0].Index, Points[3].Index, Points[4].Index);

	case 4:
		TriangleFromVertices(Points[0].Index, Points[2].Index, Points[3].Index);

	case 3:
		TriangleFromVertices(Points[0].Index, Points[1].Index, Points[2].Index);
		break;

	default:
		break;
	}
}

void MeshCreator::TriangleFromVertices(const int32& Vertex1, const int32& Vertex2, const int32& Vertex3)
{
	Triangles.Add(Vertex1);
	Triangles.Add(Vertex2);
	Triangles.Add(Vertex3);
}