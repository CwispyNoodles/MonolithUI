// Fill out your copyright notice in the Description page of Project Settings.


#include "Catenary/CatenaryBuilder.h"

FCatenaryBuilder::FCatenaryBuilder(const FVector2D& InSize, const FSlateCatenaryPaintContext& PaintContext)
	: RenderTransform(PaintContext.GetRenderTransform())
	, SingleColor(PaintContext.TintColor)
	, TextureCoord2(InSize.X, 1.0f)
	, CoordScaleV(InSize.Y * 0.01f)
{
	const float LineThickness = 2 * UE_SQRT_2 + TextureCoord2.X;
	HalfLineThickness = LineThickness / 2 + TextureCoord2.Y;
}

void FCatenaryBuilder::BuildCatenaryPoints(FCatenaryArguments InArgs, TArray<FVector2D>& InCatenaryPoints)
{
	const FVector2D InStart = InArgs.P1ConnectionSchema.Point;
	const FVector2D InEnd = InArgs.P2ConnectionSchema.Point;
	const int Segments = InArgs.Segments;
	const float L = InArgs.Length;
	const int Iterations = InArgs.Iterations;
	
	bool bIsFlipped = InStart.X > InEnd.X;
	FVector2D P1 = bIsFlipped ? InEnd : InStart;
	FVector2D P2 = bIsFlipped ? InStart : InEnd;
	const FVector2D Diff = P2 - P1;
	    
	InCatenaryPoints.Empty();
	
	// Fully Taut
	if (Segments == 1 || Diff.Length() > L)
	{
		InCatenaryPoints.Add(InEnd);
		InCatenaryPoints.Add(InStart);
		return;
	}

	// Avoids +/- inf asymptotes
	const float H = Diff.X == 0.0f ? 1.0f : Diff.X;
	const float V = Diff.Y == 0.0f ? 1.0f : Diff.Y;
	
	float A = 0.0f;

	// Calculate A
	{
		const float IntervalStep = InArgs.IntervalStep;

		// Because there is no closed form solution for A, we must iteratively solve for it
		// using the equation sqrt(L^2 - V^2) = 2 * a * sinh(H/2A) as a base.
		auto ShouldIterateA = [&]() -> bool
		{
			return FMath::Sqrt(L*L-V*V) < 2 * A * FMath::Sinh(H/(2*A));
		};
		
		// Find a rough estimate for A using a relatively large IntervalStep
		do
		{
			A += IntervalStep;
		}
		while (ShouldIterateA());

		// Next refine A via bisection.
		float PrevA = A - IntervalStep;
		float NextA = A;
		int Count = 0;
		const float Precision = InArgs.Precision;
		
		do
		{
			A = (PrevA + NextA) / 2.0f;
			if (ShouldIterateA())
			{
				PrevA = A;
			}
			else
			{
				NextA = A;
			}
			Count++;
		}
		while(NextA - PrevA > Precision && Count < Iterations);
	}
	
	const float P = (H - A * FMath::Loge((L + V) / (L - V))) / 2.0f;;
	// Q = V - L coth(H/2A)
	const float S = H / (2 * A);
	const float Q = (V - L * (FMath::Cosh(S) / FMath::Sinh(S))) / 2.0f;;

	// We add P1 and P2 directly into CatenaryPoints 
	for (int i = 1; i < Segments; i++)
	{
		const float X = H*(float)i/(float)Segments;
		const float Y = A * FMath::Cosh((X-P) / A)+Q;
		FVector2D Point(X,Y);

		// Note: I subtracted Point from End due to how the Unreal coordinate system is set up.
		InCatenaryPoints.Add(P2 - Point);
	}

	InCatenaryPoints.Insert(P2, 0);
	InCatenaryPoints.Add(P1);
}

void FCatenaryBuilder::BuildCatenaryGeometry(TArray<FVector2D> InPoints)
{
	for (auto Point : InPoints)
	{
		AppendPoint(Point);
	}
	Finish();
}

void FCatenaryBuilder::AppendPoint(FVector2D NewPoint)
{
	if (NumPointsAdded == 0)
	{
		LastPointAdded[0] = LastPointAdded[1] = NewPoint;
		NumPointsAdded++;
		return;
	}

	// We only add vertexes for the previous line segment.
	// This is because we want to average the previous and new normals
	// In order to prevent overlapping line segments on the spline.
	// These occur especially when curvature is high.
	const FVector2D NewNormal = FVector2D(LastPointAdded[0].Y - NewPoint.Y, NewPoint.X - LastPointAdded[0].X).GetSafeNormal();

	if (NumPointsAdded == 2)
	{
		// Once we have two points, we have a normal, so we can generate the first bit of geometry.
		const FVector2D LastUp = LastNormal*HalfLineThickness;

		Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(LastPointAdded[1] + LastUp), FVector2f(1.0f, CurrentCoordV), TextureCoord2, SingleColor));
		Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(LastPointAdded[1] - LastUp), FVector2f(0.0f, CurrentCoordV), TextureCoord2, SingleColor));
	}

	if (NumPointsAdded >= 2)
	{
		const FVector2D AveragedUp = (0.5f*(NewNormal + LastNormal)).GetSafeNormal()*HalfLineThickness;

		CurrentLength += FVector2D::Distance(LastPointAdded[1], LastPointAdded[0]);
		CurrentCoordV = CurrentLength * CoordScaleV;

		Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(LastPointAdded[0] + AveragedUp), FVector2f(1.0f, CurrentCoordV), TextureCoord2, SingleColor));
		Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(LastPointAdded[0] - AveragedUp), FVector2f(0.0f, CurrentCoordV), TextureCoord2, SingleColor));

		const int32 NumVerts = Vertices.Num();

		// Counterclockwise winding on triangles
		Indices.Add(NumVerts - 3);
		Indices.Add(NumVerts - 4);
		Indices.Add(NumVerts - 2);

		Indices.Add(NumVerts - 3);
		Indices.Add(NumVerts - 2);
		Indices.Add(NumVerts - 1);
	}

	LastPointAdded[1] = LastPointAdded[0];
	LastPointAdded[0] = NewPoint;
	LastNormal = NewNormal;

	++NumPointsAdded;
}

void FCatenaryBuilder::Finish()
{
	if (NumPointsAdded < 3)
	{
		// Line builder needs at least two line segments (3 points) to
		// complete building its geometry.
		// This will only happen in the case when we have a straight line.
		AppendPoint(LastPointAdded[0]);
	}
	else
	{
		// We have added the last point, but the line builder only builds
		// geometry for the previous line segment. Build geometry for the
		// last line segment.
		const FVector2D LastUp = LastNormal * HalfLineThickness;

		CurrentLength += FVector2D::Distance(LastPointAdded[1], LastPointAdded[0]);
		CurrentCoordV = CurrentLength * CoordScaleV;

		Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(LastPointAdded[0] + LastUp), FVector2f(1.0f, CurrentCoordV), TextureCoord2, SingleColor));
		Vertices.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(RenderTransform, FVector2f(LastPointAdded[0] - LastUp), FVector2f(0.0f, CurrentCoordV), TextureCoord2, SingleColor));	

		const int32 NumVerts = Vertices.Num();

		// Counterclockwise winding on triangles
		Indices.Add(NumVerts - 3);
		Indices.Add(NumVerts - 4);
		Indices.Add(NumVerts - 2);

		Indices.Add(NumVerts - 3);
		Indices.Add(NumVerts - 2);
		Indices.Add(NumVerts - 1);
	}
}
