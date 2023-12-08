// mips_helper.c

#include "mips_helper.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>  //strcpy()

void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* itoa(int num, char* str, int base) {
    int i = 0;
    int isNegative = 0;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with base 10.
    // Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

//recebe duas strings por referencia, sendo a primeiro uma string em binário 32bits
//segunda("a saída") em uma string representando inteiro
int binary32ToChar(char *charInput, char *charOutput)
{
	int aux = 0;
	char tempInput[33];

	if (charInput[0] == '0') // o ultimo bit, em complemento de 2, é reservado para a representação de sinal, entao se ele for '0' significa que o numero é positivo
	{
		strcpy(tempInput, charInput);
		aux = strtol(tempInput, &charOutput, 2); // função que converte binary, em uma string, para um decimal em outra string
		itoa(aux, charOutput, 10);
		//printf("Debug valor conversao: %ld\n", aux);
	}
	else // tratamento para numeros negativo
	{
		strcpy(tempInput, charInput);
		not(tempInput);
		aux = strtol(tempInput, &charOutput, 2);
		aux = (aux + 1) * -1;
		itoa(aux, charOutput, 10);
	}
	return aux;
}

//soma binaria - desconsidera carry - nao verifica inputs
int xor(int a, int b)
{
	if ( a==b )
	{
		return 0;
	}
	else
	{
		return a + b;
	}
}

void not (char *string)
{
	int i=0;
	do
	{
		if (string[i] == '0') 
		{
			string[i] = '1';    				
		}
		else
		{
			string[i]='0';
    	}
		i++;
	} while (string[i] != '\0');
}

void padZero(char *string, int length)
{
	int i=0, originalLength;
	originalLength = strlen(string);  //tamanho antes do preenchimento
	char stringTemp[17] = {0};        // array temporario
	strncpy (stringTemp, string, originalLength);  //guarda a string original
	stringTemp[originalLength] = '\0';
	
	int padLength = length - strlen(string);  // diferenca que preencheremos com '0'
	if (padLength < 0) padLength = 0;    // Evitamos tamanhos negativos - nunca deveria ser
	
	for (i=0; i < padLength; i++)
	{
		string[i] = '0';		
	}
	strncpy ( (string + padLength) ,stringTemp, originalLength);   // concatena de volta na string (agora com zeros) o conteudo original
}

// retorna 1 se o caracter eh numerico, incluindo o hifen para caso de negativos
// retorna 0 se qualquer outro.
int isNumeric (char ch)
{
	int output;
	switch (ch){
		case '0': output = 1;
		case '1': output = 1;
		case '2': output = 1;
		case '3': output = 1;
		case '4': output = 1;
		case '5': output = 1;
		case '6': output = 1;
		case '7': output = 1;
		case '8': output = 1;
		case '9': output = 1;
		case '-': output = 1;   // podemos argumentar que pra outros usos esta nao eh uma escolha sabia =)
		break;
	default:
		output = 0;
	}
	return output;
}

// similar a funcao isNumeric() 
int isBinary (char ch)
{
	int output;
	switch (ch){
		case '0': output = 1;
		case '1': output = 1;
		break;
	default:
		output = 0;
	}
	return output;
}

// recebe duas strings por referencia, a primeira eh a entrada ("numeros")
//  a segunda devolve a string de binário UNSIGNED de 5 bits
void charTo5BitsU (char *charInput, char *charOutput)   
{
	int num = 33; // gambiarra pra cair fora do segundo if
	if ( charInput[0] != '-' ) //procura numeros negativos, eles nem deveriam ser chamados
		num=atoi(charInput);   //num receberá o char passado para int
	
	if( num<32 ) // limite do range de 16bits
	{
        //itoa(int ,char* ,base int);
        itoa(num,charOutput,2);  // converte o numero intero 'num' em um binário de base 2 e coloca na string 'charBinary'
		padZero(charOutput, 5);
		charOutput[5] = '\0';
	}else
	{
	    strcpy(charOutput,"XXXXX");  // saida de erro
		charOutput[5] = '\0';
	}
}
 // recebe duas strings por referencia, a primeira eh a entrada ("numeros")
 //  a segunda devolve a string de binário UNSIGNED de 16 bits
void charTo16BitsU (char *charInput, char *charOutput)
{
	int num = 65537; // gambiarra pra cair fora do segundo if
	if ( charInput[0] != '-' ) //procura numeros negativos, eles nem deveriam ser chamados
		num=atoi(charInput);   //num receberá o char passado para int
	
	if( num<65536 ) // limite do range de 16bits
	{
        //itoa(int ,char* ,base int);
        itoa(num,charOutput,2);  // converte o numero intero 'num' em um binário de base 2 e coloca na string 'charBinary'
		padZero(charOutput, 16);
	}else
	{
	    strcpy(charOutput,"XXXXXXXXXXXXXXXX");  // saida de erro
		charOutput[16] = '\0';
	}
}
// recebe duas strings por referencia, a primeira eh a entrada ("numeros")
//  a segunda devolve a string de binário em Complemento de 2 em 16 bits
void charTo16Bits (char *charInput, char *charOutput)
{

	int num=atoi(charInput);   //num receberá o char passado para int	
	
	if (-32768 <= num < 32768)
	{
		if (num>=0) // se o numero é maior que 0, é passado pra binário normalmente
		{
	        //itoa(int ,char* ,base int);
	        itoa(num,charOutput,2); // converte o numero intero 'num' em um binário de base 2 e coloca na string 'charBinary'
			padZero(charOutput, 16);  //completa com zeros
		}
		else
		{
		    itoa ( (num*(-1)) -1 , charOutput,2 ); // se o numero é negativo, ele é passado pra positivo (e subtraimos 1)
		    padZero(charOutput, 16);  //completa com zeros
			not (charOutput);   	// complemento do array
		}
	}else
	{
		strcpy(charOutput,"XXXXXXXXXXXXXXXX");  // saida de erro
	}
}

void charTo26BitsU (char *charInput, char *charOutput)
{
	int num = 67108865; // gambiarra pra cair fora do segundo if
	if ( charInput[0] != '-' ) //procura numeros negativos, eles nem deveriam ser chamados
		num=atoi(charInput);   //num receberá o char passado para int
	
	if( num<67108864 ) // limite do range de 26bits
	{
        //itoa(int ,char* ,base int);
        itoa(num,charOutput,2);  // converte o numero intero 'num' em um binário de base 2 e coloca na string 'charBinary'
		padZero(charOutput, 26);
	}else
	{
	    strcpy(charOutput,"XXXXXXXXXXXXXXXXXXXXXXXXXX");  // saida de erro
		charOutput[26] = '\0';
	}
}

// recebe duas strings por referencia, a primeira eh a entrada ("numeros")
//  a segunda devolve a string de binário em Complemento de 2 em 32 bits
void charTo32Bits(char *charInput, char *charOutput)
{

	int num = atoi(charInput);   //num receberá o char passado para int	

	if (-32768 <= num < 32768)
	{
		if (num >= 0) // se o numero é maior que 0, é passado pra binário normalmente
		{
			//itoa(int ,char* ,base int);
			itoa(num, charOutput, 2); // converte o numero intero 'num' em um binário de base 2 e coloca na string 'charBinary'
			padZero(charOutput, 32);  //completa com zeros
		}
		else
		{
			itoa((num*(-1)) - 1, charOutput, 2); // se o numero é negativo, ele é passado pra positivo (e subtraimos 1)
			padZero(charOutput, 32);  //completa com zeros
			not (charOutput);   	// complemento do array
		}
	}
	else
	{
		printf("Debug: Error in charTo32Bits!");
		strcpy(charOutput, "XXXXXXXXXXXXXXXX");  // saida de erro
	}
}


/*
//recebe duas strings por referencia, sendo a primeiro uma string em binário 16bits
//segunda("a saída") em uma string representando inteiro
void binary16ToChar(char *charInput, char *charOutput)
{
	int i,aux=0;
	char tempInput[17];
	
	if(charInput[0]=='0') // o ultimo bit, em complemento de 2, é reservado para a representação de sinal, entao se ele for '0' significa que o numero é positivo
	{
		strcpy(tempInput,charInput);
		aux = strtol(tempInput, &charOutput, 2); // função que converte binary, em uma string, para um decimal em outra string
		itoa ( aux, charOutput, 10 );
		printf("teste apos: %ld\n", aux);
	}
	else // tratamento para numeros negativos
	{
		strcpy(tempInput,charInput);
		not(tempInput);
		aux = strtol( tempInput, &charOutput, 2);
		aux = (aux+1) * -1 ;
		itoa ( aux, charOutput, 10 );
	}
	strcpy(immediateAssembly,charOutput);
}

void binary5ToCharU(char *charInput, char *charOutput)
{
	int aux=0;
	char tempInput[6];
	strcpy(tempInput,charInput);
	aux = strtol(tempInput, &charOutput, 2); // função que converte binary, em uma string, para um decimal em outra string
	itoa ( aux, charOutput, 10 );
	strcpy(shamtAssembly,charOutput); // corrigir logica
}

void binary16ToCharU(char *charInput, char *charOutput) // funcao crasha para numero grandes
{
	int aux=0;
	char tempInput[17];
	strcpy(tempInput,charInput);
	aux = strtol(tempInput, &charOutput, 2); // função que converte binary, em uma string, para um decimal em outra string
	itoa ( aux, charOutput, 10 );
	strcpy(immediateAssembly,charOutput); // corrigir logica
}

void binary26ToCharU(char *charInput, char *charOutput)
{
	int aux=0;
	char tempInput[27];
	strcpy(tempInput,charInput);
	aux = strtol(tempInput, &charOutput, 2); // função que converte binary, em uma string, para um decimal em outra string
	itoa ( aux, charOutput, 10 );
	strcpy(addressAssembly,charOutput); // corrigir logica
}

*/

// #######  Funcoes de binary -> Assembly

void getOpcodeBinary (char *inputLine)
{
	strncpy (opcodeBinary, inputLine, 6);
	opcodeBinary[6] = '\0';
}

void ripDataBinary(char *opcodeBinary)
{
	strncpy(rsBinary, inputLine + 6, 5);  // does the split according to the R function
	strncpy(rtBinary, inputLine + 11, 5);
	strncpy(rdBinary, inputLine + 16, 5);
	strncpy(shamtBinary, inputLine + 21, 5);
	strncpy(functBinary, inputLine + 26, 6);
	strncpy(addressBinary, inputLine + 6, 26);   // does the split according to the J function
	strncpy(immediateBinary, inputLine + 16, 16);

	if (!(strcmp(opcodeBinary,"000000"))){  // R
		instructionR();
	} else if ( (!(strcmp(opcodeBinary,"000010"))) || (!(strcmp(opcodeBinary,"000011"))) ){  // J
		instructionJ();
	} else {  // I
		instructionI();
	}
}

// funcao que converte um registro binario em assembly
void registerToAssembly(char *registerBinary, char *registerAssembly)
{
	if (!(strcmp(registerBinary,"00000"))){  		  //retorna 0 se iguais, portanto !0 = verdadeiro
		strcpy(registerAssembly,"$zero");
	} else if (!(strcmp(registerBinary,"00001"))) {   	// 1 	Assemble temporary
		strcpy(registerAssembly,"$at");		
	} else if (!(strcmp(registerBinary,"00010"))) {		// 2 	Function Results and Expression Evaluation
		strcpy(registerAssembly,"$v0");				
	} else if (!(strcmp(registerBinary,"00011"))) {
		strcpy(registerAssembly,"$v1");			
	} else if (!(strcmp(registerBinary,"00100"))) {		// 4	Arguments
		strcpy(registerAssembly,"$a0");		
	} else if (!(strcmp(registerBinary,"00101"))) {
		strcpy(registerAssembly,"$a1");		
	} else if (!(strcmp(registerBinary,"00110"))) {
		strcpy(registerAssembly,"$a2");		
	} else if (!(strcmp(registerBinary,"00111"))) {
		strcpy(registerAssembly,"$a3");		
	} else if (!(strcmp(registerBinary,"01000"))) {		// 8	Temporaries
		strcpy(registerAssembly,"$t0");		
	} else if (!(strcmp(registerBinary,"01001"))) {
		strcpy(registerAssembly,"$t1");
	} else if (!(strcmp(registerBinary,"01010"))) {
		strcpy(registerAssembly,"$t2");
	} else if (!(strcmp(registerBinary,"01011"))) {
		strcpy(registerAssembly,"$t3");
	} else if (!(strcmp(registerBinary,"01100"))) {
		strcpy(registerAssembly,"$t4");
	} else if (!(strcmp(registerBinary,"01101"))) {
		strcpy(registerAssembly,"$t5");
	} else if (!(strcmp(registerBinary,"01110"))) {
		strcpy(registerAssembly,"$t6");
	} else if (!(strcmp(registerBinary,"01111"))) {
		strcpy(registerAssembly,"$t7");												
	} else if (!(strcmp(registerBinary,"10000"))) {		// 16	Saved Temporaries
		strcpy(registerAssembly,"$s0");		
	} else if (!(strcmp(registerBinary,"10001"))) {
		strcpy(registerAssembly,"$s1");
	} else if (!(strcmp(registerBinary,"10010"))) {
		strcpy(registerAssembly,"$s2");
	} else if (!(strcmp(registerBinary,"10011"))) {
		strcpy(registerAssembly,"$s3");
	} else if (!(strcmp(registerBinary,"10100"))) {
		strcpy(registerAssembly,"$s4");
	} else if (!(strcmp(registerBinary,"10101"))) {
		strcpy(registerAssembly,"$s5");
	} else if (!(strcmp(registerBinary,"10110"))) {
		strcpy(registerAssembly,"$s6");
	} else if (!(strcmp(registerBinary,"10111"))) {
		strcpy(registerAssembly,"$s7");		
	} else if (!(strcmp(registerBinary,"11000"))) {		// 24	Temporaries
		strcpy(registerAssembly,"$t8");
	} else if (!(strcmp(registerBinary,"11001"))) {
		strcpy(registerAssembly,"$t9");			
	} else if (!(strcmp(registerBinary,"11010"))) {		// 26	Reserved for OS Kernel
		strcpy(registerAssembly,"$k0");
	} else if (!(strcmp(registerBinary,"11011"))) {
		strcpy(registerAssembly,"$k1");
	} else if (!(strcmp(registerBinary,"11100"))) {
		strcpy(registerAssembly,"$gp");
	} else if (!(strcmp(registerBinary,"11101"))) {
		strcpy(registerAssembly,"$sp");
	} else if (!(strcmp(registerBinary,"11110"))) {
		strcpy(registerAssembly,"$fp");
	} else if (!(strcmp(registerBinary,"11111"))) {
		strcpy(registerAssembly,"$ra");					
	} else {
		strcpy(registerAssembly ,"$xx");    //caso de Erro, nenhum registro compativel encontrado
	}
}

//  ### bloco de cada funcado de binary -> assembly
void addToAssembly() // R
{
	strcpy(instructionAssembly, "add");
	regWrite = '1';
	regDst = '1';
	AluSrc = '0';
	AluOp = '2';
	memRead = '0';
	memWrite = '0';
	memToReg = '0';
	branch = '0';
}

void adduToAssembly() // R
{
	strcpy(instructionAssembly, "addu");
}

void andToAssembly() // R
{
	strcpy(instructionAssembly, "and");
}

void jrToAssembly() // R
{
	strcpy(instructionAssembly, "jr");
}

void norToAssembly() // R
{
	strcpy(instructionAssembly, "nor");
}

void orToAssembly() // R
{
	strcpy(instructionAssembly, "or");
}

void sltToAssembly() // R
{
	strcpy(instructionAssembly, "slt");
}

void sltuToAssembly() // R
{
	strcpy(instructionAssembly, "sltu");
}

void sllToAssembly() // R
{
	strcpy(instructionAssembly, "sll");
}

void srlToAssembly() // R
{
	strcpy(instructionAssembly, "srl");
}

void subToAssembly() // R
{
	strcpy(instructionAssembly, "sub");
}

void subuToAssembly() // R
{
	strcpy(instructionAssembly, "subu");
}
// tipo I
void addiToAssembly()  // I
{
	strcpy(instructionAssembly, "addi");
	regWrite = '1';
	regDst = '0';
	AluSrc = '1';
	AluOp = '2';
	memRead = '0';
	memWrite = '0';
	memToReg = '0';
	branch = '0';
}

void andiToAssembly()  // I
{
	strcpy(instructionAssembly, "andi");
	regWrite = '1';
	regDst = '0';
	AluSrc = '1';
	AluOp = '0';
	memRead = '0';
	memWrite = '0';
	memToReg = '0';
	branch = '0';
}

void beqToAssembly()  // I
{
	strcpy(instructionAssembly, "beq");
	regWrite = '0';
	regDst = '0';
	AluSrc = '0';
	AluOp = '6';
	memRead = '1';
	memWrite = '0';
	memToReg = '0';
	branch = '1';
}

void bneToAssembly()  // I
{
	strcpy(instructionAssembly, "bne");
	regWrite = '0';
	regDst = '0';
	AluSrc = '0';
	AluOp = '6';
	memRead = '0';
	memWrite = '0';
	memToReg = '0';
	branch = '1';
}

void lbuToAssembly()  // I parenteses
{
	strcpy(instructionAssembly, "lbu");
}

void lhuToAssembly()  // I parenteses
{
	strcpy(instructionAssembly, "lhu");
}

void llToAssembly()  // I parenteses - rever
{
	strcpy(instructionAssembly, "ll");
}

void luiToAssembly()    // I
{
	strcpy(instructionAssembly, "lui");
}

void lwToAssembly()    // I parenteses
{
	strcpy(instructionAssembly, "lw");
	regWrite = '1';
	regDst = '0';
	AluSrc = '1';
	AluOp = '2';
	memRead = '1';
	memWrite = '0';
	memToReg = '1';
	branch = '0';
}

void oriToAssembly()    // I
{
	strcpy(instructionAssembly, "ori");
}

void sltiToAssembly()    // I
{
	strcpy(instructionAssembly, "slti");
}

void sltiuToAssembly()    // IU
{
	strcpy(instructionAssembly, "sltiu");
}

void sbToAssembly()  // I parenteses
{
	strcpy(instructionAssembly, "sb");
}

void scToAssembly()  // I parenteses
{
	strcpy(instructionAssembly, "sc");
}

void shToAssembly()  // I parenteses
{
	strcpy(instructionAssembly, "sh");
}

void swToAssembly()  // I parenteses
{
	strcpy(instructionAssembly, "sw");
	regWrite = '0';
	regDst = '0';
	AluSrc = '1';
	AluOp = '2';
	memRead = '0';
	memWrite = '1';
	memToReg = '0';
	branch = '0';
}

void jToAssembly()  // J
{
	strcpy(instructionAssembly, "j");
	regWrite = '0';
	regDst = '0';
	AluSrc = '0';
	AluOp = '2';
	memRead = '0';
	memWrite = '0';
	memToReg = '0';
	branch = '0';

}

void jalToAssembly()  // J
{
	strcpy(instructionAssembly, "jal");
}


void instructionR ()
{
	if (!(strcmp(functBinary,"100000"))){  		  //retorna 0 se iguais, portanto !0 = verdadeiro
		addToAssembly(); // R		
	} else if (!(strcmp(functBinary,"100001"))){  		 
		adduToAssembly(); // RU	
	} else if (!(strcmp(functBinary,"100100"))){
		andToAssembly();  //R
	} else if (!(strcmp(functBinary,"001000"))){
		jrToAssembly();  //R
	} else if (!(strcmp(functBinary,"100111"))){
		norToAssembly();  //R			
	} else if (!(strcmp(functBinary,"100101"))){
		orToAssembly();  //R
	} else if (!(strcmp(functBinary,"101010"))){
		sltToAssembly();  //R
	} else if (!(strcmp(functBinary,"101011"))){
		sltuToAssembly();  //R
	} else if (!(strcmp(functBinary,"000000"))){
		sllToAssembly();  //R
	} else if (!(strcmp(functBinary,"000010"))){
		srlToAssembly();  //R
	} else if (!(strcmp(functBinary,"100010"))){
		subToAssembly();  //R
	} else if (!(strcmp(functBinary,"100011"))){
		subuToAssembly();  //RU
	} else {
		printf("ERRO! R function not recognized");    //caso de Erro, nenhum instruction compativel encontrado
	}	
}

void instructionJ ()
{
	if (!(strcmp(opcodeBinary,"000010"))){  		  //returns 0 if equal, therefore !0 = true
		jToAssembly(); 
	} else if (!(strcmp(opcodeBinary,"000011"))){
		jalToAssembly();   		 
	} else {
		printf("ERRO! J Function not recognized");    //caso de Erro, nenhum instruction compativel encontrado

	}		
}

void instructionI ()
{
	if (!(strcmp(opcodeBinary,"001000"))){  		  //retorna 0 se iguais, portanto !0 = verdadeiro
		addiToAssembly();    // I
	} else if (!(strcmp(opcodeBinary,"001100"))){
		andiToAssembly();    // I
	} else if (!(strcmp(opcodeBinary,"000100"))){
		beqToAssembly();     // I
	} else if (!(strcmp(opcodeBinary,"000101"))){
		bneToAssembly(); 	 // I	
	} else if (!(strcmp(opcodeBinary,"100100"))){
		lbuToAssembly(); 	 // I parenteses	
	} else if (!(strcmp(opcodeBinary,"100101"))){
		lhuToAssembly(); 	 // I parenteses
	} else if (!(strcmp(opcodeBinary,"110000"))){
		llToAssembly(); // I parenteses - rever
	} else if (!(strcmp(opcodeBinary,"001111"))){
		luiToAssembly();    // I
	} else if (!(strcmp(opcodeBinary,"100011"))){
		lwToAssembly();    // I
	} else if (!(strcmp(opcodeBinary,"001101"))){
		oriToAssembly();    // I
	} else if (!(strcmp(opcodeBinary,"001010"))){
		sltiToAssembly();    // I
	} else if (!(strcmp(opcodeBinary,"001011"))){
		sltiuToAssembly();    // IU
	} else if (!(strcmp(opcodeBinary,"101000"))){
		sbToAssembly();    // I parenteses
	} else if (!(strcmp(opcodeBinary,"111000"))){
		scToAssembly();    // I ??
	} else if (!(strcmp(opcodeBinary,"101001"))){
		shToAssembly();    // I parenteses
	} else if (!(strcmp(opcodeBinary,"101011"))){
		swToAssembly();    // I ??
	} else {
		printf("ERRO! Instruction not recognized");    //caso de Erro, nenhum instruction compativel encontrado
	}	
}