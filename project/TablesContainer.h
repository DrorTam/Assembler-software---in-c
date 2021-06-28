#ifndef TABLES_CONTAINER_H
#define TABLES_CONTAINER_H

#include "Tables.h"

#define ASSEMBLY_FILE_NAME_LEN		100

typedef struct TablesContainer
{
	char 					sAssemblyFileName[ASSEMBLY_FILE_NAME_LEN];
	InstructionsTable 		instructionsTable;
	EntriesTable			entriesTable;
	ExternalsTable			externalsTable;
	/*QueueCommand			queueCommand;*/ /* Save here all variables to decode them later */
} TablesContainer;

void InitTablesContainer(CString pi_sAssemblyFileName, TablesContainer* pio_pTablesContainer);

bool AddRowToInstructionsTable(TablesContainer* pio_pTablesContainer, InstructionRow pi_instructionRow, const bool pi_bIsValue);

/* This function get TablesContainer and symbol.
	The function looks for this symbol (label) in the SymbolsTable (of TablesContainer)
	and returns the symbol's address.
	If address not found, the function return 0.
*/
WORD GetSymbolAddress(TablesContainer* pi_pTablesContainer, CString pi_symbol);

void AddSymbolToEntryTable(TablesContainer* pio_pTablesContainer, CString pi_sSymbol);

bool AddSymbolToExternalsTables(TablesContainer* pio_pTablesContainer, CString pi_sSymbol);

bool AddSymbolAndAddressToExternalsTables(TablesContainer* pio_pTablesContainer, CString pi_sSymbol, const WORD pi_nAddress);

bool IsSymbolInExternalsTable(TablesContainer* pi_pTablesContainer, CString pi_sSymbol);

void CloseTableContainer(TablesContainer* pio_pTablesContainer);

#endif