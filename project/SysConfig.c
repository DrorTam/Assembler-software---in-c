#include "SysConfig.h"
#include <string.h>

bool IsEqual(CString pi_sStr1, CString pi_sStr2)
{
	return strcmp(pi_sStr1, pi_sStr2) == 0;
}

bool IsEmpty(CString pi_sStr)
{
	return IsEqual(pi_sStr, "");
}

void ConvertToWBase2(int pi_nDecimal, char po_sResultWBase2[REGISTER_STRING_SIZE])
{
	int i;
	WORD mask = 1;

	if(pi_nDecimal < 0)
	{
		int nType = pi_nDecimal & 0x3;
		pi_nDecimal >>= 2;
		pi_nDecimal *= -1;
		pi_nDecimal = ~pi_nDecimal;
		pi_nDecimal++;
		pi_nDecimal <<= 2;
		pi_nDecimal |= nType;
	}

	/*for(i=REGISTER_STRING_SIZE-2; i>=0; i--)*/
	for(i=0; i<REGISTER_STRING_SIZE - 1; i++)
	{
		po_sResultWBase2[REGISTER_STRING_SIZE - 2 - i] = (((pi_nDecimal & mask) == 0) ? '.' : '/');
		mask <<= 1;
	}
	po_sResultWBase2[REGISTER_STRING_SIZE-1] = '\0';
}