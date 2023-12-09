/*
Esta eh a segunda parte do trabalho parte da discplina Arquitetura 
de Computadores
Este eh um programa que simula um processador MIPS em C
Executaremos algumas instrucoes predefinidas

autores:
Dalton Lima - sacinopatinete@gmail.com
Stefany - stefanylacroux@hotmail.com
Victor - victor_ribeiro.pira@hotmail.com
Tadeu - tadeumartines@yahoo.com.br
Lucas Pinheiro - pinheiro.lucasaugusto@gmail.com

*/

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>  //strcpy()
#include "mips_helper.h"
#include "mips_reliability.h"
#include "reliability_logging.h"

int binaryStringToInt(char* bin){
	return strtol(bin, NULL, 2);
}

static const int INSTRUCTION_LENGTH = 16;

//Global Variables
char inputLine[33];
char opcodeBinary[7];        
char rsBinary[6];
char rtBinary[6];
char rdBinary[6];
char shamtBinary[6];
char functBinary[7];
char immediateBinary[17];   // exclusive type I
char addressBinary[27]; 	// exclusive type J

// buffers de saida
char signExtend[33];
char reg1Out[33];   // saidas dos banco de registradores
char reg2Out[33];
char aluOut[33];    // saida da ALU principal
char memOut[33];	// saida da memoria

// sinais de controle
char regWrite = '0';
char regDst = '0';
char AluSrc = '0';
char AluOp = '0';
char AluZero = '0';
char memRead = '0';
char memWrite = '0';
char memToReg = '0';
char branch = '0';

char instructionAssembly[16];     // G Var for Assembly
int PC = 0;         			// Program Counter
int cycle = 0;

// struct instruction {
// 	int instructionAddress;
// 	char instructionLine[33];
// };
// typedef struct instruction instruction;

// struct registers {
// 	char RegisterNumber[6]; // identficador de cada um dos 32 registro
// 	char registerData[33];  // 32 bits de dados contidos em cada registrador
// };
// typedef struct registers registers;

// struct data {
// 	int dataAddress;   //"linhas" ou endereco dos nossos dados da memoria 
// 	char DataLine[33]; // dados contidos em uma "linha" ou endereco
// };
// typedef struct data data;

instruction instructionMemory[16] = {
    { 0, "00100001000000000000000000000000" },
    { 1, "00100001001000000000000000000000" },
    { 2, "00100001010000000000011111010000" },
    { 3, "00100011000000000000000000000100" },
    { 4, "10101101000110000000000000000000" },
    { 5, "00001000000000000000000000001001" },
    { 6, "00100001000010000000000000000001" },
    { 7, "10101111001110000000000000000000" },
    { 8, "00010101000010100000000000000001" },
    { 9, "00001000000000000000000000001110" },
    { 10, "10001111001110000000000000000000" },
    { 11, "00110001011010000000000000000001" },
    { 12, "00010101011000001111111111111001" },
    { 13, "00000001000110011100100000100000" },
    { 14, "00001000000000000000000000000101" },
    { 15, "00100000010000000000000000100000" }
};

instruction assemblyLines[16] ={
	{0, "addi $t0, $zero, 0"},	
	{1, "addi $t1, $zero, 0"}, 
	{2, "addi $t2, $zero, 2000"},
    {3, "addi $t8, $zero, 4"},
    {4, "sw $t0, ($t8)"},
    {5, "j loop"},
    {6, "addi $t0, $t0, 1"},
    {7, "sw $t9, ($t8)"},
    {8, "bne $t0, $t2, loop"},
    {9, "j end"},
    {10, "lw $t9, ($t8)"},
    {11, "andi $t3, $t0, 1"},
    {12, "bne $t3, $zero, skip"},
    {13, "add $t9, $t9, $t0"},
	{14, "j skip"},
	{15, "addi $v0, $zero, 32"}
};

