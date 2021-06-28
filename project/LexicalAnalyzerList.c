#include "LexicalAnalyzerList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitLexicalAnalyzerNode(LexicalAnalyzerNode* pio_pLexicalAnalyzerNode)
{
	pio_pLexicalAnalyzerNode->pNextLexicalAnalyzerNode = NULL;
	pio_pLexicalAnalyzerNode->sValue[0] = '\0';
	pio_pLexicalAnalyzerNode->nType = NODE_TYPE_UNDEFINED;
}

void InitLexicalAnalyzerList(LexicalAnalyzerList* pio_pLexicalAnalyzerList)
{
	/* LexicalAnalyzerList is empty */
	/*while(pio_pLexicalAnalyzerList->pHead != NULL)
	{
		LexicalAnalyzerNode* pNode = pio_pLexicalAnalyzerList->pHead->pNextLexicalAnalyzerNode;
		free(pio_pLexicalAnalyzerList->pHead);
		pio_pLexicalAnalyzerList->pHead = pNode;
	}*/

	pio_pLexicalAnalyzerList->pHead = NULL;
	pio_pLexicalAnalyzerList->pTail = NULL;
	pio_pLexicalAnalyzerList->nSize = 0;
}

void AddToTail(LexicalAnalyzerList* pio_pLexicalAnalyzerList, LexicalAnalyzerNode* pio_pLexicalAnalyzerNode)
{
	/* LexicalAnalyzerList is empty */
	if(pio_pLexicalAnalyzerList->pTail == NULL)
	{
		pio_pLexicalAnalyzerList->pHead = pio_pLexicalAnalyzerNode;
		pio_pLexicalAnalyzerList->pTail = pio_pLexicalAnalyzerNode;
	}
	else
	{
		pio_pLexicalAnalyzerList->pTail->pNextLexicalAnalyzerNode = pio_pLexicalAnalyzerNode;
		pio_pLexicalAnalyzerList->pTail = pio_pLexicalAnalyzerList->pTail->pNextLexicalAnalyzerNode;
	}
	++pio_pLexicalAnalyzerList->nSize;
}

void DeleteLexicalAnalyzerList(LexicalAnalyzerList** pio_pLexicalAnalyzerList)
{
	LexicalAnalyzerNode* pCurrLexicalAnalyzerNode = (*pio_pLexicalAnalyzerList)->pHead;
	while(pCurrLexicalAnalyzerNode)
	{
		LexicalAnalyzerNode* pNodeToDelete = pCurrLexicalAnalyzerNode;
		pCurrLexicalAnalyzerNode = pCurrLexicalAnalyzerNode->pNextLexicalAnalyzerNode;

		free(pNodeToDelete);
	}

	free(*pio_pLexicalAnalyzerList);
	pio_pLexicalAnalyzerList = NULL;
}

void AddDataToList(LexicalAnalyzerList* pio_pList, CString pi_sValue, int pi_nType)
{
	if(strcmp(pi_sValue, "") != 0)
	{ 
		LexicalAnalyzerNode* pNode = (LexicalAnalyzerNode*) malloc(sizeof(LexicalAnalyzerNode));

		if(pNode == NULL)
		{
			fprintf(stderr, "Failed to allocate memory to LexicalAnalyzerNode.\n");
			exit(1);
		}

		InitLexicalAnalyzerNode(pNode);
		strcpy(pNode->sValue, pi_sValue);
		pNode->nType = pi_nType;

		AddToTail(pio_pList, pNode);
	}
}

int GetLexicalAnalyzerListSize(const LexicalAnalyzerList* pi_pLexicalAnalyzerList)
{
	return pi_pLexicalAnalyzerList->nSize;
}

bool IsLexicalAnalyzerListEmpty(const LexicalAnalyzerList* pi_pLexicalAnalyzerList)
{
	return GetLexicalAnalyzerListSize(pi_pLexicalAnalyzerList) == 0;
}