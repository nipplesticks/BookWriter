#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Vector.h"

typedef struct
{
	char Str[256];
	int Freq;

}FollowUpWord;

typedef struct
{
	Vector CommonWordsAfter; // pointers to other words
	char Str[256];

}Word;

char ToLower(char in);
void LoadBook(Vector * v, const char * name);
void WriteSentence(Vector * v);

void sortByFreq(Vector * v);


#define NUMBER_OF_BOOKS 7

int main(int argc, char * argv[])
{

	srand(time(0));
	Vector books[NUMBER_OF_BOOKS];

	for (int i = 0; i < NUMBER_OF_BOOKS; i++)
	{
		books[i] = NewVector();
		books[i].Create(&books[i], sizeof(char) * 256);
	}

	LoadBook(&books[0], "Assets/Thermodynamics.txt");
	LoadBook(&books[1], "Assets/Book1.txt");
	LoadBook(&books[2], "Assets/Book2.txt");
	LoadBook(&books[3], "Assets/Book3.txt");
	LoadBook(&books[4], "Assets/Book4.txt");
	LoadBook(&books[5], "Assets/Book5.txt");
	LoadBook(&books[6], "Assets/BoringText.txt");

	Vector allWords = NewVector();
	allWords.Create(&allWords, sizeof(Word));

	for (int bookType = 0; bookType < NUMBER_OF_BOOKS; bookType++) // All books
	{
		for (int vecIndex = 0; vecIndex < books[bookType].Size; vecIndex++) //All words in the current book
		{
			const char * w = books[bookType].At(&books[bookType], vecIndex); // Target word

			int found = 0;
			int index = 0;
			for (int i = 0; i < allWords.Size; i++) // Checks if the word already exists in the allWordVector
			{
				Word * target = allWords.At(&allWords, i);
				if (!strcmp(w, target->Str))
				{
					found = 1;
					index = i;
					break;
				}
			}

			if (found)
			{
				Word * target = allWords.At(&allWords, index);

				if (vecIndex + 1 != books[bookType].Size)
				{
					const char * followUp = books[bookType].At(&books[bookType], vecIndex + 1);

					int needsNew = 1;
					for (int i = 0; i < target->CommonWordsAfter.Size; i++)
					{
						FollowUpWord * fuw = target->CommonWordsAfter.At(&target->CommonWordsAfter, i);

						if (!strcmp(followUp, fuw->Str))
						{
							fuw->Freq++;
							needsNew = 0;
							break;
						}
					}
					if (needsNew)
					{
						FollowUpWord fuw;
						fuw.Freq = 1;
						strcpy_s(fuw.Str, sizeof(char) * 256, followUp);


						target->CommonWordsAfter.PushBack(&target->CommonWordsAfter, &fuw);
					}
				}
			}
			else
			{
				Word newWord;
				strcpy_s(newWord.Str, sizeof(char) * 256, w);
				newWord.CommonWordsAfter = NewVector();
				newWord.CommonWordsAfter.Create(&newWord.CommonWordsAfter, sizeof(FollowUpWord));
				if (vecIndex + 1 != books[bookType].Size)
				{
					const char * followUp = books[bookType].At(&books[bookType], vecIndex + 1);
					FollowUpWord fuw;
					fuw.Freq = 1;
					strcpy_s(fuw.Str, sizeof(char) * 256, followUp);

					newWord.CommonWordsAfter.PushBack(&newWord.CommonWordsAfter, &fuw);
				}
				allWords.PushBack(&allWords, &newWord);
			}
		}
	}

	for (int i = 0; i < allWords.Size; i++)
	{
		Word * w = allWords.At(&allWords, i);
		sortByFreq(&w->CommonWordsAfter);
	}

	for (int i = 0; i < 50; i++)
	{
		WriteSentence(&allWords);
		printf("\n");
	}

	for (int i = 0; i < NUMBER_OF_BOOKS; i++)
	{
		books[i].Free(&books[i]);
	}

	printf("\n");
	system("pause");
	return 0;
}

char ToLower(char in)
{
	char out = in;

	if (in > 64 && in < 91)
		out = in + 32;

	return out;
}

void LoadBook(Vector * v, const char * name)
{
	FILE * fp = NULL;

	fopen_s(&fp, name, "r");

	if (fp)
	{
		char str[1024] = { NULL };

		char strBufferLast[1024] = { NULL };
		int strBufferCount = 0;

		while (fgets(str, 1024, fp) != NULL)
		{
			char word[256] = { NULL };
			int wordCounter = 0;
			int lastIndex = 0;

			for (int i = 0; i < strBufferCount; i++)
			{
				char target = strBufferLast[i];
				if ((((int)target > 64 && (int)target < 91) || ((int)target > 96 && (int)target < 123)))
				{
					word[wordCounter++] = ToLower(strBufferLast[i]);
				}
				strBufferLast[i] = NULL;
			}
			strBufferCount = 0;

			for (int i = 0; i < 1024; i++)
			{
				char target = str[i];

				if ((((int)target > 64 && (int)target < 91) || ((int)target > 96 && (int)target < 123)))
				{
					word[wordCounter++] = ToLower(target);
					lastIndex = i;
					str[i] = NULL;
				}
				else if (target == ' ' && wordCounter > 0)
				{
					v->PushBack(v, word);
					for (int k = 0; k < wordCounter; k++)
						word[k] = NULL;
					wordCounter = 0;
				}
			}
			for (int i = lastIndex + 1; i < 1024; i++)
			{
				if ((str[i - 1] != NULL && str[i] != NULL))
				{
					strBufferLast[i] = str[i];
					strBufferCount++;
				}
			}
		}

		fclose(fp);
	}
}

void WriteSentence(Vector * v)
{
	int start = rand() % v->Size;

	int canContinue = 1;

	int numberOfWords = rand() % 10 + 7;

	for (int i = 0; i < numberOfWords && canContinue; i++)
	{
		Word * w = v->At(v, start);
		printf("%s", w->Str);
		int sum = 0;
		for (int k = 0; k < w->CommonWordsAfter.Size; k++)
		{
			FollowUpWord * fuw = w->CommonWordsAfter.At(&w->CommonWordsAfter, k);
			sum += fuw->Freq;
		}
		double prcnt = (double)rand() / (double)RAND_MAX;
		double randomSelection = (double)sum * prcnt;
		sum = 0;

		char newString[256] = { NULL };

		for (int k = w->CommonWordsAfter.Size - 1; k >= 0; k--)
		{
			FollowUpWord * fuw = w->CommonWordsAfter.At(&w->CommonWordsAfter, k);
			sum += fuw->Freq;
			if (sum >= randomSelection)
			{
				strcpy_s(newString, 256 * sizeof(char), fuw->Str);
				break;
			}
		}

		int startBefore = start;

		for (int k = 0; k < v->Size; k++)
		{
			Word * newWord = v->At(v, k);

			if (!strcmp(newString, newWord->Str))
			{
				start = k;
				break;
			}
		}

		canContinue = startBefore - start;
		if (canContinue && i + 1 != numberOfWords)
			printf(" ");

	}
	printf(".\n");
}

void sortByFreq(Vector * v)
{
	FollowUpWord * fuw1 = NULL;
	FollowUpWord * fuw2 = NULL;

	int i, j;

	for (i = 0; i < (int)v->Size - 1; i++)
	{
		for (j = 0; j < (int)v->Size - i - 1; j++)
		{
			fuw1 = v->At(v, j);
			fuw2 = v->At(v, j + 1);

			if (fuw1->Freq < fuw2->Freq)
			{
				v->Swap(v, j, j + 1);
			}

		}
	}
}