data dataMemory[5] = {
	{ 0,"00000000000000000000000000000000" },  // 0
	{ 1,"00000000000000000000000000000000" },  // 1
	{ 2,"00000000000000000000000000000000" },  // 2
	{ 3,"00000000000000000000000000000000" },  // 3
	{ 4,"00000000000000000000000000000000" }   // 4
};

registers registerFile[32] = {  //32 registradores, por enquanto 5 pra testar
	{ "00000","00000000000000000000000000000000" }, //0  zero
	{ "00001","00000000000000000000000000000000" }, //1  at
	{ "00010","00000000000000000000000000000000" }, //2  v0
	{ "00011","00000000000000000000000000000000" }, //3  v1
	{ "00100","00000000000000000000000000000000" }, //4  a0
	{ "00101","00000000000000000000000000000000" }, //5  a1
	{ "00110","00000000000000000000000000000000" }, //6  a2
	{ "00111","00000000000000000000000000000000" }, //7  a3
	{ "01000","00000000000000000000000000000000" }, //8  t0 
	{ "01001","00000000000000000000000000000000" }, //9  t1
	{ "01010","00000000000000000000000000000000" }, //10 t2
	{ "01011","00000000000000000000000000000000" }, //11 t3
	{ "01100","00000000000000000000000000000000" }, //12 t4
	{ "01101","00000000000000000000000000000000" }, //13 t5
	{ "01110","00000000000000000000000000000000" }, //14 t6
	{ "01111","00000000000000000000000000000000" }, //15 t7
	{ "10000","00000000000000000000000000000000" }, //16 s0
	{ "10001","00000000000000000000000000000000" }, //17 s1 
	{ "10010","00000000000000000000000000000000" }, //18 s2 
	{ "10011","00000000000000000000000000000000" }, //19 s3
	{ "10100","00000000000000000000000000000000" }, //20 s4
	{ "10101","00000000000000000000000000000000" }, //21 s5
	{ "10110","00000000000000000000000000000000" }, //22 s6
	{ "10111","00000000000000000000000000000000" }, //23 s7
	{ "11000","00000000000000000000000000000000" }, //24 t8
	{ "11001","00000000000000000000000000000000" }, //25 t9
	{ "11010","00000000000000000000000000000000" }, //26 k0
	{ "11011","00000000000000000000000000000000" }, //27 k1
	{ "11100","00000000000000000000000000000000" }, //28 gp
	{ "11101","00000000000000000000000000000000" }, //29 sp
	{ "11110","00000000000000000000000000000000" }, //30 fp
	{ "11111","00000000000000000000000000000000" }  //31 ra
};


void printRegisters()
{	
	printf("\n ### Registers ###\n"); //
	printf("$zero \t(%s): d%ld, (b%s)\n", registerFile[0].RegisterNumber, strtol(registerFile[0].registerData, NULL, 2),registerFile[0].registerData);
	printf("$v0 \t(%s): d%ld, (b%s)\n", registerFile[2].RegisterNumber, strtol(registerFile[2].registerData, NULL, 2),registerFile[2].registerData);
	printf("$t0 \t(%s): d%ld, (b%s)\n", registerFile[8].RegisterNumber, strtol(registerFile[8].registerData, NULL, 2),registerFile[8].registerData);
	printf("$t1 \t(%s): d%ld, (b%s)\n", registerFile[9].RegisterNumber, strtol(registerFile[9].registerData, NULL, 2),registerFile[9].registerData);
	printf("$t2 \t(%s): d%ld, (b%s)\n", registerFile[10].RegisterNumber, strtol(registerFile[10].registerData, NULL, 2),registerFile[10].registerData);
	printf("$t3 \t(%s): d%ld, (b%s)\n", registerFile[11].RegisterNumber, strtol(registerFile[11].registerData, NULL, 2),registerFile[11].registerData);
	printf("$t8 \t(%s): d%ld, (b%s)\n", registerFile[24].RegisterNumber, strtol(registerFile[24].registerData, NULL, 2),registerFile[24].registerData);
	printf("$t9 \t(%s): d%ld, (b%s)\n", registerFile[25].RegisterNumber, strtol(registerFile[25].registerData, NULL, 2),registerFile[25].registerData);

}

