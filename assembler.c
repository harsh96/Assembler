#include<stdio.h>
#include<string.h>

int registers[12];
/* AH,AL,BH,BL,CH,CL,DH,DL,AX,BX,CX,DX */
int CF;
int R(char *c)
{
	if(strcmp(c,"AH")==0) return 0;
	else if(strcmp(c,"AL")==0) return 1;
	else if(strcmp(c,"BH")==0) return 2;
	else if(strcmp(c,"BL")==0) return 3;
	else if(strcmp(c,"CH")==0) return 4;
	else if(strcmp(c,"CL")==0) return 5;
	else if(strcmp(c,"DH")==0) return 6;
	else if(strcmp(c,"DL")==0) return 7;
	else if(strcmp(c,"AX")==0) return 8;
	else if(strcmp(c,"BX")==0) return 9;
	else if(strcmp(c,"CX")==0) return 10;
	else if(strcmp(c,"DX")==0) return 11;
	else return -1;
}
void MOV(char *b,char *c)
{
	int reg1,reg2,temp;
	reg1 = R(b);
	reg2 = R(c);
	if(reg1<8)
	{
		if(reg2 != -1)
		{
			registers[reg1] = registers[reg2];
		}
		else
		{
			temp = atoi(c);
			if(temp<256) registers[reg1] = temp;
			else printf("Error in MOV");
		}
	}
	else
	{
		if(reg2 != -1)
		{
			registers[reg1] = registers[reg2];
		}
		else
		{
			temp = atoi(c);
			if(temp<65536) registers[reg1] = temp;
			else printf("Error in MOV");
		}
		temp = 2*(reg1-8);
		registers[temp] = registers[reg1]/256;
		registers[temp+1] = registers[reg1]%256;
	}
}
void ADD(char *b,char *c,int type)
{
	int z,reg1,reg2,temp;;
	if(type == 0) z = 0; //SIMPLE ADD
	else z = CF; //ADC
	reg1 = R(b);
	reg2 = R(c);
	if(reg2 != -1)
	{
		registers[reg1] += (registers[reg2] + z);
	}
	else
	{
		temp = atoi(c);
		if(reg1<8)
		{
			if(temp<256) registers[reg1] += (temp + z);
			else printf("Error in ADD");
		}
		else
		{
			if(temp<65536) registers[reg1] += (temp + z);
			else printf("Error in ADD");
		}	
	}
	if(reg1<8)
	{
		if(registers[reg1] >= 256)
		{
			registers[reg1] -= 256;
			CF = 1;
		}
		else CF = 0;
	}
	else
	{
		if(registers[reg1] >= 65536)
		{
			registers[reg1] -= 65536;
			CF = 1;
		}
		else CF = 0;
		temp = 2*(reg1-8);
		registers[temp] = registers[reg1]/256;
		registers[temp+1] = registers[reg1]%256;
	}
}
void SUB(char *b,char *c,int type)
{
	int z,reg1,reg2,temp;;
	if(type == 0) z = 0; //SIMPLE SUB
	else z = CF; //SBC
	reg1 = R(b);
	reg2 = R(c);
	if(reg2 != -1)
	{
		registers[reg1] += (registers[reg2] - z);
	}
	else
	{
		temp = atoi(c);
		if(reg1<8)
		{
			if(temp<256) registers[reg1] += (temp - z);
			else printf("Error in SUB");
		}
		else
		{
			if(temp<65536) registers[reg1] += (temp - z);
			else printf("Error in SUB");
		}	
	}
	if(reg1<8)
	{
		if(registers[reg1] < 0)
		{
			registers[reg1] += 256;
			CF = 1;
		}
		else CF = 0;
	}
	else
	{
		if(registers[reg1] < 0)
		{
			registers[reg1] += 65536;
			CF = 1;
		}
		else CF = 0;
		temp = 2*(reg1-8);
		registers[temp] = registers[reg1]/256;
		registers[temp+1] = registers[reg1]%256;
	}
}

main()
{  
	char f[100][20],a[20],b[20],c[20],input,dummy;
	int i=0,p;
	FILE *fp;
	fp=fopen("input.txt","r");
	do{
		fscanf(fp,"%s",f[i]);
		i++;  
	}while(strcmp(f[i-1],"END")!=0);
	i=0;
	do{
		strcpy(a,f[i++]);
		if(strcmp(a,"MOV")==0)
		{  
			strcpy(b,f[i++]);
			strcpy(c,f[i++]);
			MOV(b,c);
		}
		else if(strcmp(a,"INT")==0)
		{   
			strcpy(b,f[i++]);
			p = atoi(b);
			if(p==33)
			{
				if(registers[0]==1) 
				{
					scanf("%c",&input);
					registers[1] = (int)(input);
				}
				if(registers[0]==2) printf("%c",registers[7]);
				if(registers[0]==76) break;
			}
			else
			{
				printf("ERROR at INT statement\n");
			}
		}
		else if(strcmp(a,"ADD")==0)
		{   
			strcpy(b,f[i++]);
			strcpy(c,f[i++]);
			ADD(b,c,0);
		}
		else if(strcmp(a,"ADC")==0)
		{   
			strcpy(b,f[i++]);
			strcpy(c,f[i++]);
			ADD(b,c,1);
		}
		else if(strcmp(a,"SUB")==0)
		{   
			strcpy(b,f[i++]);
			strcpy(c,f[i++]);
			SUB(b,c,0);
		}
		else if(strcmp(a,"SBC")==0)
		{   
			strcpy(b,f[i++]);
			strcpy(c,f[i++]);
			SUB(b,c,1);
		}
   }while(1);
}
