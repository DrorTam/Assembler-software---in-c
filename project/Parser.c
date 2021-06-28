#include "Parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define BACKSPACE					' '
#define COMMA						','
#define COLON						':'
#define SEMICOLON					';'
#define OPEN_BARAKS					'('
#define CLOSE_BARAKS				')'
#define QUOT						'\"'

#define CMD_DATA 					".data"
#define CMD_STRING 					".string"

void InitAssemblyDataCode(AssemblyDataCode* pi_pAssemblyDataCode)
{
	/*char c = '\0';
	memcpy(pi_pAssemblyDataCode, &c, sizeof(AssemblyDataCode));*/

	strcpy(pi_pAssemblyDataCode->sLabel, "");
	strcpy(pi_pAssemblyDataCode->sCommandName, "");
	strcpy(pi_pAssemblyDataCode->sFirstOperand, "");
	strcpy(pi_pAssemblyDataCode->sSecondOperand, "");
	strcpy(pi_pAssemblyDataCode->sFirstParam, "");
	strcpy(pi_pAssemblyDataCode->sSecondParam, "");
}

bool ParseAssemblyLine(CString pi_sAssemblyCodeLine, AssemblyDataCode* po_pAssemblyDataCodeResult, bool* po_bIsEmptyLine)
{
	char buffer[READ_LINE_FROM_FILE_LEN] = "";
	LexicalAnalyzerList* pLexicalAnalyzerList = (LexicalAnalyzerList*) malloc(sizeof(LexicalAnalyzerList));
	
	LexicalAnalyzerNode* pNode = NULL;
	int nValue = 0;

	bool bSuccess = true;

	*po_bIsEmptyLine = false;

	if(pLexicalAnalyzerList == NULL)
	{
		fprintf(stderr, "Not enough memory to allocate LexicalAnalyzerList\n");
		exit(1);
	}

	InitAssemblyDataCode(po_pAssemblyDataCodeResult);
	InitLexicalAnalyzerList(pLexicalAnalyzerList);

	RepearLineFile(pi_sAssemblyCodeLine, buffer);

	GetLexicalListFromAssemblyCodeLine(buffer, pLexicalAnalyzerList);

	if(IsLexicalAnalyzerListEmpty(pLexicalAnalyzerList))
	{
		*po_bIsEmptyLine = true;
		return true;
	}


	pNode = pLexicalAnalyzerList->pHead;

	do
	{
		if(pNode->nType != NODE_TYPE_VALUE)
		{
			fprintf(stderr, "Assembler code can not begin with '%s'\n", pNode->sValue);
			bSuccess = false;
			break;
		}

		if(pNode->pNextLexicalAnalyzerNode != NULL &&
			pNode->pNextLexicalAnalyzerNode->sValue[0] == COLON)
		{
			if(strlen(pNode->sValue) > MAX_VARIABLE_LEN)
			{
				fprintf(stderr, "Invalid label. Label '%s' has more than %d chars.\n", pNode->sValue, (int) strlen(pNode->sValue));
				bSuccess = false;
				break;
			}

			strcpy(po_pAssemblyDataCodeResult->sLabel, pNode->sValue);
			pNode = pNode->pNextLexicalAnalyzerNode->pNextLexicalAnalyzerNode;
		}

		if(pNode == NULL)
		{
			fprintf(stderr, "Invalid line: %s\n", pi_sAssemblyCodeLine);
			bSuccess = false;
			break;
		}

		/* Get Command name */
		strcpy(po_pAssemblyDataCodeResult->sCommandName, pNode->sValue);
		pNode = pNode->pNextLexicalAnalyzerNode;

		/* Handle command: .data */
		if(IsEqual(po_pAssemblyDataCodeResult->sCommandName, CMD_DATA))
		{
			int i;
			bool bFoundNumberLastTime = false;

			char sDataValues[MAX_VARIABLE_LEN + 1] = "";

			if(pNode == NULL)
			{
				fprintf(stderr, "Error in assembly line code: '%s'. Must get value for data.\n", pi_sAssemblyCodeLine);
				bSuccess = false;
				break;
			}

			for(i=0; pNode != NULL; pNode = pNode->pNextLexicalAnalyzerNode, i++)
			{
				if(pNode->nType == NODE_TYPE_LOGIC && pNode->sValue[0] == BACKSPACE)
				{
					continue;
				}

				if(pNode->sValue[0] == ',' && pNode->nType == NODE_TYPE_LOGIC)
				{
					if(bFoundNumberLastTime == false)
					{
						fprintf(stderr, "Invalid line: %s\n", pi_sAssemblyCodeLine);
						bSuccess = false;
						break;
					}
					bFoundNumberLastTime = false;
				}
				else if(CheckValidStrNum(pNode->sValue, &nValue))
				{
					sprintf(sDataValues, "%s %s", sDataValues, pNode->sValue);
					bFoundNumberLastTime = true;
				}
				else
				{
					fprintf(stderr, ".data must contains valid numbers. The value '%s' is not a valid number.\n", pNode->sValue);
					bSuccess = false;
					break;
				}
			}

			if(bSuccess == false)
			{
				break;
			}

			Trim(sDataValues);
			strcpy(po_pAssemblyDataCodeResult->sFirstOperand, sDataValues);
		}
		/* Handle command: .string */
		else if(IsEqual(po_pAssemblyDataCodeResult->sCommandName, CMD_STRING))
		{
			if(pNode == NULL)
			{
				fprintf(stderr, "Error in assembly line code: '%s'. Must get value for string.\n", pi_sAssemblyCodeLine);
				bSuccess = false;
				break;
			}

			if(pNode->sValue[0] == QUOT && pNode->nType == NODE_TYPE_LOGIC)
			{
				pNode = pNode->pNextLexicalAnalyzerNode;
			}
			else
			{
				bSuccess = false;
				fprintf(stderr, "Invalid string definition '%s'\n", pi_sAssemblyCodeLine);
				break;
			}

			if(pNode == NULL)
			{
				fprintf(stderr, "Invaid string line '%s'\n", pi_sAssemblyCodeLine);
				bSuccess = false;
				break;
			}
			
			/* Hnadle empty string */
			if(pNode->nType == NODE_TYPE_LOGIC && pNode->sValue[0] == QUOT)
			{
				if(pNode->pNextLexicalAnalyzerNode == NULL)
				{
					strcpy(po_pAssemblyDataCodeResult->sFirstOperand, "");
					break;
				}
				else
				{
					fprintf(stderr, "Invaid string line '%s' due to: '%s'\n", pi_sAssemblyCodeLine, pNode->sValue);
					bSuccess = false;
					break;
				}
			}

			strcpy(po_pAssemblyDataCodeResult->sFirstOperand, pNode->sValue);
			pNode = pNode->pNextLexicalAnalyzerNode;

			if(pNode == NULL)
			{
				bSuccess = false;
				fprintf(stderr, "Invalid string definition '%s'\n", pi_sAssemblyCodeLine);
				break;
			}

			if(!(pNode->sValue[0] == QUOT && pNode->nType == NODE_TYPE_LOGIC && pNode->pNextLexicalAnalyzerNode == NULL))
			{
				bSuccess = false;
				fprintf(stderr, "Invalid string definition '%s'\n", pi_sAssemblyCodeLine);
				break;
			}
		}
		else
		{
			if(pNode == NULL)
			{
				/* There are some commands without no operands. */
				break;
			}

			if(pNode->nType == NODE_TYPE_LOGIC)
			{
				bSuccess = false;
				fprintf(stderr, "Error in code line '%s'\n", pi_sAssemblyCodeLine);
				break;
			}

			/* Copy first operand */
			strcpy(po_pAssemblyDataCodeResult->sFirstOperand, pNode->sValue);
			pNode = pNode->pNextLexicalAnalyzerNode;

			if(pNode == NULL)
			{
				/* There are some commands without 2 operands. */
				break;
			}

			if(pNode->nType != NODE_TYPE_LOGIC)
			{
				bSuccess = false;
				fprintf(stderr, "Error in code line '%s'. Between 2 operands must seperate ','. value is '%s'\n", pi_sAssemblyCodeLine, pNode->sValue);
				break;
			}
			
			if(pNode->sValue[0] == COMMA)
			{
				pNode = pNode->pNextLexicalAnalyzerNode;
				if(pNode == NULL)
				{
					bSuccess = false;
					fprintf(stderr, "Error in code line '%s'. Must get operand after ','.\n", pi_sAssemblyCodeLine);
					break;
				}

				/* Copy second operand */
				strcpy(po_pAssemblyDataCodeResult->sSecondOperand, pNode->sValue);

				if(pNode->pNextLexicalAnalyzerNode != NULL)
				{
					bSuccess = false;
					fprintf(stderr, "Error in code line '%s'. string after second operand is invalid.\n", pi_sAssemblyCodeLine);
					break;
				}
			}
			else
			{
				if(pNode->sValue[0] != OPEN_BARAKS)
				{
					bSuccess = false;
					fprintf(stderr, "Error in code line '%s'. Must starts with '('.\n", pi_sAssemblyCodeLine);
					break;
				}

				pNode = pNode->pNextLexicalAnalyzerNode;
				if(pNode->nType == NODE_TYPE_LOGIC)
				{
					bSuccess = false;
					fprintf(stderr, "Error in code line '%s'. Must get parameter after '('.\n", pi_sAssemblyCodeLine);
					break;
				}

				strcpy(po_pAssemblyDataCodeResult->sFirstParam, pNode->sValue);
				pNode = pNode->pNextLexicalAnalyzerNode;

				if(pNode == NULL)
				{
					bSuccess = false;
					fprintf(stderr, "Error in code line '%s'. Missing parameters.\n", pi_sAssemblyCodeLine);
					break;
				}

				if(!(pNode->nType == NODE_TYPE_LOGIC && pNode->sValue[0] == COMMA))
				{
					bSuccess = false;
					fprintf(stderr, "Error in code line '%s'. Must have ',' between parameters.\n", pi_sAssemblyCodeLine);
					break;
				}

				pNode = pNode->pNextLexicalAnalyzerNode;
				if(pNode == NULL)
				{
					bSuccess = false;
					fprintf(stderr, "Error in code line '%s'. Must get parameter after ','.\n", pi_sAssemblyCodeLine);
					break;
				}

				if(pNode->nType == NODE_TYPE_LOGIC)
				{
					bSuccess = false;
					fprintf(stderr, "Error in code line '%s'. Must get parameter after ','.\n", pi_sAssemblyCodeLine);
					break;
				}

				strcpy(po_pAssemblyDataCodeResult->sSecondParam, pNode->sValue);
				pNode = pNode->pNextLexicalAnalyzerNode;

				if(pNode == NULL)
				{
					bSuccess = false;
					fprintf(stderr, "Error in code line '%s'. Parameter must ends with ')'.\n", pi_sAssemblyCodeLine);
					break;
				}

				if(!(pNode->nType == NODE_TYPE_LOGIC && pNode->sValue[0] == CLOSE_BARAKS))
				{
					bSuccess = false;
					fprintf(stderr, "Error in code line '%s'. Parameter must ends with ')'.\n", pi_sAssemblyCodeLine);
					break;
				}

				if(pNode->pNextLexicalAnalyzerNode != NULL)
				{
					bSuccess = false;
					fprintf(stderr, "Error in code line '%s'. After ')' no other code should be.\n", pi_sAssemblyCodeLine);
					break;
				}
			}
		}

	} while(0);


	DeleteLexicalAnalyzerList(&pLexicalAnalyzerList);

	return bSuccess;
}

