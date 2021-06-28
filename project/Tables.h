#ifndef TABLES_H
#define TABLES_H

#include "SysConfig.h"

typedef char Symbol_t[MAX_VARIABLE_LEN];

typedef enum InstructionRowStatus
{
	INSTRUCTION_ROW_STATUS_EMPTY,
	INSTRUCTION_ROW_STATUS_COMPLETED
} InstructionRowStatus;

typedef struct InstructionRow
{
	/* Row Status */
	InstructionRowStatus eInstructionRowStatus;

	/* Label */
	char sLabel[MAX_VARIABLE_LEN + 1];

	/* Command Name */
	char sCommandName[MAX_COMMAND_NAME_LEN + 1];
	
	/* Opernads */
	char sFirstOperand[MAX_VARIABLE_LEN + 1];
	char sSecondOperand[MAX_VARIABLE_LEN + 1];

	/* Parameters */
	char sFirstParam[MAX_VARIABLE_LEN + 1];
	char sSecondParam[MAX_VARIABLE_LEN + 1];

	char sLookForSymbol[MAX_VARIABLE_LEN + 1];

	WORD nValue;

} InstructionRow;

typedef struct InstructionTable
{
	InstructionRow row[MEMORY_SIZE];
	int nSize;
	int nValueCounter;
} InstructionsTable;


/* Instruction Row Functions */
void InitInstructionRow(InstructionRow* pio_pInstructionRow);

/* Instruction Table Functions */
void InitInstrucitonsTable(InstructionsTable* pio_pInstructionsTable);


typedef struct EntryRow
{
	char sSymbol[MAX_VARIABLE_LEN + 1];
	WORD nAddress;
} EntryRow;

typedef struct EntriesTable
{
	EntryRow row[MEMORY_SIZE];
	int nSize;
} EntriesTable;


void InitEntryRow(EntryRow* pio_pEntryRow);

void InitEntryTable(EntriesTable* pio_pEntryTable);


typedef struct ExternalNode
{
	WORD nAddress;
	struct ExternalNode* pNext;
} ExternalNode;

typedef struct ExternalRow
{
	Symbol_t sSymbol;
	ExternalNode* pHeadListOfAddresses;
} ExternalRow;

typedef struct ExternalsTable
{
	ExternalRow row[MEMORY_SIZE];
	int nSize;
} ExternalsTable;

void InitExternalRow(ExternalRow* pio_pExternalRow);

void InitExternalsTable(ExternalsTable* pio_pExternalsTable);




/*typedef struct NodeCommand
{
	char sCommand[READ_LINE_FROM_FILE_LEN];
	struct NodeCommand* pPrev;
} NodeCommand;

typedef struct QueueCommand
{
	NodeCommand* pBeginOfQueue;
	NodeCommand* pEndOfQueue;
} QueueCommand;

void InitQueueCommand(QueueCommand* pio_pQueueCommand);*/

#endif