#ifndef REGISTER_MANAGER_H_
#define REGISTER_MANAGER_H_

#include "SysConfig.h"

/* Setting address into register */
void SetValueToRegister(WORD* pi_pRegister, const WORD pi_nValue);

/* Setting value to register */
void SetTotalValueToRegister(WORD* pi_pRegister, const WORD pi_nValue);

void SetEncodeTypeToRegister(WORD* pi_pRegister, const EncodeType pi_eEncodeType);

void SetOpcodeToRegister(WORD* pi_pRegister, const WORD pi_nValue);

void SetFirstOperandAddressingModeToRegister(WORD* pi_pRegister, const AddressingMode pi_addressingMode);

void SetSecondOperandAddressingModeToRegister(WORD* pi_pRegister, const AddressingMode pi_addressingMode);

void SetFirstParamAddressingModeToRegister(WORD* pi_pRegister, const AddressingMode pi_addressingMode);

void SetSecondParamAddressingModeToRegister(WORD* pi_pRegister, const AddressingMode pi_addressingMode);

void SetSrcRegisterNumberToRegister(WORD* pi_pRegister, const WORD pi_nRegisterNumber);

void SetDestRegisterNumberToRegister(WORD* pi_pRegister, const WORD pi_nRegisterNumber);


/* Get encode-type of register */
EncodeType GetRegisterEncodeType(WORD pi_nRegister);

/* Utils */

/* Check if register is valid or not. */
bool IsValidRegisterName(CString pi_sRegisterName);

/*
This function get register name.
the function return the register number into pi_pnRegisterNumberResult.
If register name is not valid the function return false.
*/
bool GetRegisterNumber(CString pi_sRegisterName, WORD* pi_pnRegisterNumberResult);

#endif