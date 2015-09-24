#pragma once

/*
 * Memory is just an array of integers
 * The dictionary starts at the beginning and grows towards the hw->memory.
 * The hw->memory starts at the end and grows toward the dictionary.
 */

/*
 * A dictionary entry looks like this:
 * int next; // pointer to the previous entry (0 means end)
 * int flags; // definition to come
 * int (variable-len) name[]; // (counted string)
 * int (variable-len) definition[]; (last clause is always I_RETURN)
 */

// VM instructions
typedef enum {
	I_FETCH = 1,
	I_STORE,
	I_LITERAL,
	I_DUP,
	I_SWAP,
	I_DROP, 
	I_PICK,
	I_ROT,
	I_ONEPLUS,
	I_PLUS,
	I_MINUS,
	I_MULT,
	I_DIV,
	I_EQ,
	I_NEQ,
	I_LT,
	I_GT, 
	I_TO_R, // >R
	I_R_FROM, // R>
	I_R_AT, // R@
	I_IF_RT,
	I_DO,
	I_I,
	I_LEAVE,
	I_LOOP,
	I_PLUSLOOP,
	I_GOTO,
	I_DOT,
	I_TYPE,
	I_CALL, 
	I_OVER,
	I_EMIT,
	I_DICTP, 
	I_RETURN = 99,
} INSTR_T;
