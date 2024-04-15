#include <stdio.h>
#include <stdlib.h>

#define TBLSIZE 23


// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType* next;
};

// Fill out this structure
struct HashType
{
    struct RecordType** node;
};

// Compute the hash function
int hash(int x)
{
    return x% TBLSIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType* pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
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
            pRecord->next = NULL; // Initialize next pointer
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
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        // if index is occupied with any records, print all
        if (pHashArray->node[i] != NULL)
        {
            printf("Index %d ", i);
            struct RecordType* current = pHashArray->node[i];
            while (current != NULL)
            {
                printf("-> %d, %c, %d ", current->id, current->name, current->order);
                current = current->next;
            }
            printf("\n");
        }
    }
}

// Get the length of the chain at the given index
int chainLength(struct RecordType* node)
{
    int length = 0;
    while (node != NULL)
    {
        length++;
        node = node->next;
    }
    return length;
}

// Insert a record into the hash table, ensuring that chains do not exceed MAX_CHAIN_LENGTH
void insertRecord(struct HashType* hashTable, struct RecordType* record)
{
    int index = hash(record->id);

    // Insert the record at the beginning of the chain
    record->next = hashTable->node[index];
    hashTable->node[index] = record;
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize hash table
    struct HashType hashTable;
    hashTable.node = (struct RecordType**)malloc(TBLSIZE * sizeof(struct RecordType*));
    for (int i = 0; i < TBLSIZE; ++i)
    {
        hashTable.node[i] = NULL;
    }

    // Populate hash table
    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(&hashTable, &pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(&hashTable, TBLSIZE);

    // Free memory allocated for hash table
    free(hashTable.node);

    // Free memory allocated for parsed records
    free(pRecords);

    return 0;
}