void GetLexicalListFromAssemblyCodeLine(CString pi_sAssemblyCodeLine, LexicalAnalyzerList* pio_pLexicalAnalyzerList)
{
	char tokens[] = {COMMA, OPEN_BARAKS, CLOSE_BARAKS, COLON, BACKSPACE};
	bool bInQuot = false;
	char str[BUFF_STR_NODE_SIZE];
	int nStrLength = 0;
	LexicalAnalyzerNode* pNode = NULL;
	LexicalAnalyzerNode* pPrev = NULL;
	int i;
	bool bLastCharWasBackspace = false;

	for(i=0; i<strlen(pi_sAssemblyCodeLine); i++)
	{
		if(bInQuot)
		{
			bLastCharWasBackspace = false;
			if(pi_sAssemblyCodeLine[i] == QUOT)
			{
				
				bInQuot = false;
				AddDataToList(pio_pLexicalAnalyzerList, str, NODE_TYPE_VALUE);
				str[0] = '\0';
				nStrLength = 0;

				AddDataToList(pio_pLexicalAnalyzerList, "\"", NODE_TYPE_LOGIC);
			}
			else
			{
				
				str[nStrLength] = pi_sAssemblyCodeLine[i];
				nStrLength++;
				str[nStrLength] = '\0';
			}
		}
		else if(pi_sAssemblyCodeLine[i] == QUOT)
		{
			bInQuot = true;
			bLastCharWasBackspace = false;
			AddDataToList(pio_pLexicalAnalyzerList, str, NODE_TYPE_LOGIC);
			AddDataToList(pio_pLexicalAnalyzerList, "\"", NODE_TYPE_LOGIC);

			str[0] = '\0';
			nStrLength = 0;
		}
		else if(pi_sAssemblyCodeLine[i] == SEMICOLON)
		{
			/* Copy str value to list */
			if(nStrLength > 0)
			{
				AddDataToList(pio_pLexicalAnalyzerList, str, NODE_TYPE_VALUE);
				str[0] = '\0';
				nStrLength = 0;
			}
			return ;
		}
		else
		{
			bool bTokenFound = false;
			int j;

			/* Don't copy multiple backspaces */
			if(bLastCharWasBackspace && pi_sAssemblyCodeLine[i] == BACKSPACE)
			{
				continue;
			}

			for(j=0; j<sizeof(tokens); j++)
			{
				if(pi_sAssemblyCodeLine[i] == tokens[j])
				{
					char buff[2];

					
					if(pi_sAssemblyCodeLine[i] == BACKSPACE)
					{
						bLastCharWasBackspace = true;
					}
					else
					{
						bLastCharWasBackspace = false;
					}

					/* Copy str value to list */
					if(nStrLength > 0)
					{
						AddDataToList(pio_pLexicalAnalyzerList, str, NODE_TYPE_VALUE);
						str[0] = '\0';
						nStrLength = 0;
					}

					buff[0] = pi_sAssemblyCodeLine[i];
					buff[1] = '\0';
					AddDataToList(pio_pLexicalAnalyzerList, buff, NODE_TYPE_LOGIC);

					bTokenFound = true;
					break;
				}
			}

			if(bTokenFound == false)
			{
				bLastCharWasBackspace = false;
				str[nStrLength] = pi_sAssemblyCodeLine[i];
				nStrLength++;
				str[nStrLength] = '\0';
			}
		}
	}

	if(nStrLength > 0)
	{
		AddDataToList(pio_pLexicalAnalyzerList, str, NODE_TYPE_VALUE);
	}

	if(!IsLexicalAnalyzerListEmpty(pio_pLexicalAnalyzerList))
	{
		/* Remove backspaces from list (except between label and colon) */
		pNode = pio_pLexicalAnalyzerList->pHead;
		pPrev = NULL;
		while(pNode)
		{
			if(pNode->nType == NODE_TYPE_LOGIC && pNode->sValue[0] == BACKSPACE)
			{
				if(pNode->pNextLexicalAnalyzerNode != NULL && 
					!(pNode->pNextLexicalAnalyzerNode->nType == NODE_TYPE_LOGIC && 
					pNode->pNextLexicalAnalyzerNode->sValue[0] == COLON))
				{
					if(pPrev != NULL)
					{
						LexicalAnalyzerNode* pTmpNode = pNode->pNextLexicalAnalyzerNode;
						free(pNode);
						pNode = pTmpNode;

						pPrev->pNextLexicalAnalyzerNode = pNode;

						continue;
					}
					else
					{
						/* shouldn't happen */
						printf("Error. Can't handle line when code began with backspace\n");
						return ;
					}
				}
				else
				{
					pPrev = pNode;
					continue;
				}
			}
			else
			{
				pPrev = pNode;
				pNode = pNode->pNextLexicalAnalyzerNode;
			}
		}
	}
}

