#include <iostream>
#include <windows.h>
using namespace std;

const int N = 500;

struct Solution
{
	int positions[N];
	int Energy;
};

//Swap random elements
void SwapElem(Solution* sol)
{
	int a = rand() % N;
	int b = a;
	int temp;
	while (a == b) { b = rand() % N; }
	temp = (*sol).positions[a];
	(*sol).positions[a] = (*sol).positions[b];
	(*sol).positions[b] = temp;
}
//Create and shuffle new solution
void NewSolution(Solution* sol)
{
	for (int i = 0; i < N; i++) (*sol).positions[i] = i;
	for (int i = 0; i < N; i++) SwapElem(sol);
}
//Calculate energy (amount of conflicts)
void CalcEnergy(Solution* sol)
{
	int conflicts = 0;
	int dx[4] = { -1,1,-1,1 };
	int dy[4] = { -1,1,1,-1 };
	int tx, ty;
	for (int i = 0; i < N; i++)
	{
		for (int dir = 0; dir < 4; dir++)
		{
			tx = i + dx[dir];
			ty = (*sol).positions[i] + dy[dir];
			while (tx >= 0 && tx < N && ty >= 0 && ty < N)
			{
				if ((*sol).positions[tx] == ty) conflicts++;
				tx += dx[dir];
				ty += dy[dir];
			}
		}
	}
	(*sol).Energy = conflicts;
}
//Copy solution from ref to aim
void CopySolution(Solution ref, Solution* aim)
{
	for (int i = 0; i < N; i++) (*aim).positions[i] = ref.positions[i];
	(*aim).Energy = ref.Energy;
}
//Print solution as text
void ShowSolution(Solution sol)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (j == sol.positions[i]) cout << "Q";
			else cout << "-";
		}
		cout << endl;
	}
}

int main()
{
	srand(time(NULL));
	system("chcp 1251>nul");

	double
		Tstart = 100.0,
		TCurrent = Tstart,
		Tend = 0.1,
		Talpha = 0.99;
	int IPT = 1000;
	double deltaE, Probability;
	bool New, Best;
	Solution SolCurrent, SolWorking, SolBest;
	int accepted;
	double genRand = 0.0;
	int randBorder = 10000;

	Best = false;
	NewSolution(&SolCurrent);
	CalcEnergy(&SolCurrent);

	cout << "Initial state: N = " << N << " and E = " << SolCurrent.Energy << endl;
	Sleep(1000);

	CopySolution(SolCurrent, &SolWorking);
	CopySolution(SolCurrent, &SolBest);
	
	while (TCurrent > Tend)
	{
		accepted = 0;
		for (int iter = 0; iter < IPT; iter++)
		{
			//Модифицируем решение
			New = false;
			SwapElem(&SolWorking);
			CalcEnergy(&SolWorking);

			//Если новое решение удачное
			if (SolWorking.Energy <= SolCurrent.Energy) New = true;
			//Если не удачное, проверяем критерий допуска
			else
			{
				deltaE = SolWorking.Energy - SolCurrent.Energy;
				Probability = exp(-deltaE / TCurrent);
				genRand = (rand() % randBorder) / (double)(randBorder);
				if (Probability > genRand)
				{
					accepted++;
					New = true;
				}
			}
			//Если нашлось (тем или иным образом) новое решение
			if (New)
			{
				//Копируем новое решение в текущее;
				New = false;
				CopySolution(SolWorking, &SolCurrent);
				//Проверяем, является ли текущее решение лучшим среди всех остальных
				if (SolCurrent.Energy < SolBest.Energy)
				{
					CopySolution(SolCurrent, &SolBest);
					Best = true;
				}
			}
			else
			{
				CopySolution(SolCurrent, &SolWorking);
			}
		}
		cout << "T: " << TCurrent << "\tE: " << SolBest.Energy << "\tA: " << accepted << endl;
		TCurrent *= Talpha;
		if (SolBest.Energy == 0) break;
	}
	//if (Best) ShowSolution(SolBest);

	system("pause>nul");
	return 0;
}