void printDataMemory()
{
	printf("\n ### DataMemory ###\n"); //
	int i;
	for (i = 1; i < 5; i++)
	{
		printf("Mem Addr: %d - Data: d%d (b%s)\n", (dataMemory[i]).dataAddress, binaryStringToInt((dataMemory[i]).DataLine), (dataMemory[i]).DataLine);
	}
}

void printInstruction()
{
	printf("RS:");	puts(rsBinary);
	printf("RT:");	puts(rtBinary);
	printf("RD:"); 	puts(rdBinary);
	printf("Shamt:"); puts(shamtBinary);
	printf("Funct:"); puts(functBinary);
	printf("immed: d%d (b%s)\n", binaryStringToInt(immediateBinary), immediateBinary);
	printf("Address: d%d (b%s)\n", binaryStringToInt(addressBinary), addressBinary);
	// printf("SignExtended: "); puts(signExtend);
	// printf("AluOut: "); puts(aluOut);
	// printf("AluZero: %c", AluZero);
}


void signExtender()
{
	printf("Sign extender: extends %s to ", immediateBinary);
	strcpy(&signExtend[16], immediateBinary);

	char signBit = immediateBinary[0];
	for(int i = 0; i < 16; i++)
		signExtend[i] = signBit;
	printf("%s\n", signExtend);
}

int alu()
{
	
	printf("\n");
	printf("Alu\n");
	char aluSource2[33];
	char temp1[33];
	char temp2[33];
	int int1, int2, result;
	AluZero = '0';  // just to reset

	printf("\tAlu1:d%d = b%s (from reg1out)\n", binaryStringToInt(reg1Out), reg1Out);


	if (AluSrc == '0')
	{
		strcpy(aluSource2, reg2Out);
		printf("\tAlu2: d%d = b%s (from reg2Out)\n", binaryStringToInt(aluSource2), aluSource2);

	}
	else
	{
		strcpy(aluSource2, signExtend);
		printf("\tAlu2: d%d = b%s (from signExtend)\n",binaryStringToInt(aluSource2), aluSource2);

	}


	int1 = binary32ToChar(reg1Out, temp1);  //converts the entries to integer
	int2 = binary32ToChar(aluSource2, temp2);

	printf("\tALU Operation: ");
	if (AluOp == '0')	// and
	{
		result = int1 & int2;
		printf("\tand --> %d & %d\n", int1, int2);
	}
	else if (AluOp == '2') // Addition
	{
		result = int1 + int2;  // add the integers
		printf("\tadd --> %d + %d\n", int1, int2);
	}
	else if (AluOp == '6') // subtract
	{
		result = int1 - int2;  // add the integers
		printf("\tsub --> %d - %d\n", int1, int2);
	}
	else{
		printf("BREAK");
		exit(0);
	}

	if(int1 == int2)
		AluZero = '1';

	printf("AluOut: %d\nAluZero: %c\n", result, AluZero);
	
	itoa(result, temp1, 10);  // convert to text
	charTo32Bits(temp1, aluOut);  // converts it to binary text and gives the output

	return 1;//ALU_reliabilityUpdate();
}

