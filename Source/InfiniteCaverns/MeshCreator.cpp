// Daniel Alejandro Castro García <dandev237@gmail.com>

#include "InfiniteCaverns.h"
#include "MeshCreator.h"

Node::Node(FVector _Pos)
{
	Position = _Pos;
}

ControlNode::ControlNode(FVector _Pos, bool _Active, float SquareSize) : Node(_Pos)
{
	Active = _Active;
	Above = Node(_Pos + FVector{ 0.0f, 0.0f, 1.0f } * SquareSize);
	Right = Node(_Pos + FVector{ 1.0f, 0.0f, 0.0f } * SquareSize);
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

void MeshCreator::CreateMesh(const TArray<int32> &Map, const float &SquareSize, const int32 &Width, const int32 &Height)
{
	MeshCreator::MeshGrid = SquareGrid(Map, SquareSize, Width, Height);

	for (int x = 0; x < Width - 1; ++x)
	{
		for (int y = 0; y < Height - 1; ++y)
		{
			TriangulateSquare(MeshGrid.SquaresGrid[x][y]);
		}
	}
}

SquareGrid::SquareGrid(const TArray<int32, FDefaultAllocator> &Map, const float &SquareSize, const int32 &Width, const int32 &Height)
{
	float MapWidth = Width * SquareSize;
	float MapHeight = Height * SquareSize;

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
				FVector NodePosition = FVector{ -MapWidth / 2 + x*SquareSize + SquareSize / 2, 0, -MapHeight / 2 + y*SquareSize + SquareSize / 2 };
				Nodes.Add(ControlNode(NodePosition, Map[y*Width + x] == 1, SquareSize));
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

	//REFERENCE IMAGE: http://jamie-wong.com/images/14-08-11/marching-squares-mapping.png
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
	MeshFromPoints(Points);		//RETURN PMC  here
}

void MeshCreator::MeshFromPoints(TArray<Node> &Points)
{
	TArray<FVector> Vertices;
	TArray<TTuple<int32, int32, int32>> Triangles;	//Each triangle is defined by a tuple of three integers (vertices in clockwise order)

	for (int i = 0; i < Points.Num(); ++i)
	{
		if (Points[i].Index == -1) //Vertex is not initialized
		{
			Points[i].Index = Vertices.Num();	//Get the actual count of elements in Vertices and use it as vertex index
			Vertices.Add(Points[i].Position);
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
		Triangles.Add(TriangleFromVertices(Points[0], Points[4], Points[5]));

	case 5:
		Triangles.Add(TriangleFromVertices(Points[0], Points[3], Points[4]));

	case 4:
		Triangles.Add(TriangleFromVertices(Points[0], Points[2], Points[3]));

	case 3:
		Triangles.Add(TriangleFromVertices(Points[0], Points[1], Points[2]));
		break;

	default:
		break;
	}
}

TTuple<int32, int32, int32> MeshCreator::TriangleFromVertices(const Node &Node1, const Node &Node2, const Node &Node3)
{
	//The vertices MUST be sorted in clockwise order!!
	return TTuple<int32, int32, int32>{Node1.Index, Node2.Index, Node3.Index};
}