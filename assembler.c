#include<stdio.h>
#include<string.h>

int registers[16];
/* AH,AL,BH,BL,CH,CL,DH,DL,AX,BX,CX,DX */
int CF;
FILE *op;
char binI[5];
void tobinI(int x)
{	
	int i=0;
	while(i<=3)
	{
		if(x%2==0) binI[3-i] = '0';
		else binI[3-i] = '1';
		x=x/2;
		i++;
	}
	binI[4] = '\0';
}
int R(char *c)
{
	if(strcmp(c,"AL")==0) return 0;
	else if(strcmp(c,"CL")==0) return 1;
	else if(strcmp(c,"DL")==0) return 2;
	else if(strcmp(c,"BL")==0) return 3;
	else if(strcmp(c,"AH")==0) return 4;
	else if(strcmp(c,"CH")==0) return 5;
	else if(strcmp(c,"DH")==0) return 6;
	else if(strcmp(c,"BH")==0) return 7;
	else if(strcmp(c,"AX")==0) return 8;
	else if(strcmp(c,"CX")==0) return 9;
	else if(strcmp(c,"DX")==0) return 10;
	else if(strcmp(c,"BX")==0) return 11;
	else if(strcmp(c,"SP")==0) return 12;
	else if(strcmp(c,"BP")==0) return 13;
	else if(strcmp(c,"SI")==0) return 14;
	else if(strcmp(c,"DI")==0) return 15;
	else return -1;
}
void MOV(char *b,char *c)
{
	int reg1,reg2,temp;
	char result[9];
	reg1 = R(b);
	reg2 = R(c);
	if(reg1<8)
	{
		if(reg2 != -1)
		{
			registers[reg1] = registers[reg2];
			temp = 10001010;
			fprintf(op,"%d ",temp);
			result[0] = '1';
			result[1] = '1';
			tobinI(reg1);
			result[2] = binI[1];
			result[3] = binI[2];
			result[4] = binI[3];
			tobinI(reg2);
			result[5] = binI[1];
			result[6] = binI[2];
			result[7] = binI[3];
			result[8] = '\0';
			fprintf(op,"%s\n",result);
		}
		else
		{
			temp = atoi(c);
			
			if(temp<256) 
			{
				registers[reg1] = temp;
				temp = 	1011;
				tobinI(reg1);
				fprintf(op,"%d%s %s\n",temp,binI,c);
			}
			else printf("Error in MOV \n");
		}
	}
	else
	{
		if(reg2 != -1)
		{
			registers[reg1] = registers[reg2];
			temp = 10001011;
			fprintf(op,"%d ",temp);
			result[0] = '1';
			result[1] = '1';
			tobinI(reg1);
			result[2] = binI[1];
			result[3] = binI[2];
			result[4] = binI[3];
			tobinI(reg2);
			result[5] = binI[1];
			result[6] = binI[2];
			result[7] = binI[3];
			result[8] = '\0';
			fprintf(op,"%s\n",result);
		}
		else
		{
			temp = atoi(c);
			if(temp<65536)
			{
				registers[reg1] = temp;
				temp = 	1011;
				tobinI(reg1);
				fprintf(op,"%d%s %s\n",temp,binI,c);
			}
			else printf("Error in MOV");
		}
		temp = 2*(reg1-8);
		registers[temp] = registers[reg1]/256;
		registers[temp+1] = registers[reg1]%256;
	}
}
void ADD(char *b,char *c,int type)
{
	int z,reg1,reg2,temp;
	char result[9];
	if(type == 0) z = 0; //SIMPLE ADD
	else z = CF; //ADC
	reg1 = R(b);
	reg2 = R(c);
	if(reg2 != -1)
	{
		registers[reg1] += (registers[reg2] + z);
		tobinI(reg1);
		fprintf(op,"%s%c","0000001",binI[0]);
		result[0] = '1';
		result[1] = '1';
		result[2] = binI[1];
		result[3] = binI[2];
		result[4] = binI[3];
		tobinI(reg2);
		result[5] = binI[1];
		result[6] = binI[2];
		result[7] = binI[3];
		result[8] = '\0';
		fprintf(op," %s\n",result);
	}
	else
	{
		temp = atoi(c);
		int s;
		char w;
		tobinI(reg1);
		w=binI[0];
		if(w=='0') s=0;
		else
		{
			if(temp<128&&temp>-128) s=1;
			else s=0;
		}
		if(reg1<8)
		{
			if(temp<256) 
			{
				registers[reg1] += (temp + z);
				fprintf(op,"%s%d%c %s%c%c%c %s\n","100000",s,w,"11000",binI[1],binI[2],binI[3],c);	
			}
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
	op = fopen("machineCode.txt","w");
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
				if(registers[4]==1) 
				{
					scanf("%c",&input);
					registers[0] = (int)(input);
				}
				if(registers[4]==2) printf("%c",registers[2]);
				if(registers[4]==76) break;
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
