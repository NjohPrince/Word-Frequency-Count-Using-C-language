#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<limits.h>

#define max_word 50
#define Max_words 8

typedef struct
{
	char word[max_word];
	int count;

}words_t;

char *strlwr(char *str)
{
	char *p = str;

	while(*p)
	{
		*p = tolower(*p);
		p++;
	}
	return str;
}

/* return 1 if c is alphabetic (a..z or A..Z), 0 otherwise */
int is_alpha (char c) 
{
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') 
		return 1;
	return 0;
}

/* remove the i'th character from the string s */
void remove_char(char *s, int i) 
{
	while (s[i]) 
	{
		i++;
		s[i-1] = s[i];
	}
	s[i] = 0;
}

/* remove non-alphabetic characters from the string s */
void remove_non_alpha(char *s) 
{
	int	i;

	for (i=0; s[i]; i++) 
		if (!is_alpha (s[i])) 
			remove_char (s, i);
}

int main(void)
{
	FILE *fp;
	char word[max_word];

	size_t i, j, len, index = 0, max_words = Max_words;

	words_t *words = (words_t*)calloc(max_words, sizeof *words);

	if(!words)
	{
		perror("calloc-words");
		exit(EXIT_FAILURE);
	}

	fp = fopen("complete-shakespeare.txt", "r");
	
	if(fp == NULL)
	{
		fputs("Unable to locate the file\n", stderr);
		exit(EXIT_FAILURE);
	}

	while(fscanf(fp, "%s", word) == 1)
	{
		int isunique = 1;

		if (!is_alpha(word[0])) 
		{

			/* get rid of non-letters */
			remove_non_alpha (word);
		}

			len = strlen(word);

			while(len && ispunct(word[len - 1]))
			{
				word[--len] = 0;
			}

			strlwr(word);

			for(i=0; i<index; i++)
			{
				if(strcmp(words[i].word, word) == 0)
				{
					isunique = 0;
					words[i].count++;
					break;
				}
			}

		if(isunique)
		{
			if(index == max_words)
			{
				void *tmp = realloc(words, 2 * max_words * sizeof *words); //reallocating memory space
				if(!tmp)
				{
					perror("realloc-words");
					break;
				}
				words = (words_t*)tmp;

				/*setting all the new memory locations to 0*/
				memset(words + max_words, 0, max_words * sizeof *words);

				/*letting the program know an increase in size*/
				max_words *= 2;
			}
			memcpy(words[index].word, word, len+1);
			words[index++].count++;
		}
	}

	fclose(fp);

	/*sorting in descending order of frequency||count*/

	int tmp, len_2 = 0;

	for(i=0; i<index; i++)
	{
		for(j=i+1; j<index; j++)
		{
			if(words[i].count < words[j].count)
			{
				tmp = words[i].count;
				words[i].count = words[j].count;
				words[j].count = tmp;
				
				len_2 = strlen(words[i].word);
				char tmp_word[len_2];

				strcpy(tmp_word, words[i].word);
				strcpy(words[i].word, words[j].word);
				strcpy(words[j].word, tmp_word); 
			}
		}
	}

	/*FILE *fp2;
	fp2 = fopen("FE19A086_asgn_1.txt", "w");*/   //opening a file in a write mode
	
	/*writing to the file while printing tothe output screen*/
	
	/*fprintf(fp2, "\n S/N \t\t WORD \t\t\t FREQUENCY \n");
    fprintf(fp2, "--------------------------------------------------\n");*/
	

	puts("\nS/N\t\tWORD\t\t\tFREQUENCY\n");

	for(i=0; i<index; i++)
	{
			strcpy(word, words[i].word);
			/**word = toupper(*word);*/

			printf("%lld\t\t%-25s%d\n", i+1, word, words[i].count);
			/*fprintf(fp2, "%lld\t\t%-25s%d\n", i+1, word, words[i].count);*/
	}
	
	/*fclose(fp2); */   /*closing the file*/  
	
	free(words);    /*free allocated spaces*/
	
	return 0;
}

