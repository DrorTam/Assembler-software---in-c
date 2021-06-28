#include "Tables.h"
#include <string.h>

void InitInstructionRow(InstructionRow* pio_pInstructionRow)
{
	pio_pInstructionRow->eInstructionRowStatus = INSTRUCTION_ROW_STATUS_EMPTY;

	strcpy(pio_pInstructionRow->sLabel, 		"");
	strcpy(pio_pInstructionRow->sCommandName, 	"");
	strcpy(pio_pInstructionRow->sFirstOperand, 	"");
	strcpy(pio_pInstructionRow->sSecondOperand, "");
	strcpy(pio_pInstructionRow->sFirstParam, 	"");
	strcpy(pio_pInstructionRow->sSecondParam, 	"");
	strcpy(pio_pInstructionRow->sLookForSymbol,	"");

	pio_pInstructionRow->nValue = 0;
}

void InitInstrucitonsTable(InstructionsTable* pio_pInstructionsTable)
{
	pio_pInstructionsTable->nSize = 0;
	pio_pInstructionsTable->nValueCounter = 0;
}

void InitEntryRow(EntryRow* pio_pEntryRow)
{
	strcpy(pio_pEntryRow->sSymbol, "");
	pio_pEntryRow->nAddress = 0;
}

void InitEntryTable(EntriesTable* pio_pEntriesTable)
{
	pio_pEntriesTable->nSize = 0;
}

/*void InitExternalNode(ExternalNode* pio_pExternalNode)
{
	pio_pExternalNode->nAddress = 0;
	pio_pExternalNode->pNext = NULL;
}*/

void InitExternalRow(ExternalRow* pio_pExternalRow)
{
	strcpy(pio_pExternalRow->sSymbol, "");
	pio_pExternalRow->pHeadListOfAddresses = NULL;
}

void InitExternalsTable(ExternalsTable* pio_pExternalsTable)
{
	pio_pExternalsTable->nSize = 0;
}

/*void InitQueueCommand(QueueCommand* pio_pQueueCommand)
{
	pio_pQueueCommand->pBeginOfQueue 	= NULL;
	pio_pQueueCommand->pEndOfQueue 		= NULL;
}*/