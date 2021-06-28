#include "Decoder.h"
#include "RegisterManager.h"
#include <stdio.h>
#include <string.h>

bool DeocdeAssemblyCodeLine(CString pi_sAssemblyLine, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType)
{
	AssemblyDataCode assemblyDataCode;
	CommandData commandData;

	bool bSuccess = false;
	bool bIsEmptyLine = false;

	bSuccess = ParseAssemblyLine(pi_sAssemblyLine, &assemblyDataCode, &bIsEmptyLine);

	if(bIsEmptyLine)
	{
		return true;
	}

	if(bSuccess == false)
	{
		return false;
	}

	if(!GetCommandData(assemblyDataCode.sCommandName, &commandData))
	{
		/* Display command line errors only for non values (first iteration on file) */
		fprintf(stderr, "Unknown command '%s'.\n", assemblyDataCode.sCommandName);
		return false;

	}

	return (*commandData.pFunctionHandler)(assemblyDataCode, pio_pTablesContainer, pi_eDecodeLineType);
}

bool PostProcessing(TablesContainer* pio_pTablesContainer)
{
	int i;
	bool bSuccess = true;

	/* Handle Instructions Table */
	for(i=0; i<pio_pTablesContainer->instructionsTable.nSize; i++)
	{
		InstructionRow* pInstructionRow = pio_pTablesContainer->instructionsTable.row + i;
		if(pInstructionRow->eInstructionRowStatus == INSTRUCTION_ROW_STATUS_EMPTY)
		{
			WORD nAddress = GetSymbolAddress(pio_pTablesContainer, pInstructionRow->sLookForSymbol);

			if(nAddress != 0)
			{
				SetEncodeTypeToRegister(&pInstructionRow->nValue, ENCODE_TYPE_RELOCATABLE);
				SetValueToRegister(&pInstructionRow->nValue, nAddress);
			}
			else if(IsSymbolInExternalsTable(pio_pTablesContainer, pInstructionRow->sLookForSymbol)) 
			{
				SetEncodeTypeToRegister(&pInstructionRow->nValue, ENCODE_TYPE_EXTERNAL);
				AddSymbolAndAddressToExternalsTables(pio_pTablesContainer, pInstructionRow->sLookForSymbol, ADDRESS_BEGIN + i);
			}
			else
			{
				fprintf(stderr, "Undefined symbol '%s'.\n", pInstructionRow->sLookForSymbol);
				return false;
			}
		}
	}

	/* Handle Entries Table */
	for(i=0; i<pio_pTablesContainer->entriesTable.nSize; i++)
	{
		EntryRow* pEntryRow = pio_pTablesContainer->entriesTable.row + i;
		int nAddress = GetSymbolAddress(pio_pTablesContainer, pEntryRow->sSymbol);

		if(nAddress == 0)
		{
			fprintf(stderr, "Undefined symbol '%s'.\n", pEntryRow->sSymbol);
			bSuccess = false;
		}
		else
		{
			pEntryRow->nAddress = nAddress;
		}
	}

	return bSuccess;
}

