#include "Decoder.h"
#include "TablesContainer.h"
#include "FileOutput.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	/*ArrayOfCommandData arrCommandsData;
	GetArrayOfCommandData(arrCommandsData);

	printf("Command: %s , operadnds: %d\n", arrCommandsData[2].sName, arrCommandsData[2].nNumOfOperands);*/

	FILE* file = NULL;
	bool bSuccess = true;
	int i;
	size_t lenOfLine = 0;
	char sAssemblyFileName[ASSEMBLY_FILE_NAME_LEN] = "";
	char buffer[READ_LINE_FROM_FILE_LEN] = "";

	 if(argc <= 1)
    {
    	fprintf(stderr, "Please insert at least one file.\n");
    	exit(1);
    }

	for(i=1; i<argc; i++)
	{
		TablesContainer tablesContainer;
		bSuccess = true;

    	sprintf(sAssemblyFileName, "%s.as", argv[i]);
	    file = fopen(sAssemblyFileName, "r");

	    if(file == NULL)
	    {
	    	fprintf(stderr, "The file %s has not been found.\n", sAssemblyFileName);
	    	continue;
	    }

	    InitTablesContainer(argv[i], &tablesContainer);

	    fprintf(stdout, "\nCompilng %s.as\n==========================================================\n", argv[i]);
	   
	    /* Read lines of non values */
		while (fgets(buffer, READ_LINE_FROM_FILE_LEN, file))
		{
			lenOfLine = strlen(buffer);

			if(lenOfLine == 2 && (buffer[0] == 10 || buffer[0] == 13))
			{
				buffer[0] = '\0';
				lenOfLine = 0;
			}

			if(lenOfLine > 0 && buffer[lenOfLine - 1] == '\n')
			{
				buffer[lenOfLine - 1] = '\0';
			}

			/*Trim(buffer);
			printf("len=%d , after='%s'\n", (int)strlen(buffer), buffer);*/

			bSuccess &= DeocdeAssemblyCodeLine(buffer, &tablesContainer, DECODE_LINE_TYPE_NON_VALUES);
		}

		if(bSuccess)
		{
			fseek(file, 0, SEEK_SET);

			/* Read lines of values */
			while (fgets(buffer, READ_LINE_FROM_FILE_LEN, file))
			{
				lenOfLine = strlen(buffer);
				if(buffer[lenOfLine - 1] == '\n')
				{
					buffer[lenOfLine - 1] = '\0';
				}

				bSuccess &= DeocdeAssemblyCodeLine(buffer, &tablesContainer, DECODE_LINE_TYPE_VALUES);
			}

			fclose(file);
		}

		/* Do now Later Code*/

		bSuccess &= PostProcessing(&tablesContainer);

		if(!bSuccess)
		{
			fprintf(stderr, "Error found in your assembly code file %s.\n", sAssemblyFileName);
		}

		if(bSuccess)
		{
			fprintf(stdout, "%s.as compiled successfuly.\n", argv[i]);
			bSuccess &= FlushTablesContainer(&tablesContainer);
		}

		CloseTableContainer(&tablesContainer);
	}

	return 0;
}