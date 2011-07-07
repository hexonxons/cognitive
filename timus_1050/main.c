#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int flag = 1;
	int isOpen = 1;
	int isParagraph = 1;

	int buflen = 0;
	int inlen = 0;
	int outlen = 0;

	char *output;
	char *input;
	char *buffer;

	int i = 0;
	int openPos = 0;

	output = (char *) malloc (30000 * sizeof(char));
	buffer = (char *) malloc (30000 * sizeof(char));
	memset(buffer, 0, 30000);
	memset(output, 0, 30000);

	while(flag)
	{
		input = (char *) malloc (100 * sizeof(char));
		memset(input, 0, 100);

		fgets(input, 100, stdin);
		inlen = strlen(input);
		if(!strncmp(input, "\\endinput", 9))
		{
			if (input[10] != 0)
				continue;
			flag = 0;
			continue;
		}

		for(i = 0; i < inlen; ++i)
		{
			// если идет двойной слеш
			if ((input[i] == '\\') && (input[i + 1] == '\\'))
			{
				sprintf(buffer + buflen, "\\\\");
				buflen += 2;
				i += 1;
				continue;
			}

			// если конец параграфа
			if ((input[i] == '\\' && input[i + 1] == 'p' && input[i + 2] == 'a' && input[i + 3] == 'r') || (input[i] == 10 && input[i + 1] == 0 && i == 0))
			{
				sprintf(buffer + buflen, input + i);
				buflen += inlen - i;
				if(!isOpen)
					sprintf(buffer + openPos, buffer + openPos + 2);
				isOpen = 1;
				sprintf(output + outlen, "%s", buffer);
				outlen += buflen;
				free(buffer);
				buflen = 0;
				buffer = (char *) malloc (30000 * sizeof(char));
				memset(buffer, 0, 30000);
				i = inlen;
				continue;
			}

			if(input[i] == '"' && ((i == 0 || input[i - 1] != '\\') || (i != 1 || input[i - 2] == '\\')))
			{
				// Если открывающая/ закрывающая кавычка - печатаем во временный буффер
				if(isOpen)
				{
					sprintf(buffer + buflen, "``");
					isOpen = 0;
					buflen += 2;
					openPos = buflen - 2;
				}
				else
				{
					sprintf(buffer + buflen, "''");
					isOpen = 1;
					buflen += 2;
				}
			}
			else
			{
				sprintf(buffer + buflen, "%c", input[i]);
				++buflen;
			}
		}
		free(input);
	}
	printf("%s", output);
	printf("%s", input);
}