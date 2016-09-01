/**************************************************************
				Script Inserter V0.5
				Final Fantasy X				
***************************************************************

			IGNITZ <ignitzhjfk@hotmail.com>
			http://ignitz.x10hosting.com/

		Fórum Unificado de ROMHacking e Tradução
			http://www.romhacking.trd.br/

					PO.B.R.E.
			http://www.romhackers.org/


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
		printf("(C)2011 FFX Script Inserter v0.5\nIGNITZ <ignitzhjfk@hotmail.com>\n");
		printf("Usage: %s FILE\n",argv[0]);
		return -1;
	}
	FILE *arq;
	arq = fopen(argv[1],"rb+");	
	if (arq == NULL) // Verifica se o arquivo arq está acessível
	{
		printf("Erro ao abrir arquivo %s\n",argv[1]);
		return -1;
	}
	///////////////
	unsigned long int a, x, size, check, sector[17],offset_ID, pointer_jp; char ID[12];
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
		
		//Recdompilador matador de script japonês
		if(sector[x] == 0xFFFFFFFF)
			goto FINAL;
	}
	
FINAL:
	
	fseek(arq,sector[2],SEEK_SET);
	pointer_jp = 0; fread(&pointer_jp,2,1,arq); //lê o tamanho do bloco dos ponteiros japoneses

	
	fseek(arq,sector[3],SEEK_SET);
	unsigned char *buffer;
	buffer = (unsigned char*) malloc(size - sector[2]);
	
	fread(buffer,size - sector[2],1,arq);
	
	fseek(arq,sector[2] + pointer_jp,SEEK_SET);
	unsigned long int contador;
	for( contador = 0; contador < size - sector[2] - pointer_jp; contador++)
	{
		fwrite(0x0,1,1,arq);
	}
	
	fseek(arq,sector[2] + pointer_jp,SEEK_SET);
	fwrite(buffer,size - (sector[2] + pointer_jp),1,arq);
	
	
	fseek(arq,0x4,SEEK_SET);
	//unsigned long int diferenca = sector[3]-sector[2] - pointer_jp;
	unsigned long int diferenca = sector[3] - (sector[2] + pointer_jp);
	
	
	for(x = 1; x < 16; x++)
	{	
		if(sector[x] == 0xFFFFFFFF)
			goto FINAL2;

		if(x > 2)
		{
			if(sector[x] == 0x0 || sector[x+1] == 0xFFFFFFFF)
			{
				if (sector[x+1] == 0xFFFFFFFF)
					sector[x] = size;
				
				fwrite(&sector[x],4,1,arq);
			}
			else
			{
				sector[x] -= diferenca;
				fwrite(&sector[x],4,1,arq);
			}
		}
		else
			fwrite(&sector[x],4,1,arq);
	}
	
	
	FINAL2:
	//Finalzando com chave de mestre
	fseek(arq,0x4,SEEK_SET);
	for(x = 1; x < 16; x++)
	{
		fread(&sector[x],4,1,arq);
		// printf("%08X \n",sector[x]);
		
		//Recdompilador matador de script japonês
		if(sector[x] == 0xFFFFFFFF)
		{
			goto FINAL3;
		}
	}
	FINAL3:;
	char string_comando[200];
	fseek(arq,sector[1]+0x0C,SEEK_SET); fread(&offset_ID,4,1,arq);
	fseek(arq,sector[1]+offset_ID,SEEK_SET); fread(ID,12,1,arq);
	fseek(arq,sector[x-2],SEEK_SET); fread(&offset_script,2,1,arq);
		
	//Comando adicionado para zerar ponteiros japoneses
	unsigned long int dummy = 0;
	fseek(arq,sector[2],SEEK_SET);
	unsigned int num_pointer_jp = pointer_jp / 4;
	
	for ( a = 0; a < num_pointer_jp ; a++ )
	{
		fwrite(&pointer_jp,2,1,arq); fread(&dummy,2,1,arq); dummy = 0;
	}
	
	fclose(arq);
	
	// Inserter.exe FILE614.DEC FILE614.DEC_znkd0600.txt tabela.tbl 19450
	sprintf(string_comando,"inserter.exe \"%s\" \"%s_%s.txt\" \"tabela.tbl\" %X",argv[1],argv[1],ID,sector[x-2]+offset_script);
	system(string_comando);
	sprintf(string_comando,"ffxrecalc.exe \"%s\"",argv[1]);
	system(string_comando);
	//sprintf(string_comando,"ffxtool_v03.exe \"%s\" \"%s_%s.txt\" \"tabela.tbl\" %X",argv[1],argv[1],ID,sector[x-2]+offset_script);
	
	
	return 0;
}
