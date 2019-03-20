#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Vector.h"

const uint ELEMENTS = 10;
const uint STRING_SIZE = 256;
const char Vowel[] =
{
	'A', 'E', 'I', 'O', 'U', 'Y'
};
const char Consonants[] = 
{
	'B', 'C', 'D', 'F', 'G', 'H',
	'J', 'K', 'L', 'M', 'N', 'P',
	'Q', 'R', 'S', 'T', 'V', 'W',
	'X', 'Z'
};

int IsVowel(char t)
{
	for (size_t i = 0; i < _countof(Vowel); i++)
	{
		if ((int)t < 97)
		{
			if (t == Vowel[i])
				return 1;
		}
		else
		{
			if (t == (char)((int)Vowel[i] + 32))
				return 1;
		}
	}
	return 0;
}

int CharCmpi(char a, char b)
{
	if (a == b || (char)(int)(a + 32) == b || (char)(int)(b + 32) == a)
		return 1;

	return 0;
}

int NumberOfConsonantsInARow(char arr[], int current)
{
	int counter = 0;

	for (size_t i = current; i >= 0; i--)
	{
		if (!IsVowel(arr[i]))
			counter++;
		else break;
	}

	return counter;
}

void AddSentence(Vector * vec)
{
	char str[1024] = { NULL };
	int k = 1;
	for (size_t i = 0; i < rand() % 10 + 3; i++)
	{
		int r = rand();
		int stringSize = r % 5 + 2;

		uint t = rand() % 3;
		int add = 0;

		if (k > 1)
			add = 32;

		if (t && stringSize > 1)
		{
			if (k == 1)
				str[0] = (char)((int)Consonants[rand() % _countof(Consonants)] + add);
			else
				str[k++] = (char)((int)Consonants[rand() % _countof(Consonants)] + add);
		}
		else
		{
			if (k == 1)
				str[0] = (char)((int)Vowel[rand() % _countof(Vowel)] + add);
			else
				str[k++] = (char)((int)Vowel[rand() % _countof(Vowel)] + add);
			
		}

		add = 32;

		int before = k;

		for (k; k < stringSize + before; k++)
		{
			if (IsVowel(str[k - 1]))
			{
				char c = (char)((int)Consonants[rand() % _countof(Consonants)] + add);

				str[k] = c;
			}
			else
			{
				if (t && NumberOfConsonantsInARow(str, k) < 3)
				{
					char c = (char)((int)Consonants[rand() % _countof(Consonants)] + add);
					str[k] = c;
				}
				else
				{
					char c = (char)((int)Vowel[rand() % _countof(Vowel)] + add);
					str[k] = c;
				}
			}
		}

		str[k++] = ' ';
	}
	
	str[k - 1] = '.';
	str[k] = ' ';
	if (!(rand() % 3))
		str[k] = '\n';

	vec->PushBack(vec, str);
}

int main(int argc, char * argv[])
{
	srand(time(0));
	Vector strVec = NewVector();

	strVec.Create(&strVec, sizeof(char) * STRING_SIZE);

	for (size_t i = 0; i < 65; i++)
	{
		AddSentence(&strVec);
	}

	printf("String Vector\n");

	FILE * fp = NULL;
	fopen_s(&fp, "file1.txt", "w");
	if (fp)
	{
		
		for (size_t i = 0; i < strVec.Size; i++)
		{

			fprintf_s(fp, "%s", (char*)strVec.At(&strVec, i));
			printf("%s", (char*)strVec.At(&strVec, i));
		}

		fclose(fp);
	}

	strVec.Free(&strVec);

	printf("\n");
	system("pause");
	return 0;
}