/* Remove withspaces and tabs */
void RepearLineFile(CString pi_sAssemblyLine, String sResult)
{
	int i;
	int nCommaLocation = -1;
	int nNextCharOfResult = 0;
	int nLocation = -1;
	int bStartWrite = false;
	int bHandleString = 0;
	int nRemoveAt = -1;

	/* Remove 13 char or 10 ASCII char */
	for(i=0; i<strlen(pi_sAssemblyLine); i++)
	{
		if(pi_sAssemblyLine[i] == 10 ||pi_sAssemblyLine[i] == 13)
		{
			pi_sAssemblyLine[i] = 0;
			break;
		}
	}

	/* Remove space between comma. */
	for(i=0; i<strlen(pi_sAssemblyLine); i++)
	{
		if(pi_sAssemblyLine[i] == ',')
		{
			nCommaLocation = i;
			break;
		}
	}

	if(nCommaLocation == -1)
	{
		memcpy(sResult, pi_sAssemblyLine, strlen(pi_sAssemblyLine) + 1);
	}
	else
	{
		/* Remove backward spaces */
		for(i=nCommaLocation-1; i>=0; i--)
		{
			if(pi_sAssemblyLine[i] == ' ')
			{
				continue;
			}

			/* first char */
			nLocation = i;
			break;
		}

		if(nLocation != -1)
		{
			nNextCharOfResult = nLocation;
			memcpy(sResult, pi_sAssemblyLine, nLocation + 1);

			sResult[++nNextCharOfResult] = ',';
			sResult[++nNextCharOfResult] = '\0';
		}


		/* Remove forward spaces. */
		nLocation = -1;
		for(i=nCommaLocation+1; i<strlen(pi_sAssemblyLine)+1; i++)
		{
			if(!bStartWrite && pi_sAssemblyLine[i] == ' ')
			{
				continue;
			}

			bStartWrite = true;
			sResult[nNextCharOfResult++] = pi_sAssemblyLine[i];
		}
	}

	/* Replace tab with backspace */
	for(i=0; i<strlen(sResult); i++)
	{
		if(sResult[i] == '\t')
		{
			sResult[i] = ' ';
		}
	}

	/* Remove string after ; */
	for(i=0; i<strlen(sResult); i++)
	{
		if(bHandleString==0 && sResult[i] == ';')
		{
			nRemoveAt = i;
			break;
		}
		else if(sResult[i] == '\"')
		{
			bHandleString = !bHandleString;
		}
	}

	if(nRemoveAt != -1)
	{
		sResult[nRemoveAt] = '\0';
	}

	Trim(sResult);
}

