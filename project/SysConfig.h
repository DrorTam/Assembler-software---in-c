#ifndef _SYSCONFIG_H_
#define _SYSCONFIG_H_

#include <inttypes.h>

#define DEBUG						1

#define ADDRESS_BEGIN				100
#define NUM_OF_REGISTERS			8
#define REGISTER_SIZE				14
#define REGISTER_STRING_SIZE		REGISTER_SIZE + 1
#define MEMORY_SIZE					256
#define MAX_VARIABLE_LEN			31
#define ARR_DATA_VALUES_LEN			100
#define MAX_COMMAND_NAME_LEN		8
#define READ_LINE_FROM_FILE_LEN		1000

#define false						0
#define true						1

typedef uint8_t bool;

typedef uint16_t WORD;

typedef char* String;
typedef const String CString;

typedef enum AddressingMode
{
	ADDRESSING_MODE_IMMEDIATE		= 0,
	ADDRESSING_MODE_ADDRESS			= 1,
	ADDRESSING_MODE_PARAM_JMP		= 2,
	ADDRESSING_MODE_REGISTER		= 3,

	ADDRESSING_MODE_INVALID
} AddressingMode;

typedef enum EncodeType
{
	ENCODE_TYPE_ABSOLUTE			= 0,
	ENCODE_TYPE_EXTERNAL			= 1,
	ENCODE_TYPE_RELOCATABLE			= 2,

	ENCODE_TYPE_INVALID
} EncodeType;

bool IsEqual(CString pi_sStr1, CString pi_sStr2);

bool IsEmpty(CString pi_sStr);

void ConvertToWBase2(int nDecimal, char sResultWBase2[REGISTER_SIZE]);

#endif