void registerOut()
{
	int i;
	for (i = 0; i < 32; i++)
	{
		if (!strcmp(opcodeBinary, "000000") || !strcmp(opcodeBinary, "000101"))  //if it is an R-type instruction, beq ou bne...
		{
			if (!strcmp(rsBinary, registerFile[i].RegisterNumber))  //we took reg1 from RS
			{
				strcpy(reg1Out, registerFile[i].registerData);
				break;
			}
		}
		else  // type I or J, so we take reg1 from RT
		{
			if (!strcmp(rtBinary, registerFile[i].RegisterNumber))
			{
				strcpy(reg1Out, registerFile[i].registerData);
				break;
			}
		}
	}
	for (i = 0; i < 32; i++)
	{
		if ((!strcmp(opcodeBinary, "000100")) || (!strcmp(opcodeBinary, "000101")))
		{
			if (!strcmp(rtBinary, registerFile[i].RegisterNumber))
			{
				strcpy(reg2Out, registerFile[i].registerData);
				break;
			}
		}
		else
		if (!strcmp(rdBinary, registerFile[i].RegisterNumber))
		{
			strcpy(reg2Out, registerFile[i].registerData);
			break;
		}
	}
	
	if (!strcmp(opcodeBinary, "101011")) // especial para o SW
	{
		for (i = 0; i < 32; i++)
		{
			if (!strcmp(rsBinary, registerFile[i].RegisterNumber))
			{
				strcpy(reg2Out, registerFile[i].registerData);
				break;
			}
		}
	}
	printf("Reg1Out: "); puts(reg1Out);
	printf("Reg2Out: "); puts(reg2Out);
}

int verifyBranch()
{
	int signExtInteger = strtol(signExtend, NULL, 2);


	if(branch == '1')
	{
		if(strcmp(instructionAssembly, "bne")==0 && AluZero == '0')
		{
			printf("BRANCHing by moving %d instructions to", signExtInteger);
			return signExtInteger;
		}
		else if(strcmp(instructionAssembly, "beq")==0 && AluZero == '1')
		{
			printf("BRANCHing by moving %d instructions to ", signExtInteger);
			return signExtInteger;
		}
		else{
			printf("Not BRANCHING, next ");
			return 0;
		}
	}
	
	printf("Not BRANCHING, next ");
	return 0;
}

void memoryHandler()
{
	int memoryAddress;
	char temp[33];
	if (memWrite == '1')
	{
		memoryAddress = binary32ToChar(aluOut, temp);
		strcpy( dataMemory[memoryAddress/4].DataLine, reg2Out ); //copia os dados na memoria
		printf("Writing reg2out (d%d = b%s) to memory address %d\n", binaryStringToInt(reg2Out), reg2Out, memoryAddress/4);
	}

	if (memRead == '1')
	{
		memoryAddress = binary32ToChar(aluOut, temp);
		strcpy(memOut, dataMemory[memoryAddress/4].DataLine); //copia os dados na memoria
		printf("Reading from memory address %d (%s)\n", memoryAddress/4, memOut);

	}

}

void writeBack() // no banco de registradores
{
	char writeAddress[6];
	char writeData[33];
	int i;
		
	if (regWrite == '1')
	{
		
		if (regDst == '1') // Define em qual registrador sera escrito
		{   // add
			strcpy(writeAddress, rtBinary);

		}
		else
		{   // addi, lw, sw, beq (os dois ultimos nao fazem Write Back)
			strcpy(writeAddress, rsBinary);
		}

		if (memToReg == '1')  // define de onde vem os dados a serem escritos
		{
			strcpy(writeData, memOut);  // dados vem da saida de memoria
		}
		else
		{
			strcpy(writeData, aluOut);  // dados vem diretamente da Alu
		}
		char temp[10];
		registerToAssembly(writeAddress, temp);
		printf("Writing d%d (b%s) to register %s", binaryStringToInt(writeData), writeData, temp);


		// escrevendo efetivamente os dados
		for (i = 0; i < 32; i++)
		{
			registerToAssembly(writeAddress, temp);
			if  (!strcmp(registerFile[i].RegisterNumber, writeAddress) )
			{
				//printf("IDK: %s, %s", registerFile[i].RegisterNumber, writeAddress);
				strcpy(registerFile[i].registerData, writeData);
				break;
			}
		}
	}
	else
		printf("No writeback\n");
}