void Trim(String pio_str)
{
        char sTmp[1000];
        int nStringLength = strlen(pio_str);
        int n = 0;
        int i;

        /* Left Trim */
        for(i=0; i<nStringLength; i++)
        {
                if(pio_str[i] != ' ')
                {
                        break;
                }
                ++n;
        }

        strcpy(sTmp, pio_str + n);
        strcpy(pio_str, sTmp);

        nStringLength = strlen(pio_str);
        n = 0;

        for(i=0; i<nStringLength; i++)
        {
                if(pio_str[nStringLength - 1 - i] != ' ')
                {
                        break;
                }
                ++n;
        }

        if(nStringLength - n > 0)
        {
                strncpy(sTmp, pio_str, nStringLength - n - 1);
                sTmp[nStringLength - n] = '\0';
                strcpy(pio_str, sTmp);
        }
}

bool CheckValidStrNum(String sStr, int* nNumResult)
{
	int i;
	int res=0;

	for(i=0; i<strlen(sStr); i++)
	{
		if(sStr[i] < '0' || sStr[i] > '9')
		{
			if(i != 0 || (i==0 && sStr[0] != '-' && sStr[0] !='+'))
			{
				return false;
			}
		}
	}

	res = atoi(sStr);

	/* Check range of number of it valid or not */
	if(res > 8191 || res < -8192)
	{
		fprintf(stderr, "Number %s is not in range number. a valid number is between (-8192) to 8191.\n",sStr);
		return false;
	}

	*nNumResult = res;

	return true;
}

