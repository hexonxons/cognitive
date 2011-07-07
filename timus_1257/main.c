#include <stdlib.h>
#include <stdio.h>

#define concat(a,b) a ## b

struct pair 
{
	char *preHyphen;
	char *postHyphen;
};


int *findMatches(char *word, struct pair *dic, int dicSz)
{
	int i = 0;
	int j = 0;
	int *ret = (int *) malloc (dicSz * sizeof(int));
	memset(ret, -1, dicSz * sizeof(int));

	for (i = 0; i < dicSz; ++i)
	{
		int preLen = strlen(dic[i].preHyphen);
		int postLen = strlen(dic[i].postHyphen);

		char *temp = (char *) malloc ((preLen + postLen) * sizeof(char));
		memset(temp, 0, (preLen + postLen) * sizeof(char));
		memcpy(temp, dic[i].preHyphen, preLen);
		memcpy(temp + preLen, dic[i].postHyphen, postLen);
		temp[preLen + postLen - 1] = 0;
		if (strstr(word, temp))
			ret[j++] = i;
		free(temp);
	}
	return ret;
}

int main()
{
	char *data = (char *) malloc (250 * 451 * sizeof(char));
	int i = 0;
	// ���������� ���� � �������
	int num = 0;
	char ch = 0;
	struct pair *dict;
	// ������ �����
	char *word;
	int j = 0;

	memset(data, 0, 250 * 450 * sizeof(char));

	scanf("%d ", &num);
	dict = (struct pair *) malloc (num * sizeof(struct pair));

	// ��������� �������
	for (i = 0; i <  num; ++i)
	{
		int cnt = 0;
		int len = 0;
		// 40 �������� + -
		char *str = (char *) malloc (41 * sizeof(char));
		memset(str, 0, 41 * sizeof(char));

		gets(str);
		len = strlen(str);

		while (str[cnt] != '-')
			++cnt;

		dict[i].preHyphen = (char *)malloc(cnt * sizeof(char));
		dict[i].postHyphen = (char *)malloc((len - cnt) * sizeof(char));

		strncpy(dict[i].preHyphen, str, cnt);
		dict[i].preHyphen[cnt] = 0;
		// ch - ������ ��� �������
		for (ch = 0; ch < cnt; ++ch)
			dict[i].preHyphen[ch] = tolower(dict[i].preHyphen[ch]);

		strncpy(dict[i].postHyphen, str + cnt + 1, (len - cnt));
		for (ch = cnt + 1; ch < len; ++ch)
			dict[i].preHyphen[ch] = tolower(dict[i].preHyphen[ch]);

		free(str);
	}

	// ���������� ������
	ch = fgetc(stdin);

	i = 0;
	while (ch != EOF)
	{
		data[i++] = ch;
		ch = fgetc(stdin);		
	}

	i = 0;

	word  = (char *) malloc (40 * sizeof(char));
	memset(word, 0, 40 * sizeof(char));
	// ���������� �������������� ������
	while (1)
	{
		int lengthCnt = 0;
		while(lengthCnt != 40)
		{
			// ��������� �� �����
			if (data[i] != ' ' && data[i] != 10)
			{
				word[j++] = data[i++];
				++lengthCnt;
			}
			else
				// ���� ������ ��� ������� ������ - ����� �����������, �� �������� ���
				// � ����� ����� ���� ����� ����������
				if(data[i] == ' ' || data[i] == 10)
				{
					++lengthCnt;
					// ���� ������� ������ - ������� ������
					if (data[i] == 10)
						lengthCnt = 0;
					printf("%s%c", word, data[i]);
					memset(word, 0, 40 * sizeof(char));
					j = 0;
					++i;
				}
		}
		// ���� �� 40x ������� ����� ����������� - ������ ��������� ������� ������
		if (word[0] == 0 || data[i] == ' ' || data[i] == 10)
		{
			printf("%s\n", word);
			memset(word, 0, 40 * sizeof(char));
			j = 0;
			lengthCnt = 0;
		}
		// ����� ����� ���������� �����...
		else
		{
			// ���������� ���� � word �� ����, ��� ��� ��������� ����� 40 �������� � ������
			int lettersBeforeEnd = j;
			int flag = 1;
			// ������ ������� ���������, ������� ��������� � ����� ������
			int *arr;// = (int *) malloc (num * sizeof(int));
			// ����� ����������� ���������� ��������
			int maxPossibleLen = 0;
			// ����� ����������� ���������� ��������
			int maxPossibleNum = -1;
			// ������
			char *outBuffer = (char *) malloc ((strlen(word) + 2) * sizeof(char));
			// ���������� ����� �� �������/�������� ������
			while (flag)
			{
				if (data[i] != ' ' && data[i] != 10)
					word[j++] = data[i++];
				else
					if(data[i] == ' ' || data[i] == 10)
						flag = 0;
			}
			arr = findMatches(word, dict, num);
			j = 0;

			while (arr[j] != -1)
			{
				int preLen = strlen(dict[arr[j]].preHyphen);
				if (preLen < lettersBeforeEnd && preLen > maxPossibleLen)
				{
					maxPossibleLen = preLen;
					maxPossibleNum = arr[j];
				}
				++j;
			}
			// ����� ����� �� ������
			// ������ �������� ��� pre-����� + '-' + '\n'
			for (j = 0; j < strlen(dict[maxPossibleNum].preHyphen); ++j)
				sprintf(outBuffer + j, "%c", word[j]);
			sprintf(outBuffer + j + 1, "-");
			sprintf(outBuffer + j + 2, "\n");
			sprintf(outBuffer + j + 3, "%s", word + j);

			printf("%s", outBuffer);
			free(outBuffer);
			memset(word, 0, 40 * sizeof(char));
		}

	}
}