int main () 
{
	log_init("reliability.log");
	log_write("PROGRAM START");
	

	printf("\n ---------== Initial value of register file and memory file ==--------\n\n");
	printDataMemory(); //  imprime estado da Memoria
	printRegisters();  //  imprime estado dos registradores

	printf("\nPress key to begin...");
	getc(stdin);

	inputLine[0] = '\0';  // zera a linha de entrada
	//int PC;  // nosso Program Counter
	//int cycle = 0;
	int flag = 1;
	
	for (PC = 0 ; PC < INSTRUCTION_LENGTH; PC++ )  //performs a cycle, until the end of the instructions, the number of instructions is fixed
	{

		printf("\n\n\n================================================================================================================\n==========================================         Cycle=%d         ========================================\n", cycle++);
		printRegisters();
		printDataMemory();
		printf("\n\n\n\n");
		
		// Save processor state every 100 Cycles
		if(cycle % 1000 == 0)
		{
			saveProcessorState();
		}

		if(cycle == 6356 && flag)
		{
			flag = 0;
			log_write("Injecting transient fault into processor during cycle 6356, causing processor to hang");
			resetProcessorState();
		}
		
		
		
		
		/*
		*
		*	INSTRUCTION FETCH
		*
		*/ 
		printf("------------------=INSTRUCTION FETCH=------------------");
		printf("\nInstruction %d:\n", instructionMemory[PC].instructionAddress);  //prints the instruction index
		strcpy(inputLine, instructionMemory[PC].instructionLine); //copies the instruction to a temporary buffer
		printf("%s --> (%s)\n", assemblyLines[PC].instructionLine, inputLine);
		printf("\n\n");











		/*
		*
		*	INSTRUCTION DECODE
		*
		*/
		printf("------------------=INSTRUCTION DECODE=------------------\n");
		getOpcodeBinary(inputLine);  // rip the OpCode
		ripDataBinary(opcodeBinary); // rip the rest of the data - parse
		if (!(strcmp(opcodeBinary,"000000"))){  // R
			printf("R-Type: %s\n", instructionAssembly);
		} else if ( (!(strcmp(opcodeBinary,"000010"))) || (!(strcmp(opcodeBinary,"000011"))) ){  // J
			printf("J-Type: %s\n", instructionAssembly);
		} else {  // I
			printf("I-Type: %s\n", instructionAssembly);
		}
		printInstruction();
		signExtender(); // understands immediate
		printf("\n\n");










		/*
		*
		*	EXECUTE
		*
		*/
		printf("------------------=EXECUTE=------------------\n");
		registerOut(); // returns output values ​​from registers to buffers (global variables)
		if(!alu())         // ALU arithmetic operations
		{
			printf("\n\n\nUNRECOVERABLE FAILURE FROM ALU\n\n\n");
			break;
		}

		if(strcmp(instructionAssembly, "j") == 0)
		{
			PC = strtol(addressBinary, NULL, 2);
			printf("JUMPing to ");
		}
		else
			PC = PC + verifyBranch();    // check through "AluZero" and "Branch" and, if necessary, change the PC to take the branch
		printf("program counter = %d\n", PC+1);
		printf("\n\n");











		/*
		*
		*	MEMORY
		*
		*/
		printf("------------------=MEMORY=------------------\n");
		memoryHandler();  // manages in-memory operations
		printf("\n\n");









		/*
		*
		*	WRITEBACK
		*
		*/
		printf("------------------=WRITEBACK=------------------\n");
		writeBack();
		printf("\n\n");

		

		//printf("\nType any key to continue...");
		//getc(stdin);
		//clearScreen();	

	}
	printf("\n\n\n\n\n");
	printf("###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   \n");
	printf("###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   \n");
	printf("   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###\n");
	printf("   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###\n");
	printf("###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   \n");
	printf("###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   \n");
	printf("   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###\n");
	printf("   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###   ###\n\n\n");

	// checagem final das memorias e dos registradores!
	printf("---------------------------------------== Final Results ==---------------------------------------\n\n");

	
	printRegisters();  //  imprime estado dos registradores
	printDataMemory(); //  imprime estado da Memoria
	
return 0;
}
