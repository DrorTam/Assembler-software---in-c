#ifndef _LexicalAnalyzerList_H_
#define _LexicalAnalyzerList_H_

#include "SysConfig.h"

#define BUFF_STR_NODE_SIZE	40

#define NODE_TYPE_UNDEFINED	0
#define NODE_TYPE_LOGIC		1
#define NODE_TYPE_VALUE		2

typedef struct LexicalAnalyzerNode
{
	struct LexicalAnalyzerNode* pNextLexicalAnalyzerNode;
	char sValue[BUFF_STR_NODE_SIZE];
	int nType;
} LexicalAnalyzerNode;

typedef struct LexicalAnalyzerList
{
	LexicalAnalyzerNode* pHead;
	LexicalAnalyzerNode* pTail;
	int nSize;
} LexicalAnalyzerList;

void InitLexicalAnalyzerNode(LexicalAnalyzerNode* pLexicalAnalyzerNode);

void InitLexicalAnalyzerList(LexicalAnalyzerList* pList);

void AddToTail(LexicalAnalyzerList* pList, LexicalAnalyzerNode* pLexicalAnalyzerNode);

void DeleteLexicalAnalyzerList(LexicalAnalyzerList** pList);

void AddDataToList(LexicalAnalyzerList* pio_pList, CString pi_sValue, int pi_nType);

int GetLexicalAnalyzerListSize(const LexicalAnalyzerList* pi_pLexicalAnalyzerList);

bool IsLexicalAnalyzerListEmpty(const LexicalAnalyzerList* pi_pLexicalAnalyzerList);

#endif