bool CheckValidLabelName(const char* sLabelName)
{
	int i;
	char sRegName[3] = {'r', '0', '\0'};

	if(strstr(sLabelName, " ") != NULL || strstr(sLabelName, "\t") != NULL)
	{
		fprintf(stderr, "Internal error. Invalid label name '%s'. Label contains spaces/tabes.\n", sLabelName);
		return false;
	}

	if(strlen(sLabelName) == 0 || strlen(sLabelName) > 30)
	{
		return false;
	}

	/* check if label starts with digit */
	if(sLabelName[0] >= '0' && sLabelName[0] <= '9')
	{
		return false;
	}

	/* Check if label has register name */
	for(i=0; i<9; i++)
	{
		sRegName[1] = '0' + i;
		if(strcmp(sRegName, sLabelName) == 0)
		{
			return false;
		}
	}

	return true;
}

bool GetToken(char* sStr, const char cDelimiter, char* sResult, int* pio_pNextPosRun)
{
	int i=0,j=0;
	int bIsSomethingInserted = false;

	int nLastPas = *pio_pNextPosRun;

	if(strlen(sStr) - *pio_pNextPosRun <= 0)
	{
		return false;
	}

	for(i=*pio_pNextPosRun; i<strlen(sStr); i++)
	{
		if(!bIsSomethingInserted && sStr[i] == cDelimiter)
		{
			continue;
		}

		if(sStr[i] == cDelimiter)
		{
			*pio_pNextPosRun = i;
			break;
		}

		bIsSomethingInserted = true;
		sResult[j++] = sStr[i];
	}

	if(nLastPas == *pio_pNextPosRun)
	{
		*pio_pNextPosRun = strlen(sStr);
	}

	sResult[j] = '\0';
	return true;
}