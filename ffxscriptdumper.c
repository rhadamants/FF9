/**************************************************************
				Script Dumper V0.1
				Final Fantasy X				
***************************************************************

			IGNITZ <ignitzhjfk@hotmail.com>
			http://www.niitsuma.co.cc/

		Fórum Unificado de ROMHacking e Tradução
			http://www.romhacking.trd.br/

					CETRANSLATORS
			http://www.cetranslators.com.br/

					PO.B.R.E.
			http://www.romhackers.org/


O programa pode ser distribuído e modificado contanto que
os créditos originais ou modificados sejam mantidos.
**************************************************************/

// Bibliotecas
#include<stdio.h>
#include<stdlib.h>
#include<conio.h> // Usado em DEBUG durante a criação do programa
#include<string.h>

int main (int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("(C)2009 FFX Script Dumper v0.1\nIGNITZ <ignitzhjfk@hotmail.com>\n");
		printf("Usage: %s FILE\n",argv[0]);
		return -1;
	}
	FILE *arq;
	arq = fopen(argv[1],"rb");	
	if (arq == NULL) // Verifica se o arquivo arq está acessível
	{
		printf("Erro ao abrir arquivo %s\n",argv[1]);
		return -1;
	}
	///////////////
	unsigned long int x, size, check, sector[17],offset_ID; char ID[12];
	unsigned short int offset_script;
	///////////////

	fseek(arq,0,SEEK_END); size = ftell(arq); fseek(arq,0,SEEK_SET);
	
	if (size <= 0x40) // Verifica se o arquivo arq está acessível
	{
		printf("Erro ao tamanho do arquivo %s\n",argv[1]);
		return -1;
	}
	
	fread(&check,4,1,arq);
	if (check != 0x31305645)
	{
		printf("Arquivo %s inválido.\n",argv[1]);
		return -1;
	}
	

	for(x = 1; x < 16; x++)
	{
		fread(&sector[x],4,1,arq);
		// printf("%08X \n",sector[x]);
		if(sector[x] == 0xFFFFFFFF)
		{
			char string_comando[200];
			fseek(arq,sector[1]+0x0C,SEEK_SET); fread(&offset_ID,4,1,arq);
			fseek(arq,sector[1]+offset_ID,SEEK_SET); fread(ID,12,1,arq);
			fseek(arq,sector[x-2],SEEK_SET); fread(&offset_script,2,1,arq);
			sprintf(string_comando,"dumper.exe \"%s\" \"tabela.tbl\" %X %X \"%s_%s.txt\"",argv[1],sector[x-2]+offset_script,sector[x-1],argv[1],ID);
			system(string_comando); goto FINAL;
		}
	}
FINAL:
	return 0;
}
