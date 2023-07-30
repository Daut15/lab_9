#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int id;
	char name;
	int order;
};
struct Node
{
	struct RecordType record;
	struct Node *next;
};
#define HASH_SIZE 15

// Fill out this structure
struct HashType
{
	struct Node *hashArray[HASH_SIZE];
};

// Compute the hash function
int hash(int x)
{
	return x % HASH_SIZE;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
	FILE *inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i = 0; i < hashSz; ++i)
	{
		printf("index %d -> ", i);
		struct Node *currentNode = pHashArray->hashArray[i];
		while (currentNode != NULL)
		{
			printf("%d %c %d -> ", currentNode->record.id, currentNode->record.name, currentNode->record.order);
			currentNode = currentNode->next;
		}
		printf("NULL\n");
		// if index is occupied with any records, print all
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	struct HashType hashTable;
	for (int i = 0; i < HASH_SIZE; ++i)
	{
		hashTable.hashArray[i] = NULL;
	}

	for (int i = 0; i < recordSz; ++i)
	{
		struct RecordType *pRecord = pRecords + i;
		int index = hash(pRecord->id);

		struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
		if (newNode == NULL)
		{
			printf("Memory allocation failed for a new node.\n");
			exit(-1);
		}

		newNode->record = *pRecord;
		newNode->next = NULL;

		if (hashTable.hashArray[index] == NULL)
		{
			hashTable.hashArray[index] = newNode;
		}
		else
		{
			newNode->next = hashTable.hashArray[index];
			hashTable.hashArray[index] = newNode;
		}
	}

	displayRecordsInHash(&hashTable, HASH_SIZE);

	for (int i = 0; i < HASH_SIZE; ++i)
	{
		struct Node *currentNode = hashTable.hashArray[i];
		while (currentNode != NULL)
		{
			struct Node *temp = currentNode;
			currentNode = currentNode->next;
			free(temp);
		}
	}

	free(pRecords);

	return 0;
}