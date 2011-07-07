#include <stdlib.h>
#include <stdio.h>

struct STable
{
	int table[9];
	int best;
};

int checkPos(int *table)
{
	// диагонали
	if (table[0] == table[4] && table[4] == table[8])
		return table[0];
	if (table[2] == table[4] && table[4] == table[6])
		return table[2];

	// горизонтали
	if (table[0] == table[1] && table[1] == table[2])
		return table[0];
	if (table[3] == table[4] && table[4] == table[5])
		return table[3];
	if (table[6] == table[7] && table[7] == table[8])
		return table[6];

	// вертикали
	if (table[0] == table[3] && table[3] == table[6])
		return table[0];
	if (table[1] == table[4] && table[4] == table[7])
		return table[1];
	if (table[2] == table[5] && table[5] == table[8])
		return table[2];

	// или ничья
	return -1;
}


void step(struct STable * ret, struct STable table, int moveColor, int tableSz)
{
	int i = 0;
	int j = 0;

	for(j = 0; j < tableSz; ++j)
	{
		for (i = 0; i < 9; ++i)
		{
			if (table.table[i] == -1)
			{
				ret[j].table[i] = moveColor;
				++j;
			}
		}
	}
}

int main()
{
	struct STable rootTable;
	struct STable *stepTable;
	int i = 0;
	memset(rootTable.table, -1, 9 * sizeof(int));
	rootTable.best = -1;

	//  считываем позицию
	for (i = 0; i < 9; ++i)
	{
		char temp;
		scanf("%c", &temp);
		if (temp == 'X')
			rootTable.table[i] = 1;
		else
			if (temp == 'O')
				rootTable.table[i] = 0;
			else
				if (temp == 10)
					--i;
	}

	stepTable = (struct STable *) malloc(3 * sizeof(struct STable));
	for (i = 0; i < 3; ++i)
		memcpy(stepTable[i].table, rootTable.table, 9 * sizeof(int));
	// первый шаг, ходят кресты
	step(stepTable, rootTable, 1, 3);

	for(i = 0; i < 3; ++i)
	{
		// если черные не выиграли после хода, то настает ход белых
		// белые могут выграть только этим ходом
		if (checkPos(stepTable[i].table) == -1)
		{
			int j = 0;
			int winFlag = 0;
			struct STable *secondStepTable = (struct STable *) malloc(2 * sizeof(struct STable));
			for (j = 0; j < 2; ++j)
				memcpy(secondStepTable[j].table, stepTable[i].table, 9 * sizeof(int));
			// ходят белые, 2 варианта
			step(secondStepTable, stepTable[i], 0, 2);
			for (j = 0; j < 2; ++j)
			{
				// если белые выигрывают, то метим в stepTable[i].best = 0
				if (checkPos(secondStepTable[j].table) == 0)
				{
					stepTable[i].best = 0;
					break;
				}
				// иначе надо проверить выиграют ли черные
				// если выиграют в обоих случаях - значит черные выигрывают игру,
				// если только в одном или ни разу - метим stepTable[i].best = -1
				else
				{
					struct STable *lastStep = (struct STable *) malloc(sizeof(struct STable));
					memcpy(lastStep->table, secondStepTable[j].table, 9 * sizeof(int));
					step(lastStep, secondStepTable[j], 1, 1);
					if (checkPos(lastStep->table) == 1)
						++winFlag;
					free(lastStep);
				}
			}
			if (winFlag == 2)
			{
				printf("Crosses win");
				free(stepTable);
				free(secondStepTable);
				return 0;
			}
			else
			{
				if(stepTable[i].best != 0)
					stepTable[i].best = -1;
			}

			free(secondStepTable);
		}
		// если черные выиграли после хода, то они выиграли
		else
		{
			printf("Crosses win");
			free(stepTable);
			return 0;
		}
	}


	if (stepTable[0].best == 0 && stepTable[1].best == 0 && stepTable[2].best == 0)
	{
		printf("Ouths win");
	}
	else
		printf("Draw");

	free(stepTable);
	return 0;
}