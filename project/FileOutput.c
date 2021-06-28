#include "FileOutput.h"

void WriteAddressAndRegisterValue(FILE* pio_pFile, const WORD pi_nAddress, const WORD pi_nValue)
{
	char sBinaryMachineCode[REGISTER_STRING_SIZE] = "";
	
	ConvertToWBase2(pi_nValue, sBinaryMachineCode);
	fprintf(pio_pFile, "%d\t%s\n", pi_nAddress, sBinaryMachineCode);
}

bool FlushTablesContainer(TablesContainer* pi_pTablesContainer)
{
	int i;
	char sObjFileName[ASSEMBLY_FILE_NAME_LEN] 		= "";
	char sEntryFileName[ASSEMBLY_FILE_NAME_LEN] 	= "";
	char sExternalFileName[ASSEMBLY_FILE_NAME_LEN]	= "";

	FILE* pFile = NULL;

	sprintf(sObjFileName, 	"%s.ob", 		pi_pTablesContainer->sAssemblyFileName);
	sprintf(sEntryFileName, "%s.ent", 		pi_pTablesContainer->sAssemblyFileName);
	sprintf(sExternalFileName, "%s.ext", 	pi_pTablesContainer->sAssemblyFileName);

	/* Start writing Obj file */
	pFile = fopen(sObjFileName, "w");
	if(pFile == NULL)
	{
		fprintf(stderr, "Failed to create %s\n", sObjFileName);
		return false;
	}
	
	fprintf(pFile, "    %d %d\n", 
		(pi_pTablesContainer->instructionsTable.nSize - pi_pTablesContainer->instructionsTable.nValueCounter), 
		pi_pTablesContainer->instructionsTable.nValueCounter);

	for(i=0; i<pi_pTablesContainer->instructionsTable.nSize; i++)
	{
		InstructionRow* pInstructionRow = pi_pTablesContainer->instructionsTable.row + i;
		WriteAddressAndRegisterValue(pFile, ADDRESS_BEGIN + i, pInstructionRow->nValue);
	}

	fclose(pFile);

	fprintf(stdin, "%s generated.\n", sObjFileName);

	/* Start writing Entry file */
	if(pi_pTablesContainer->entriesTable.nSize > 0)
	{
		pFile = fopen(sEntryFileName, "w");
		if(pFile == NULL)
		{
			fprintf(stderr, "Failed to create %s\n", sEntryFileName);
			return false;
		}

		for(i=0; i<pi_pTablesContainer->entriesTable.nSize; i++)
		{
			EntryRow* pEntryRow = pi_pTablesContainer->entriesTable.row + i;
			fprintf(pFile, "%s\t%d\n", pEntryRow->sSymbol, pEntryRow->nAddress);
		}

		fclose(pFile);

		fprintf(stdin, "%s generated.\n", sEntryFileName);
	}


	/* Start writing External file */
	if(pi_pTablesContainer->externalsTable.nSize > 0)
	{
		pFile = fopen(sExternalFileName, "w");
		if(pFile == NULL)
		{
			fprintf(stderr, "Failed to create %s\n", sExternalFileName);
			return false;
		}
		
		for(i=0; i<pi_pTablesContainer->externalsTable.nSize; i++)
		{
			CString sSymbol = pi_pTablesContainer->externalsTable.row[i].sSymbol;
			ExternalNode* pNode = pi_pTablesContainer->externalsTable.row[i].pHeadListOfAddresses;

			while(pNode)
			{
				fprintf(pFile, "%s\t%d\n", sSymbol, pNode->nAddress);
				pNode = pNode->pNext;
			}
		}

		fclose(pFile);

		fprintf(stdin, "%s generated.\n", sExternalFileName);
	}


	return true;
}