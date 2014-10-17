

#include "Ballerts.h"
#include "BalaLib.h"


UBalaLib::UBalaLib(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}



#define EPS (1e-4)

#define MAXN 30

float CL[MAXN];
float CF[MAXN];
bool PirosEl[MAXN][MAXN];

// M[i]: i-edik fiuhoz mi a lanyindex
int M[MAXN];
// RevM[i]: i-edik lanyhoz mi a fiuindex
int RevM[MAXN];

TArray<int32> UBalaLib::MaxWeightMatching(TArray<float> Weights, const int32 N)
{

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

	bool TeljesParositas = false;

	while (!TeljesParositas)
	{
		
		//javitoutas a piros reszgrafban
		bool VanJavitoUt = false;
		do
		{
			//mik a piros elek - ezt kesobb lehetne hatekonyabban
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < N; j++)
				{

					if (CF[i] + CL[j] < Weights[i*N + j] + EPS &&  CF[i] + CL[j] > Weights[i*N + j] - EPS)
					{
						PirosEl[j][i] = true;
					}
					else
					{
						PirosEl[j][i] = false;
					}
				}
			}

			//Fiubol indul, Lany-ra vegzodik, int indexek
			TArray<int32> javitoUt;
			javitoUt.Init(2 * N);
			javitoUt.Empty(2 * N);
			for (int i = 0; i < N; i++)
			{
				//parositatlan
				if (M[i] == -1)
				{
					javitoUt.Add(i);
					break;
				}
			}
			if (javitoUt.Num() == 0) break;//nincs javitout a pirosban

			
			
			bool VanPirosSzomszed = false;
			do
			{
				
				int LastIndex = javitoUt.Last();
				bool breakFlag = false;
				for (int j = 0; j < N; j++)
				{
					if (PirosEl[j][LastIndex])
					{
						VanPirosSzomszed = true;
						javitoUt.Add(j);
						if (RevM[j] != -1)
						{
							javitoUt.Add(RevM[j]);
							break;
						}
						else
						{
							VanJavitoUt = true;
							for (int i = 0; i < javitoUt.Num() - 1; i++)
							{
								if (i % 2 == 0)
								{
									M[javitoUt[i]] = javitoUt[i + 1];
									RevM[javitoUt[i + 1]] = javitoUt[i];
								}
							}
							breakFlag = true;
							break;
							
						}
					}
				}
				if (breakFlag == true) break;
			} while (VanPirosSzomszed);
			
				
		} while (VanJavitoUt);
		
		//besorolas
		//delta kiszamolas
		//cimke korrigalas deltaval
	}



	TArray<int32> Result;
	Result.Init(N);
	Result.Empty(N);

	for (int i = 0; i < N; i++)
	{
		Result[i] = M[i];
	}

	return Result;

}

void UBalaLib::JavitoUtas(TArray<float> Weights, const int32 N)
{


	//javitoutas a piros reszgrafban
	bool VanJavitoUt = false;
	do
	{
		VanJavitoUt = false;

		//mik a piros elek - ezt kesobb lehetne hatekonyabban
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{

				if (CF[i] + CL[j] < Weights[i*N + j] + EPS &&  CF[i] + CL[j] > Weights[i*N + j] - EPS)
				{
					PirosEl[j][i] = true;
				}
				else
				{
					PirosEl[j][i] = false;
				}
			}
		}

		//Fiubol indul, Lany-ra vegzodik, int indexek
		TArray<int32> javitoUt;
		javitoUt.Init(2 * N);
		javitoUt.Empty(2 * N);
		for (int i = 0; i < N; i++)
		{
			//parositatlan
			if (M[i] == -1)
			{
				javitoUt.Add(i);
				break;
			}
		}
		if (javitoUt.Num() == 0) break;//biztos nincs javitout a pirosban



		bool VanPirosSzomszed = false;
		do
		{

			int LastIndex = javitoUt.Last();
			bool breakFlag = false;
			for (int j = 0; j < N; j++)
			{
				if (PirosEl[j][LastIndex])
				{
					VanPirosSzomszed = true;
					javitoUt.Add(j);
					if (RevM[j] != -1)
					{
						javitoUt.Add(RevM[j]);
						break;
					}
					else
					{
						VanJavitoUt = true;
						for (int i = 0; i < javitoUt.Num() - 1; i++)
						{
							if (i % 2 == 0)
							{
								M[javitoUt[i]] = javitoUt[i + 1];
								RevM[javitoUt[i + 1]] = javitoUt[i];
							}
						}
						breakFlag = true;
						break;

					}
				}
			}
			if (breakFlag == true) break;
		} while (VanPirosSzomszed);


	} while (VanJavitoUt);


}



