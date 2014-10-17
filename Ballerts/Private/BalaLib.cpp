// Fill out your copyright notice in the Description page of Project Settings.

#include "Ballerts.h"
#include "BalaLib.h"


UBalaLib::UBalaLib(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}


#define EPS (1e-4)

#define MAXN 100


void UBalaLib::Assignment(TArray<float> Weights, const int32 N, TArray<int32>& Result)
{
	if (N > MAXN) return;

	//cimkek
	float CL[MAXN];
	float CF[MAXN];

	// besorolashoz
	int Fiu[MAXN];
	int Lany[MAXN];

	bool PirosEl[MAXN][MAXN];

	// M[i]: i-edik fiuhoz mi a lanyindex
	int M[MAXN];
	// RevM[i]: i-edik lanyhoz mi a fiuindex
	int RevM[MAXN];


	//kezdo cimkezes
	for (int i = 0; i < N; i++)
	{
		CL[i] = 0.f;
		float maxW = Weights[i * N + 0];
		for (int j = 0; j < N; j++)
		{
			if (maxW < Weights[i * N + j]) maxW = Weights[i * N + j];
		}
		CF[i] = maxW;


		M[i] = -1;
		RevM[i] = -1;
	}

	//piros elek + kezdo parositas
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{

			if (CF[i] + CL[j] < Weights[i*N + j] + EPS &&  CF[i] + CL[j] > Weights[i*N + j] - EPS)
			{
				PirosEl[j][i] = true;
				if (M[i] == -1 && RevM[j] == -1)
				{
					M[i] = j;
					RevM[j] = i;
				}
			}
			else
			{
				PirosEl[j][i] = false;
			}
		}
	}

	////test data a javitoutashoz
	//for (int i = 0; i < N; i++) {
	//	M[i] = -1;
	//	RevM[i] = -1;
	//	for (int j = 0; j < N; j++) PirosEl[j][i] = false;
	//}
	//PirosEl[1][0] = true;
	//PirosEl[2][0] = true;
	//PirosEl[0][1] = true;
	//PirosEl[1][2] = true;
	//PirosEl[2][3] = true;
	//PirosEl[3][3] = true;

	//M[1] = 0;
	//RevM[0] = 1;
	//RevM[1] = 0;
	//M[0] = 1;
	////test data end

	TArray<TArray<int32>> Paths;

	do{

	//javitoutas a piros reszgrafban
	bool VanJavitoUt = true;
	do
	{
		if (VanJavitoUt)
		{
			Paths.Empty();
			for (int i = 0; i < N; i++)
			{
				//parositatlan
				if (M[i] == -1)
				{
					TArray<int32> OnePath;
					OnePath.Empty();
					OnePath.Add(i);
					Paths.Add(OnePath);
				}
			}
		}
		if (Paths.Num() == 0) break;

		VanJavitoUt = false;

		//Fiubol indul, Lany-ra vegzodik, int indexek
		TArray<int32> javitoUt = Paths.Pop();
		int LastIndex = javitoUt.Last();
		for (int j = 0; j < N; j++)
		{
			bool BreakFlag = false;
			if (PirosEl[j][LastIndex] && j != M[LastIndex])
			{
				if (RevM[j] != -1)
				{
					TArray<int32> AnotherPath = javitoUt;
					AnotherPath.Add(j);
					AnotherPath.Add(RevM[j]);
					Paths.Add(AnotherPath);
				}
				else
				{
					VanJavitoUt = true;
					javitoUt.Add(j);
					for (int i = 0; i < javitoUt.Num() - 1; i++)
					{
						if (i % 2 == 0)
						{
							M[javitoUt[i]] = javitoUt[i + 1];
							RevM[javitoUt[i + 1]] = javitoUt[i];
						}
					}
					BreakFlag = true;
				}
			}
			if (BreakFlag) break;
		}

	} while (VanJavitoUt || Paths.Num() > 0);

	bool TeljesParositas = true;
	for (int i = 0; i < N; i++)
	{
		if (M[i] == -1)
		{
			TeljesParositas = false;
			break;
		}
	}
	if (TeljesParositas)
	{
		Result.Empty();
		Result.AddZeroed(N);

		for (int i = 0; i < N; i++)
		{
			Result[i] = M[i];
		}
		return;
	}

	Paths.Empty();
	//besorolas (F1,F2,F3 megfeleloje: Fiu[i] = 1,2,3, Lanyra ugyanigy)
	for (int i = 0; i < N; i++)
	{
		if (M[i] == -1)
		{
			Fiu[i] = 1;
			TArray<int32> OnePath;
			OnePath.Empty();
			OnePath.Add(i);
			Paths.Add(OnePath);

		}
		else
		{
			Fiu[i] = 3;
		}
		if (RevM[i] == -1)
		{
			Lany[i] = 1;
		}
		else
		{
			Lany[i] = 3;
		}
	}
	//F2,L2 besorolasa
	//kezdopontokbol (F1) alternalo uton elert lanyok: L2, L2 párjai: F2
	do
	{
		TArray<int32> APath = Paths.Pop();
		int LastIndex = APath.Last();
		for (int j = 0; j < N; j++)
		{
			if (PirosEl[j][LastIndex] && j != M[LastIndex])
			{
				Lany[j] = 2;
				Fiu[RevM[j]] = 2;
				TArray<int32> OnePath = APath;
				OnePath.Add(j);
				OnePath.Add(RevM[j]);
				Paths.Add(OnePath);
			}
		}
	} while (Paths.Num() > 0);

	//delta kiszamolasa
	float delta = -1.f;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if ((Fiu[i] == 1 || Fiu[i] == 2) && (Lany[j] == 1 || Lany[j] == 3))
			{
				float tartalek = CF[i] + CL[j] - Weights[i * N + j];
				if (delta > tartalek || delta == -1.f)
				{
					delta = tartalek;
				}
			}
		}
	}

	//cimkeertekek frissitese
	for (int i = 0; i < N; i++)
	{
		if (Fiu[i] == 1 || Fiu[i] == 2)
		{
			CF[i] -= delta;
		}
		if (Lany[i] == 2)
		{
			CL[i] += delta;
		}
	}

	//mik az uj piros elek
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (PirosEl[j][i] && !(Fiu[i] == 3 && Lany[j] == 2))
			{
				continue;
			}
			if (FMath::Abs(CF[i] + CL[j] - Weights[i*N + j]) < EPS)
			{
				PirosEl[j][i] = true;
			}
			else
			{
				PirosEl[j][i] = false;
			}
		}
	}

	}
	while (true);

}