void GetArrayOfCommandData(ArrayOfCommandData po_arrOfCommandData)
{
	/* Command: mov */
	{
		CommandData commandData = {
			COMMAND_MOV, 						/* Command Name */
			0,									/* Opcode */
			2,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_IMMEDIATE,
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		po_arrOfCommandData[0] = commandData;
	}
	
	/* Command: cmp */
	{
		CommandData commandData = {
			COMMAND_CMP, 						/* Command Name */
			1,									/* Opcode */
			2,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_IMMEDIATE,
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_IMMEDIATE,
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[1] = commandData;
	}

	/* Command: add */
	{
		CommandData commandData = {
			COMMAND_ADD, 						/* Command Name */
			2,									/* Opcode */
			2,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_IMMEDIATE,
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[2] = commandData;
	}

	/* Command: sub */
	{
		CommandData commandData = {
			COMMAND_SUB, 						/* Command Name */
			3,									/* Opcode */
			2,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_IMMEDIATE,
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_IMMEDIATE,
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[3] = commandData;
	}

	/* Command: not */
	{
		CommandData commandData = {
			COMMAND_NOT, 						/* Command Name */
			4,									/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[4] = commandData;
	}

	/* Command: clr */
	{
		CommandData commandData = {
			COMMAND_CLR, 						/* Command Name */
			5,									/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[5] = commandData;
	}

	/* Command: lea */
	{
		CommandData commandData = {
			COMMAND_LEA, 						/* Command Name */
			6,									/* Opcode */
			2,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[6] = commandData;
	}

	/* Command: inc */
	{
		CommandData commandData = {
			COMMAND_INC, 						/* Command Name */
			7,									/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[7] = commandData;
	}

	/* Command: dec */
	{
		CommandData commandData = {
			COMMAND_DEC, 						/* Command Name */
			8,									/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[8] = commandData;
	}

	/* Command: jmp */
	{
		CommandData commandData = {
			COMMAND_JMP, 						/* Command Name */
			9,									/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_PARAM_JMP,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[9] = commandData;
	}

	/* Command: bne */
	{
		CommandData commandData = {
			COMMAND_BNE, 						/* Command Name */
			10,									/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_PARAM_JMP,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[10] = commandData;
	}

	/* Command: red */
	{
		CommandData commandData = {
			COMMAND_RED, 						/* Command Name */
			11,									/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[11] = commandData;
	}

	/* Command: prn */
	{
		CommandData commandData = {
			COMMAND_PRN, 						/* Command Name */
			12,									/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_IMMEDIATE,
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[12] = commandData;
	}

	/* Command: jsr */
	{
		CommandData commandData = {
			COMMAND_JSR, 						/* Command Name */
			13,									/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_ADDRESS,
				ADDRESSING_MODE_PARAM_JMP,
				ADDRESSING_MODE_REGISTER,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[13] = commandData;
	}

	/* Command: rts */
	{
		CommandData commandData = {
			COMMAND_RTS, 						/* Command Name */
			14,									/* Opcode */
			0,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[14] = commandData;
	}

	/* Command: stop */
	{
		CommandData commandData = {
			COMMAND_STOP, 						/* Command Name */
			15,									/* Opcode */
			0,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleOpcodeCommand
		};
		
		po_arrOfCommandData[15] = commandData;
	}

	/* Command: .data */
	{
		CommandData commandData = {
			COMMAND_DATA, 						/* Command Name */
			0xFFFF,								/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleCommandData
		};
		
		po_arrOfCommandData[16] = commandData;
	}

	/* Command: .string */
	{
		CommandData commandData = {
			COMMAND_STRING, 						/* Command Name */
			0xFFFF,								/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleCommandString
		};
		
		po_arrOfCommandData[17] = commandData;
	}

	/* Command: .entry */
	{
		CommandData commandData = {
			COMMAND_ENTRY,						/* Command Name */
			0xFFFF,								/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleCommandEntry
		};
		
		po_arrOfCommandData[18] = commandData;
	}

	/* Command: .external */
	{
		CommandData commandData = {
			COMMAND_EXTERN,						/* Command Name */
			0xFFFF,								/* Opcode */
			1,									/* Number of operands */
			/* Valid Src Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			/* Valid Dest Addressing Mode */
			{
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID,
				ADDRESSING_MODE_INVALID
			},
			&HandleCommandExternal
		};
		
		po_arrOfCommandData[19] = commandData;
	}
}

bool GetCommandData(CString pi_sCommandName, CommandData* po_pCommandData)
{
	ArrayOfCommandData arrOfCommandData;
	bool bCommandFound = false;
	int i;

	GetArrayOfCommandData(arrOfCommandData);

	for(i=0; i<MAX_COMMANDS && !bCommandFound; i++)
	{
		if(IsEqual(pi_sCommandName, arrOfCommandData[i].sName))
		{
			*po_pCommandData = arrOfCommandData[i];
			bCommandFound = true;
		}
	}

	return bCommandFound;
}

bool ConvertRegisterNameToRegisterNumber(CString pi_sRegisterName, int* po_nRegisterNumber)
{
	if(strlen(pi_sRegisterName) == 2 && pi_sRegisterName[0] == 'r' && (pi_sRegisterName[1] >= '0' && pi_sRegisterName[1] <= '7'))
	{
		*po_nRegisterNumber = pi_sRegisterName[1] - '0';
		return true;
	}

	*po_nRegisterNumber = 0;
	return false;
}

/* 
Do not use this function to check if operand is Param-Jump addressing mode!!!
*/
AddressingMode GetAddressingMode(CString pi_sOperand)
{
	/* check if register */
	int nTmp = 0;
	if(ConvertRegisterNameToRegisterNumber(pi_sOperand, &nTmp))
	{
		return ADDRESSING_MODE_REGISTER;
	}


	/* check if Immediate */
	if(strlen(pi_sOperand) >= 2 && pi_sOperand[0] == '#')
	{
		int nNum = 0;
		if(CheckValidStrNum(pi_sOperand + 1, &nNum))
		{
			return ADDRESSING_MODE_IMMEDIATE;
		}
		else
		{
			return ADDRESSING_MODE_INVALID;
		}
	}

	/* check if label (address) */
	if(CheckValidLabelName(pi_sOperand))
	{
		return ADDRESSING_MODE_ADDRESS;
	}

	return ADDRESSING_MODE_INVALID;
}

bool IsAddressingModeValidToCommand(const CommandData pi_commandData, CString pi_sOperand, bool pi_bCompareToSourceAddressingMode)
{
	AddressingMode eFirstOperandAddressingMode = GetAddressingMode(pi_sOperand);
	int i=0;
	int nMaxAddressingModeOptions = ADDRESSING_MODE_INVALID;
	bool bFound = false;

	if(eFirstOperandAddressingMode == ADDRESSING_MODE_INVALID)
	{
		return false;
	}

	for(i=0; i<nMaxAddressingModeOptions; i++)
	{
		if(pi_bCompareToSourceAddressingMode)
		{
			if(eFirstOperandAddressingMode == pi_commandData.eSrcAddressingMode[i])
			{
				bFound = true;
				break;
			}
		}
		else
		{
			if(eFirstOperandAddressingMode == pi_commandData.eDestAddressingMode[i])
			{
				bFound = true;
				break;
			}
		}
	}

	return bFound;
}

void SetLookForSymbol(InstructionRow* po_pInstructionRow, CString pi_sOperandValue)
{
	strcpy(po_pInstructionRow->sLookForSymbol, pi_sOperandValue);
}

bool ManageInformativeLines(CString pi_sFirstOperand, 
							CString pi_sSecondOperand, 
							CString pi_sFirstParam, 
							CString pi_sSecondParam, 
							InstructionRow* po_arrInstructionRow, 
							int* pio_nArrInstructionRowSize)
{
	int nTmp = 0;
	if(!IsEmpty(pi_sFirstOperand) && !IsEmpty(pi_sSecondOperand))
	{
		/* if both are registers */
		if(GetAddressingMode(pi_sFirstOperand) == ADDRESSING_MODE_REGISTER && GetAddressingMode(pi_sSecondOperand) == ADDRESSING_MODE_REGISTER)
		{
			InstructionRow* pInstructionRow = po_arrInstructionRow + (*pio_nArrInstructionRowSize);
			pInstructionRow->eInstructionRowStatus = INSTRUCTION_ROW_STATUS_COMPLETED;

			SetEncodeTypeToRegister(&pInstructionRow->nValue, ENCODE_TYPE_ABSOLUTE);

			if(!ConvertRegisterNameToRegisterNumber(pi_sFirstOperand, &nTmp))
			{
				fprintf(stderr, "Internal error: ManageInformativeLines, %d", __LINE__);
				return false;
			}

			SetSrcRegisterNumberToRegister(&pInstructionRow->nValue, nTmp);

			if(!ConvertRegisterNameToRegisterNumber(pi_sSecondOperand, &nTmp))
			{
				fprintf(stderr, "Internal error: ManageInformativeLines, %d\n", __LINE__);
				return false;
			}


			SetDestRegisterNumberToRegister(&pInstructionRow->nValue, nTmp);

			(*pio_nArrInstructionRowSize)++;

			return true;
		}
		else
		{
			int i;
			for(i=0; i<2; i++)
			{
				InstructionRow* pInstructionRow = po_arrInstructionRow + (*pio_nArrInstructionRowSize);
				CString sOperandValue = (i == 0 ? pi_sFirstOperand : pi_sSecondOperand);

				switch(GetAddressingMode(sOperandValue))
				{
					case ADDRESSING_MODE_ADDRESS:
					{
						pInstructionRow->eInstructionRowStatus = INSTRUCTION_ROW_STATUS_EMPTY;
						SetLookForSymbol(pInstructionRow, sOperandValue);
					}
					break;

					case ADDRESSING_MODE_IMMEDIATE:
					{
						pInstructionRow->eInstructionRowStatus = INSTRUCTION_ROW_STATUS_COMPLETED;
						SetEncodeTypeToRegister(&pInstructionRow->nValue, ENCODE_TYPE_ABSOLUTE);
						
						if(!CheckValidStrNum(sOperandValue + 1, &nTmp))
						{
							fprintf(stderr, "Internal error: ManageInformativeLines, %d\n", __LINE__);
							return false;
						}

						SetValueToRegister(&pInstructionRow->nValue, nTmp);
					}
					break;

					case ADDRESSING_MODE_REGISTER:
					{
						pInstructionRow->eInstructionRowStatus = INSTRUCTION_ROW_STATUS_COMPLETED;
						SetEncodeTypeToRegister(&pInstructionRow->nValue, ENCODE_TYPE_ABSOLUTE);
						
						if(!ConvertRegisterNameToRegisterNumber(sOperandValue, &nTmp))
						{
							fprintf(stderr, "Internal error: ManageInformativeLines, %d\n", __LINE__);
							return false;
						}

						/* set as source register */
						if(i == 0)
						{
							SetSrcRegisterNumberToRegister(&pInstructionRow->nValue, nTmp);
						}
						else /* Set as dest register */
						{
							SetDestRegisterNumberToRegister(&pInstructionRow->nValue, nTmp);
						}
					}
					break;

					default:
					{
						fprintf(stderr, "Internal error: ManageInformativeLines, %d\n", __LINE__);
						return false;
					}
				}

				(*pio_nArrInstructionRowSize)++;
			}
		}
	}
	else if(!IsEmpty(pi_sFirstOperand) && IsEmpty(pi_sSecondOperand))
	{
		InstructionRow* pInstructionRow = po_arrInstructionRow + (*pio_nArrInstructionRowSize);

		switch(GetAddressingMode(pi_sFirstOperand))
		{
			case ADDRESSING_MODE_ADDRESS:
			{
				pInstructionRow->eInstructionRowStatus = INSTRUCTION_ROW_STATUS_EMPTY;
				SetLookForSymbol(pInstructionRow, pi_sFirstOperand);
			}
			break;

			case ADDRESSING_MODE_IMMEDIATE:
			{
				pInstructionRow->eInstructionRowStatus = INSTRUCTION_ROW_STATUS_COMPLETED;
				SetEncodeTypeToRegister(&pInstructionRow->nValue, ENCODE_TYPE_ABSOLUTE);
				
				if(!CheckValidStrNum(pi_sFirstOperand + 1, &nTmp))
				{
					fprintf(stderr, "Internal error: ManageInformativeLines, %d\n", __LINE__);
					return false;
				}

				SetValueToRegister(&pInstructionRow->nValue, nTmp);
			}
			break;

			case ADDRESSING_MODE_REGISTER:
			{
				pInstructionRow->eInstructionRowStatus = INSTRUCTION_ROW_STATUS_COMPLETED;
				SetEncodeTypeToRegister(&pInstructionRow->nValue, ENCODE_TYPE_ABSOLUTE);
				
				if(!ConvertRegisterNameToRegisterNumber(pi_sFirstOperand, &nTmp))
				{
					fprintf(stderr, "Internal error: ManageInformativeLines, %d\n", __LINE__);
					return false;
				}

				/* set as dest register */
				SetDestRegisterNumberToRegister(&pInstructionRow->nValue, nTmp);
			}
			break;

			default:
			{
				fprintf(stderr, "Internal error: ManageInformativeLines, %d\n", __LINE__);
				return false;
			}
		}
		(*pio_nArrInstructionRowSize)++;
	}

	if(!IsEmpty(pi_sFirstParam))
	{
		/* Call to function again. but this time send params as operands (because it is the same logic) */
		if(!ManageInformativeLines(pi_sFirstParam, pi_sSecondParam, "", "", po_arrInstructionRow, pio_nArrInstructionRowSize))
		{
			return false;
		}
	}

	return true;
}

bool HandleOpcodeCommand(AssemblyDataCode pi_assemblyDataCode, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType)
{
	CommandData commandData;
	int i=0;
	int nNumOfOperands = 0;
	bool bSuccess = true;
	InstructionRow commandInstructionRow;
	InstructionRow arrInstructionRow[3];
	int nArrInstructionRowSize = 0;

	if(pi_eDecodeLineType != DECODE_LINE_TYPE_NON_VALUES)
	{
		/* skip */
		return true;
	}

	for(i=0; i<3; i++)
	{
		InitInstructionRow(arrInstructionRow + i);
	}

	if(!IsEmpty(pi_assemblyDataCode.sLabel))
	{
		if(!CheckValidLabelName(pi_assemblyDataCode.sLabel))
		{
			fprintf(stderr, "Invalid label name '%s'\n", pi_assemblyDataCode.sLabel);
			return false;
		}

		if(GetSymbolAddress(pio_pTablesContainer, pi_assemblyDataCode.sLabel) != 0)
		{
			fprintf(stderr, "Symbol '%s' has defined more than 1 time.\n", pi_assemblyDataCode.sLabel);
			return false;
		}
	}

	/* Get Command Data */
	if(!GetCommandData(pi_assemblyDataCode.sCommandName, &commandData))
	{
		fprintf(stderr, "Undefined command '%s'\n", pi_assemblyDataCode.sCommandName);
		return false;
	}

	/* check number of operands */
	nNumOfOperands = IsEmpty(pi_assemblyDataCode.sFirstOperand) ? 0 : 1;
	nNumOfOperands += IsEmpty(pi_assemblyDataCode.sSecondOperand) ? 0 : 1;

	if(commandData.nNumOfOperands != nNumOfOperands)
	{
		fprintf(stderr, "Invalid number of operands for command %s. Number of operands for this command should be %d.\n", pi_assemblyDataCode.sCommandName, commandData.nNumOfOperands);
		return false;
	}

	/* Check addressing mode of first operand */
	if(nNumOfOperands == 1)
	{
		bool bFound = IsAddressingModeValidToCommand(commandData, pi_assemblyDataCode.sFirstOperand, false);

		if(!bFound)
		{
			fprintf(stderr, "Invalid addressing mode of first operand. Command: %s , First operand: '%s'.\n", commandData.sName, pi_assemblyDataCode.sFirstOperand);
			return false;
		}

		if(!IsEmpty(pi_assemblyDataCode.sFirstParam))
		{
			/* check if param-jmp addressing mode is valid*/
			if(commandData.eDestAddressingMode[0] != ADDRESSING_MODE_PARAM_JMP &&
				commandData.eDestAddressingMode[1] != ADDRESSING_MODE_PARAM_JMP &&
				commandData.eDestAddressingMode[2] != ADDRESSING_MODE_PARAM_JMP &&
				commandData.eDestAddressingMode[3] != ADDRESSING_MODE_PARAM_JMP)
			{
				fprintf(stderr, "Invalid addressing mode in command %s. Can't use parameters.\n", commandData.sName);
				return false;
			}
		}
	}
	else if(nNumOfOperands == 2)
	{
		bool bFound = IsAddressingModeValidToCommand(commandData, pi_assemblyDataCode.sFirstOperand, true);

		if(!bFound)
		{
			fprintf(stderr, "Invalid addressing mode of first operand. Command: %s , First operand: '%s'.\n", commandData.sName, pi_assemblyDataCode.sFirstOperand);
			return false;
		}

		bFound = IsAddressingModeValidToCommand(commandData, pi_assemblyDataCode.sSecondOperand, false);
		if(!bFound)
		{
			fprintf(stderr, "Invalid addressing mode of second operand. Command: %s , First operand: '%s'.\n", commandData.sName, pi_assemblyDataCode.sSecondOperand);
			return false;
		}

		if(!IsEmpty(pi_assemblyDataCode.sFirstParam))
		{
			fprintf(stderr, "Command '%s' cannot have params: (%s,%s).\n", commandData.sName, pi_assemblyDataCode.sFirstParam, pi_assemblyDataCode.sSecondParam);
			return false;
		}
	}


	/* Add instruction row */
	InitInstructionRow(&commandInstructionRow);
	commandInstructionRow.eInstructionRowStatus = INSTRUCTION_ROW_STATUS_COMPLETED; /*INSTRUCTION_ROW_STATUS_COMMAND;*/

	strcpy(commandInstructionRow.sCommandName, pi_assemblyDataCode.sCommandName);

	strcpy(commandInstructionRow.sLabel, pi_assemblyDataCode.sLabel); /* Copy label */
	SetEncodeTypeToRegister(&commandInstructionRow.nValue, ENCODE_TYPE_ABSOLUTE); /* Set encode type */

	SetOpcodeToRegister(&commandInstructionRow.nValue, commandData.nOpcode);

	do
	{
		if(IsEmpty(pi_assemblyDataCode.sFirstOperand))
		{
			break;
		}

		strcpy(commandInstructionRow.sFirstOperand, pi_assemblyDataCode.sFirstOperand);

		if(!IsEmpty(pi_assemblyDataCode.sSecondOperand))
		{
			/* Second operand exists which mean first operand is src */
			SetFirstOperandAddressingModeToRegister(&commandInstructionRow.nValue, GetAddressingMode(pi_assemblyDataCode.sFirstOperand));
		}
		else
		{
			/* No second operand which means first operand is dest */
			SetSecondOperandAddressingModeToRegister(&commandInstructionRow.nValue, GetAddressingMode(pi_assemblyDataCode.sFirstOperand));
		}

		if(IsEmpty(pi_assemblyDataCode.sSecondOperand) && IsEmpty(pi_assemblyDataCode.sFirstParam))
		{
			break;
		}

		/* Second operand exists */
		if(!IsEmpty(pi_assemblyDataCode.sSecondOperand))
		{
			strcpy(commandInstructionRow.sSecondOperand, pi_assemblyDataCode.sSecondOperand);
			SetSecondOperandAddressingModeToRegister(&commandInstructionRow.nValue, GetAddressingMode(pi_assemblyDataCode.sSecondOperand));
			break;
		}
		else
		{
			/* Handle params */
			SetSecondOperandAddressingModeToRegister(&commandInstructionRow.nValue, ADDRESSING_MODE_PARAM_JMP);

			strcpy(commandInstructionRow.sFirstParam, pi_assemblyDataCode.sFirstParam);
			SetFirstParamAddressingModeToRegister(&commandInstructionRow.nValue, GetAddressingMode(pi_assemblyDataCode.sFirstParam));

			strcpy(commandInstructionRow.sSecondParam, pi_assemblyDataCode.sSecondParam);
			SetSecondParamAddressingModeToRegister(&commandInstructionRow.nValue, GetAddressingMode(pi_assemblyDataCode.sSecondParam));
		}
	} while(0);

	/* Add Instruction line to InstructionTable */	
	bSuccess = AddRowToInstructionsTable(pio_pTablesContainer, commandInstructionRow, false);

	ManageInformativeLines(pi_assemblyDataCode.sFirstOperand, 
								pi_assemblyDataCode.sSecondOperand, 
								pi_assemblyDataCode.sFirstParam,
								pi_assemblyDataCode.sSecondParam,
								arrInstructionRow,
								&nArrInstructionRowSize);

	/* Add other rows */
	for(i=0; i<nArrInstructionRowSize && bSuccess; i++)
	{
		bSuccess = AddRowToInstructionsTable(pio_pTablesContainer, arrInstructionRow[i], false);
	}

	return bSuccess;
}

bool HandleCommandData(AssemblyDataCode pi_assemblyDataCode, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType)
{
	int nNextPos = 0;
	char sResult[MAX_VARIABLE_LEN + 1] = "";
	int nNumResult = 0;
	InstructionRow instructionRow;

	char sStr[MAX_VARIABLE_LEN + 1] = "";
	strcpy(sStr, pi_assemblyDataCode.sFirstOperand);

	if(pi_eDecodeLineType != DECODE_LINE_TYPE_VALUES)
	{
		/* skip */
		return true;
	}

	if(!IsEmpty(pi_assemblyDataCode.sLabel))
	{
		if(!CheckValidLabelName(pi_assemblyDataCode.sLabel))
		{
			fprintf(stderr, "Invalid label name '%s'\n", pi_assemblyDataCode.sLabel);
			return false;
		}

		if(GetSymbolAddress(pio_pTablesContainer, pi_assemblyDataCode.sLabel) != 0)
		{
			fprintf(stderr, "Symbol '%s' has defined more than 1 time.\n", pi_assemblyDataCode.sLabel);
			return false;
		}
	}

	InitInstructionRow(&instructionRow);
	strcpy(instructionRow.sLabel, pi_assemblyDataCode.sLabel);
	strcpy(instructionRow.sFirstOperand, pi_assemblyDataCode.sFirstOperand);
	strcpy(instructionRow.sCommandName, pi_assemblyDataCode.sCommandName);

	while(GetToken(sStr, ' ', sResult, &nNextPos))
	{
		instructionRow.eInstructionRowStatus = INSTRUCTION_ROW_STATUS_COMPLETED; /*INSTRUCTION_ROW_STATUS_DATA_COMMAND;*/

		CheckValidStrNum(sResult, &nNumResult);
		instructionRow.nValue = nNumResult;

		if(!AddRowToInstructionsTable(pio_pTablesContainer, instructionRow, true))
		{
			return false;
		}

		InitInstructionRow(&instructionRow);
	}

	return true;
}

bool HandleCommandString(AssemblyDataCode pi_assemblyDataCode, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType)
{
	int i;
	InstructionRow instructionRow;

	char sValue[MAX_VARIABLE_LEN + 1] = "";
	strcpy(sValue, pi_assemblyDataCode.sFirstOperand);

	if(pi_eDecodeLineType != DECODE_LINE_TYPE_NON_VALUES)
	{
		/* skip */
		return true;
	}

	if(!IsEmpty(pi_assemblyDataCode.sLabel))
	{
		if(!CheckValidLabelName(pi_assemblyDataCode.sLabel))
		{
			fprintf(stderr, "Invalid label name '%s'\n", pi_assemblyDataCode.sLabel);
			return false;
		}

		if(GetSymbolAddress(pio_pTablesContainer, pi_assemblyDataCode.sLabel) != 0)
		{
			fprintf(stderr, "Symbol '%s' has defined more than 1 time.\n", pi_assemblyDataCode.sLabel);
			return false;
		}
	}

	InitInstructionRow(&instructionRow);

	strcpy(instructionRow.sLabel, pi_assemblyDataCode.sLabel);
	strcpy(instructionRow.sCommandName, pi_assemblyDataCode.sCommandName);
	strcpy(instructionRow.sFirstOperand, pi_assemblyDataCode.sFirstOperand);
	instructionRow.eInstructionRowStatus = INSTRUCTION_ROW_STATUS_COMPLETED; /*INSTRUCTION_ROW_STATUS_STRING_COMMAND;*/

	for(i=0; i<strlen(pi_assemblyDataCode.sFirstOperand); i++)
	{
		instructionRow.eInstructionRowStatus = INSTRUCTION_ROW_STATUS_COMPLETED; /*INSTRUCTION_ROW_STATUS_STRING_CHAR_VALUE;*/
		SetTotalValueToRegister(&instructionRow.nValue, pi_assemblyDataCode.sFirstOperand[i]);
		AddRowToInstructionsTable(pio_pTablesContainer, instructionRow, true);

		InitInstructionRow(&instructionRow);
	}

	instructionRow.eInstructionRowStatus = INSTRUCTION_ROW_STATUS_COMPLETED; /*INSTRUCTION_ROW_STATUS_STRING_CHAR_VALUE;*/

	SetTotalValueToRegister(&instructionRow.nValue, 0);
	AddRowToInstructionsTable(pio_pTablesContainer, instructionRow, true);

	return true;
}

bool HandleCommandEntry(AssemblyDataCode pi_assemblyDataCode, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType)
{
	if(pi_eDecodeLineType != DECODE_LINE_TYPE_NON_VALUES)
	{
		/* skip */
		return true;
	}

	if(!IsEmpty(pi_assemblyDataCode.sLabel))
	{
		fprintf(stderr, "Warning: Ignoring assembly code line. Entry line has defined with label. %s: .entry ...\n", pi_assemblyDataCode.sLabel);
		return true;
	}

	if(IsEmpty(pi_assemblyDataCode.sFirstOperand))
	{
		fprintf(stderr, "Entry line defined without operand (symbol)");
		return false;
	}

	if(!IsEmpty(pi_assemblyDataCode.sSecondOperand))
	{
		fprintf(stderr, "Entry must get only one operand. Invalid second opernad '%s'.\n", pi_assemblyDataCode.sSecondOperand);
		return false;
	}

	if(!IsEmpty(pi_assemblyDataCode.sFirstParam))
	{
		fprintf(stderr, "Entry cannot get params. Invalid param '%s'.\n", pi_assemblyDataCode.sFirstOperand);
		return false;
	}

	if(!IsEmpty(pi_assemblyDataCode.sSecondParam))
	{
		fprintf(stderr, "Entry cannot get params. Invalid param '%s'.\n", pi_assemblyDataCode.sSecondOperand);
		return false;
	}

	if(GetAddressingMode(pi_assemblyDataCode.sFirstOperand) != ADDRESSING_MODE_ADDRESS)
	{
		fprintf(stderr, "Entry must get name of symbol. '%s' is not a symbol.\n", pi_assemblyDataCode.sFirstOperand);
		return false;
	}

	AddSymbolToEntryTable(pio_pTablesContainer, pi_assemblyDataCode.sFirstOperand);

	return true;
}

bool HandleCommandExternal(AssemblyDataCode pi_assemblyDataCode, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType)
{
	if(pi_eDecodeLineType != DECODE_LINE_TYPE_NON_VALUES)
	{
		/* skip */
		return true;
	}

	if(!IsEmpty(pi_assemblyDataCode.sLabel))
	{
		fprintf(stderr, "Warning: Ignoring assembly code line. External line has defined with label. %s: .ext ...\n", pi_assemblyDataCode.sLabel);
		return true;
	}

	if(IsEmpty(pi_assemblyDataCode.sFirstOperand))
	{
		fprintf(stderr, "External line defined without operand (symbol)");
		return false;
	}

	if(!IsEmpty(pi_assemblyDataCode.sSecondOperand))
	{
		fprintf(stderr, "External must get only one operand. Invalid second opernad '%s'.\n", pi_assemblyDataCode.sSecondOperand);
		return false;
	}

	if(!IsEmpty(pi_assemblyDataCode.sFirstParam))
	{
		fprintf(stderr, "External cannot get params. Invalid param '%s'.\n", pi_assemblyDataCode.sFirstOperand);
		return false;
	}

	if(!IsEmpty(pi_assemblyDataCode.sSecondParam))
	{
		fprintf(stderr, "External cannot get params. Invalid param '%s'.\n", pi_assemblyDataCode.sSecondOperand);
		return false;
	}

	if(GetAddressingMode(pi_assemblyDataCode.sFirstOperand) != ADDRESSING_MODE_ADDRESS)
	{
		fprintf(stderr, "External must get name of symbol. '%s' is not a symbol.\n", pi_assemblyDataCode.sFirstOperand);
		return false;
	}

	return AddSymbolToExternalsTables(pio_pTablesContainer, pi_assemblyDataCode.sFirstOperand);
}