#include "TablesContainer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitTablesContainer(CString pi_sAssemblyFileName, TablesContainer* pio_pTablesContainer)
{
	strcpy(pio_pTablesContainer->sAssemblyFileName, pi_sAssemblyFileName);
	InitInstrucitonsTable(&(pio_pTablesContainer->instructionsTable));
	InitEntryTable(&(pio_pTablesContainer->entriesTable));
	InitExternalsTable(&(pio_pTablesContainer->externalsTable));
}

bool AddRowToInstructionsTable(TablesContainer* pio_pTablesContainer, InstructionRow pi_instructionRow, const bool pi_bIsValue)
{
	if(pio_pTablesContainer->instructionsTable.nSize == MEMORY_SIZE)
	{
		fprintf(stderr, "Your assembly program need more memory than %d.\n", MEMORY_SIZE);
		return false;
	}

	pio_pTablesContainer->instructionsTable.row[pio_pTablesContainer->instructionsTable.nSize] = pi_instructionRow;
	pio_pTablesContainer->instructionsTable.nSize++;

	if(pi_bIsValue)
	{
		pio_pTablesContainer->instructionsTable.nValueCounter++;
	}

	return true;
}

WORD GetSymbolAddress(TablesContainer* pi_pTablesContainer, CString pi_sSymbol)
{
	int i;

	if(IsEmpty(pi_sSymbol))
	{
		return 0;
	}

	for(i=0; i<pi_pTablesContainer->instructionsTable.nSize; i++)
	{
		if(IsEmpty(pi_pTablesContainer->instructionsTable.row[i].sLabel))
		{
			continue;
		}

		if(IsEqual(pi_sSymbol, pi_pTablesContainer->instructionsTable.row[i].sLabel))
		{
			return ADDRESS_BEGIN + i;
		}
	}

	/* If symbol not found */
	return 0;
}

void AddSymbolToEntryTable(TablesContainer* pio_pTablesContainer, CString pi_sSymbol)
{
	EntryRow* pEntryRow = pio_pTablesContainer->entriesTable.row + pio_pTablesContainer->entriesTable.nSize;
	
	strcpy(pEntryRow->sSymbol, pi_sSymbol);
	pEntryRow->nAddress = 0;

	pio_pTablesContainer->entriesTable.nSize++;
}

bool AddSymbolToExternalsTables(TablesContainer* pio_pTablesContainer, CString pi_sSymbol)
{
	/* Look if this external symbol was defined before */
	int i;
	ExternalRow externalRow;
	for(i=0; i<pio_pTablesContainer->externalsTable.nSize; i++)
	{
		if(IsEqual(pio_pTablesContainer->externalsTable.row[i].sSymbol, pi_sSymbol))
		{
			fprintf(stderr, "External symbol '%s' defined more than 1 time\n", pi_sSymbol);
			return false;
		}
	}

	/* Add external row to external table */
	InitExternalRow(&externalRow);

	strcpy(externalRow.sSymbol, pi_sSymbol);

	pio_pTablesContainer->externalsTable.row[pio_pTablesContainer->externalsTable.nSize] = externalRow;
	pio_pTablesContainer->externalsTable.nSize++;

	return true;
}

bool AddSymbolAndAddressToExternalsTables(TablesContainer* pio_pTablesContainer, CString pi_sSymbol, const WORD pi_nAddress)
{
	/* Look if this external symbol was defined before */
	int i;
	ExternalRow*  pExternalRow  = NULL;
	ExternalNode* pExternalNode = NULL;

	for(i=0; i<pio_pTablesContainer->externalsTable.nSize; i++)
	{
		if(IsEqual(pio_pTablesContainer->externalsTable.row[i].sSymbol, pi_sSymbol))
		{
			pExternalRow = pio_pTablesContainer->externalsTable.row + i;
			break;
		}
	}

	if(pExternalRow == NULL)
	{
		/* not found */
		return false;
	}

	pExternalNode = (ExternalNode *) malloc(sizeof(ExternalNode));

	if(pExternalNode == NULL)
	{
		fprintf(stderr, "Your computer is out of memory! Failed in function AddSymbolAndAddressToExternalsTables , %d\n", __LINE__);
		return false;
	}

	pExternalNode->nAddress = pi_nAddress;
	pExternalNode->pNext 	=  pExternalRow->pHeadListOfAddresses;

	pExternalRow->pHeadListOfAddresses = pExternalNode;

	return true;
}

bool IsSymbolInExternalsTable(TablesContainer* pi_pTablesContainer, CString pi_sSymbol)
{
	int i;
	for(i=0; i<pi_pTablesContainer->externalsTable.nSize; i++)
	{
		if(IsEqual(pi_pTablesContainer->externalsTable.row[i].sSymbol, pi_sSymbol))
		{
			return true;
		}
	}

	return false;
}

void CloseTableContainer(TablesContainer* pio_pTablesContainer)
{
	/* Free list from external table */
	int i;
	for(i=0; i<pio_pTablesContainer->externalsTable.nSize; i++)
	{
		ExternalNode* pNode = pio_pTablesContainer->externalsTable.row[i].pHeadListOfAddresses;
		while(pNode)
		{
			pio_pTablesContainer->externalsTable.row[i].pHeadListOfAddresses = pNode->pNext;
			free(pNode);

			pNode = pio_pTablesContainer->externalsTable.row[i].pHeadListOfAddresses;
		}
	}
}