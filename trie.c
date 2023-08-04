/*
    Maksim Petrushin
    UCF - COP3502
    08/04/2023
    Trie Implementation (iterrative except the deallocate function)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
    int count;//0 is none, otherwise 1,2,3,4,......
    struct Trie* next[26];
};

// Initializes a trie structure
struct Trie *createTrie(void)
{
     // Create the Trie structure
    struct Trie* myTree = malloc(sizeof(struct Trie));
    myTree->count = 0;

    // Set each pointer to NULL.
    int i;
    for (i=0; i<26; i++)
        myTree->next[i] = NULL;

    // Return a pointer to the newly created root/head.
    return myTree;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{   
    //itterative version... loop troguh
    int i=0, len = strlen(word);
    for(i=0; i<len; i++){
        //get the using ASCII of the letter minus ASCII of the a to get its position in aplphabet.
        int nextIndex = word[i] - 'a';
        //if NULL, this letter is not in trie... create it then.
        if (pTrie->next[nextIndex] == NULL){
            pTrie->next[nextIndex] = createTrie();
        }

        //loop to next pointer of trie
        pTrie=pTrie->next[nextIndex];
    }
    //add to count of this word (last character..)
    pTrie->count+=1;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    int i=0, len = strlen(word);
    //iterate trough trie whether it has these specific letters
    for(i=0; i<len; i++){
        int nextIndex = word[i] - 'a';
        //if null meaning this letter is not in the trie in this possition, just return 0
        if (pTrie->next[nextIndex] == NULL){
            return 0;
        }
        pTrie=pTrie->next[nextIndex];
    }
    //return the count of this word otherwise
    return pTrie->count;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    int i;
    //go trough all possible letters in this position
    for (i=0; i<26; i++)
        //if not null, meaning it exists, delete it (by calling a deallocateTrie function on it)
        if (pTrie->next[i] != NULL)
            pTrie->next[i]=deallocateTrie(pTrie->next[i]);
    //in the end dealocate this root structure (the pointer to the very first letter position)
    free(pTrie);
    return NULL;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    
    int i, n;
    FILE* ifp = fopen(filename, "r");
    // Read in number of words.
    fscanf(ifp, "%d", &n);
    // Allocate memory in the string array
    for(i=0;i<n;i++){
        pInWords[i]=malloc(sizeof(char)*256);
    }

    // Read in each word and insert it.
    for (i=0; i<n; i++) {
        char word[256];
        fscanf(ifp, "%s", word);
        strcpy(pInWords[i],word);
    }
    //close file pointer
    fclose(ifp);
    return n;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
    printf("%d\n", numWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}