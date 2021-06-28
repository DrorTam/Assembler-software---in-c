#include "RegisterManager.h"
#include <string.h>

void SetValueToRegister(WORD* pi_pRegister, const WORD pi_nValue)
{
	*pi_pRegister |= (pi_nValue << 2);
}

void SetTotalValueToRegister(WORD* pi_pRegister, const WORD pi_nValue)
{
	*pi_pRegister = pi_nValue;
}

void SetEncodeTypeToRegister(WORD* pi_pRegister, const EncodeType pi_eEncodeType)
{
	*pi_pRegister &= 0x3FFC;
	*pi_pRegister |= (WORD) pi_eEncodeType;
}

void SetOpcodeToRegister(WORD* pi_pRegister, const WORD pi_nValue)
{
	*pi_pRegister &= 0x3C3F;
	*pi_pRegister |= (pi_nValue << 6);
}

void SetFirstOperandAddressingModeToRegister(WORD* pi_pRegister, const AddressingMode pi_addressingMode)
{
	*pi_pRegister &= 0x3FCF;
	*pi_pRegister |= (((WORD) pi_addressingMode) << 4);
}

void SetSecondOperandAddressingModeToRegister(WORD* pi_pRegister, const AddressingMode pi_addressingMode)
{
	*pi_pRegister &= 0x3FF3;
	*pi_pRegister |= (((WORD) pi_addressingMode) << 2);
}

void SetFirstParamAddressingModeToRegister(WORD* pi_pRegister, const AddressingMode pi_addressingMode)
{
	*pi_pRegister &= 0x0FFF;
	*pi_pRegister |= (((WORD) pi_addressingMode) << 12);
}

void SetSecondParamAddressingModeToRegister(WORD* pi_pRegister, const AddressingMode pi_addressingMode)
{
	*pi_pRegister &= 0x33FF;
	*pi_pRegister |= (((WORD) pi_addressingMode) << 10);
}

void SetSrcRegisterNumberToRegister(WORD* pi_pRegister, const WORD pi_nRegisterNumber)
{
	*pi_pRegister &= 0x3C7F;
	*pi_pRegister |= (pi_nRegisterNumber << 8);
}

void SetDestRegisterNumberToRegister(WORD* pi_pRegister, const WORD pi_nRegisterNumber)
{
	*pi_pRegister &= 0x3FF3;
	*pi_pRegister |= (pi_nRegisterNumber << 2);
}

EncodeType GetRegisterEncodeType(WORD pi_nRegister)
{
	return (EncodeType) (pi_nRegister & 0x03);
}

bool IsValidRegisterName(CString pi_sRegisterName)
{
	return strlen(pi_sRegisterName) == 2 && pi_sRegisterName[0] == 'r' &&
			(pi_sRegisterName[1] - '0') >= 0 && (pi_sRegisterName[1] - '0') <= 7;
}

bool GetRegisterNumber(CString pi_sRegisterName, WORD* pi_pnRegisterNumberResult)
{
	*pi_pnRegisterNumberResult = 0xFFFF;
	if(!IsValidRegisterName(pi_sRegisterName))
	{
		return false;
	}

	*pi_pnRegisterNumberResult = pi_sRegisterName[1] - '0';
	return true;
}