#ifndef PARSER_H
#define PARSER_H

#include "SysConfig.h"
#include "LexicalAnalyzerList.h"

typedef struct AssemblyDataCode
{
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

} AssemblyDataCode;

void InitAssemblyDataCode(AssemblyDataCode* pi_pAssemblyDataCode);

bool ParseAssemblyLine(CString pi_sAssemblyCodeLine, AssemblyDataCode* po_pAssemblyDataCodeResult, bool* po_bIsEmptyLine);

void GetLexicalListFromAssemblyCodeLine(CString pi_sAssemblyCodeLine, LexicalAnalyzerList* pio_pLexicalAnalyzerList);

/* Trim the string */
void Trim(String pio_str);

bool CheckValidStrNum(CString sStr, int* nNumResult);

bool CheckValidLabelName(const char* sLabelName);

void RepearLineFile(CString sAssemblyLine, String sResult);

/* 
Get next token of sStr.
sStr - Original string.
cDelimiter - Delimiter char.
sResult - The result (the token itself).
pio_nNextRunStartFromHere - Last char stopped (should be sent with 0 in first).
*/
bool GetToken(char* sStr, const char cDelimiter, char* sResult, int* pio_nNextRunStartFromHere);

#endif