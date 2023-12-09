// Header that defines registers, functions and constants for a MIPS processor
//mips_helper.h
#ifndef MIPS_HELPER_H
#define MIPS_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //strcpy()

struct instruction {
	int instructionAddress;
	char instructionLine[33];
};
typedef struct instruction instruction;

struct registers {
	char RegisterNumber[6]; // identficador de cada um dos 32 registro
	char registerData[33];  // 32 bits de dados contidos em cada registrador
};
typedef struct registers registers;

struct data {
	int dataAddress;   //"linhas" ou endereco dos nossos dados da memoria 
	char DataLine[33]; // dados contidos em uma "linha" ou endereco
};
typedef struct data data;

// extern struct instruction instructionMemory[16];
// extern struct data dataMemory[5];
// extern struct registers registerFile[32];
extern instruction instructionMemory[16];
extern data dataMemory[5];
extern registers registerFile[32];


//Global Variables
extern char inputLine[33];

extern char opcodeBinary[7];        // G Var for Binary
extern char rsBinary[6];
extern char rtBinary[6];
extern char rdBinary[6];
extern char shamtBinary[6];
extern char functBinary[7];

extern char immediateBinary[17];   // exclusive type I
extern char addressBinary[27]; 	// exclusive type J

// buffers de saida
extern char signExtend[33];
extern char reg1Out[33];   // saidas dos banco de registradores
extern char reg2Out[33];
extern char aluOut[33];    // saida da ALU principal
extern char memOut[33];	// saida da memoria


// sinais de controle
extern char regWrite;
extern char regDst;
extern char AluSrc;
extern char AluOp;
extern char AluZero;
extern char memRead;
extern char memWrite;
extern char memToReg;
extern char branch;

extern char instructionAssembly[16];     // G Var for Assembly
extern int PC;         // Program Counter
extern int cycle;

// fim da declaracao de Global Variables

//Core Instruction Set (tipo + 6bits)
/*
// R Format Instructions - 12 types (Tipo + 6 bits Funct) (opcode fixo 000000)
add = R100000 	// 0/20 *
addu = R100001 	// 0/21
and = R100100 	// 0/24
jr = R001000	// 0/08
nor = R100111 	// 0/27
or  = R100101 	// 0/25
slt = R101010 	// 0/2a
sltu = R101011	// 0/2b
sll  = R000000	// 0/00
srl  = R000010	// 0/02
sub = R100010 	// 0/22
subu = R100011  // 0/23

// I Format Instructions - 17 types (Tipo + 6 bits opcode)
addi = I001000 		// 8
addiu = I001001		// 9
andi = I001100		// c
beq = I000100 		// 4
bne = I000101		// 5
lbu = I00100100 	// 24 
lhu = I100101 		// 25
ll = I110000		// 30
lui = I001111		// f
lw = I100011 		// 23
ori = I001101 		// d
slti = I001010 		// a
sltiu = I001011 	// b  
sb = I101000		// 28
sc = I111000		// 38
sh = I101001		// 29
sw = I101011		// 2b

// J Format Instructions - 2 types (tipo + 6 bits opcode) 
j = J000010 		// 2
jal = J000011 		// 3

*/

void reverse(char str[], int length);
char* itoa(int num, char* str, int base);
int binary32ToChar(char *charInput, char *charOutput);
int xor(int a, int b);
void not(char *string);
void padZero(char *string, int length);
int isNumeric(char ch);
int isBinary(char ch);
void charTo5BitsU(char *charInput, char *charOutput);
void charTo16BitsU(char *charInput, char *charOutput);
void charTo16Bits(char *charInput, char *charOutput);
void charTo26BitsU(char *charInput, char *charOutput);
void charTo32Bits(char *charInput, char *charOutput);

void getOpcodeBinary(char *inputLine);
void ripDataBinary(char *opcodeBinary);
void registerToAssembly(char *registerBinary, char *registerAssembly);
void instructionR();
void instructionI();
void instructionJ();

void addToAssembly();
void adduToAssembly();
void andToAssembly();
void jrToAssembly();
void norToAssembly();
void orToAssembly();
void sltToAssembly();
void sltuToAssembly();
void sllToAssembly();
void srlToAssembly();
void subToAssembly();
void subuToAssembly();
void addiToAssembly();
void andiToAssembly();
void beqToAssembly();
void bneToAssembly();
void lbuToAssembly();
void lhuToAssembly();
void llToAssembly();
void luiToAssembly();
void lwToAssembly();
void oriToAssembly();
void sltiToAssembly();
void sltiuToAssembly();
void sbToAssembly();
void scToAssembly();
void shToAssembly();
void swToAssembly();
void jToAssembly();
void jalToAssembly();

#endif  // MIPS_HELPER_H
