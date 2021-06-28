#ifndef DECODER_H
#define DECODER_H

#include "SysConfig.h"
#include "Parser.h"
#include "TablesContainer.h"

#define MAX_COMMANDS	20

/* Commadns List */
#define COMMAND_MOV					"mov"
#define COMMAND_CMP					"cmp"
#define COMMAND_ADD					"add"
#define COMMAND_SUB					"sub"
#define COMMAND_LEA					"lea"
#define COMMAND_NOT					"not"
#define COMMAND_CLR					"clr"
#define COMMAND_INC					"inc"
#define COMMAND_DEC					"dec"
#define COMMAND_JMP					"jmp"
#define COMMAND_BNE					"bne"
#define COMMAND_RED					"red"
#define COMMAND_PRN					"prn"
#define COMMAND_JSR					"jsr"
#define COMMAND_RTS					"rts"
#define COMMAND_STOP				"stop"

/* commands without opcode */
#define COMMAND_DATA				".data"
#define COMMAND_STRING				".string"

#define COMMAND_ENTRY				".entry"
#define COMMAND_EXTERN				".extern"

typedef enum eDecodeLineType
{
	DECODE_LINE_TYPE_NON_VALUES, 	/* instructions, entry, external */
	DECODE_LINE_TYPE_VALUES 		/* data, string */

} eDecodeLineType;

/* Define pointer to handler function */
typedef bool (*HANDLE_COMMAND_PTR_FUNC) (AssemblyDataCode pi_assemblyDataCode, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType);

/* CommandData defines all command data information */
typedef struct CommandData
{
	char 						sName[10]; 					/* Command name */
	WORD 						nOpcode;					/* Opcode */
	int 						nNumOfOperands;				/* Number of operands */
	AddressingMode 				eSrcAddressingMode[4];		/* Valid source addressing mode */
	AddressingMode 				eDestAddressingMode[4];		/* Valid destination addressing mode */
	HANDLE_COMMAND_PTR_FUNC		pFunctionHandler;			/* Handler pointer function */
} CommandData;

/* Array of commands data */
typedef CommandData ArrayOfCommandData[MAX_COMMANDS];

/* Decode assembly code line. return true if success, else false. */
bool DeocdeAssemblyCodeLine(CString pi_sAssemblyLine, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType);

/* This function complete the instruction table with all symbol addresses */
bool PostProcessing(TablesContainer* pio_pTablesContainer);

void GetArrayOfCommandData(ArrayOfCommandData po_arrOfCommandData);

bool GetCommandData(CString pi_sCommandName, CommandData* po_pCommandData);

bool ConvertRegisterNameToRegisterNumber(CString pi_sRegisterName, int* po_nRegisterNumber);

AddressingMode GetAddressingMode(CString pi_sOperand);

/* Handle Functions */

bool HandleOpcodeCommand(AssemblyDataCode pi_assemblyDataCode, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType);

bool HandleCommandData(AssemblyDataCode pi_assemblyDataCode, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType);

bool HandleCommandString(AssemblyDataCode pi_assemblyDataCode, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType);

bool HandleCommandEntry(AssemblyDataCode pi_assemblyDataCode, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType);

bool HandleCommandExternal(AssemblyDataCode pi_assemblyDataCode, TablesContainer* pio_pTablesContainer, const eDecodeLineType pi_eDecodeLineType);


#endif