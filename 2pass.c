// Name: Vertika Sahu					Class: BTech CS 4th Sem. Batch C	
// Roll No.: 1913169					ID: BTBTC19181

// Two Pass Algorithm
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	FILE *fp1,*mdt,*mnt,*exp,*final;
	int MDTC=0,MNTC=0,i,l,k,flag,MDTP,m;
	char c,m_name,st[20],ALA[5],mnt_line[20],macro_line[20];
	
	fp1=fopen("INPUT_CODE.txt","r");
	
	if(fp1==NULL)
	{
		printf("Cannot open file Input_code\n");
		exit(0);
	}
	
	mdt=fopen("MDT.txt","w");
	
	if(mdt==NULL)
	{
		printf("Cannot open file mdt\n");
		exit(0);
	}
	
	mnt=fopen("MNT.txt","w");
	
	if(mnt==NULL)
	{
		printf("Cannot open file mnt\n");
		exit(0);
	}
	
	exp=fopen("EXPAND.txt","w");		//EXPAND is the intermediate code file before macro expansion
	
	if(exp==NULL)
	{
		printf("Cannot open file Exp\n");
		exit(0);
	}
	
	//Beginning of Pass 1
	do
	{
		fgets(st,20,fp1);			//reading one source card at a time from input code
		
		if(strcmp(st,"MACRO\n")==0)		//checking if the source card is MACRO pseudo-op
		{
			fgets(st,20,fp1);
	
			i=0;
			MNTC++;
			fputc((MNTC+'0'),mnt);
			fputc(' ',mnt);
			while(st[i]!='\n')			//getting the macro name and storing it in MNT table
			{
				putc(st[i],mnt);
				i++;
			}
			fputc(' ',mnt);
			MDTC++;					//increasing the value of MDTC by 1
			c=MDTC+'0';
			fputc(c,mnt);				//writing the value of MDTC of MDT as MDTP in MNT
			fputc('\n',mnt);
		
			fputc(c,mdt);				//writing the value for MDTC in MDT
			fputc(' ',mdt);
			fputs(st,mdt);				//copying the entire string with macro name and arguments in MDT
		
			i=0;
			l=0;
			while(st[i]!='\0') 			//setting up ALA
			{
				if(st[i]=='&')			//argument is placed next to symbol '&'
				{
					ALA[l]=st[i+1]; 	//placing the argument in lth index of ALA 
					l++;	
				}
				
				i++;
			}
			do
			{
				fgets(st,20,fp1);		//reading next source card
				i=0;
				MDTC++;				//increasing the value of MDTC by 1
				while(st[i]!='\0')		//substituting index notation for arguments
				{
					if(st[i]=='&')
					{
						st[i]='#';
						for(k=0;k<l;k++)
						{
							if(st[i+1]==ALA[k])	
							{
								st[i+1]=k+'0';
							}
						}	
					}
					i++;
				}
			
				c=MDTC+'0';
				fputc(c,mdt);		//entering the value of MDTC in MDT
				fputc(' ',mdt);	
				fputs(st,mdt);		//entering the line in MDT
		
			}while(strcmp(st,"MEND\n")!=0);		//checking if the source card is MEND pseudo-op
		}
		
		else	//if the source card is not a MACRO pseudo-op then write copy of source code
		{
			fputs(st,exp);
			if(strcmp(st,"END")==0)		//if source card is END then close files and go to pass 2
			{
				printf("Pass 1 complete!!\n");
				fclose(fp1);
				fclose(mdt);
				fclose(mnt);
				fclose(exp);
				goto pass2;
			}
		}	
	}while(1);
	//End of Pass 1
	
	
	//Beginning of Pass 2

	pass2:
		MDTP=1;
		
		exp=fopen("EXPAND.txt","r");
		
		if(exp==NULL)
		{
			printf("Cannot open file EXPAND\n");
			exit(0);
		}
		
		final=fopen("FINAL_CODE.txt","w");
		
		if(final==NULL)
		{
			printf("Cannot open file FINAL_CODE\n");
			exit(0);
		}
	
		do
		{
			fgets(st,20,exp);		//read one line at a time from EXPAND code
		
			mnt=fopen("MNT.txt","r");
			if(mnt==NULL)
			{
				printf("Cannot open file MNT\n");
				exit(0);
			}
			
			flag=0;
			
			do			//searching MNT for match with operation code
			{
				while(c!=' ' && c!=EOF) 
					c=fgetc(mnt);
				
				c=fgetc(mnt);
				m=0;
				i=0;
				while(c!=' ' && c!=EOF) 
				{
					if(c==st[i])
						m++;

					i++;
					c=fgetc(mnt);		
				}
				if(m==i && m!=0)		//if Macro name found- flag=1 else flag=0
				{
					flag=1;
					c=fgetc(mnt);
					while(c!=' ')
					{
						c=fgetc(mnt);	
					}
					
					MDTP=(fgetc(mnt)-'0');		//MDT index from MNT entry
					break;
				}
			}while(c!=EOF);	
		
			fclose(mnt);
			
			if(flag)		//if Macro name found
			{
				i=0;
				l=0;
				while(st[i]!='\0')		// setting up ALA
				{
			 	if(st[i]==',')
					{
						ALA[l]=st[i-1];
						l++;
					}		
					i++;
				}
				ALA[l]=st[i-2];
			
				mdt=fopen("MDT.txt","r");
				if(mdt==NULL)
				{
					printf("Cannot open file MDT\n");
					exit(0);
				}
				do    //reaching to that line in MDT from where the macro definition for that particular macro begins
				{
					fgets(macro_line,20,mdt);	
				}while(macro_line[0]!=(MDTP+'0'));
				
				do
				{		
					fgets(macro_line,20,mdt);	//getting line from MDT
					i=0;
					while(macro_line[i]!='\0')	//substituting arguments from macro call
					{
						if(macro_line[i]=='#')
						{
							strcpy(&macro_line[i],&macro_line[i+1]); 	//to remove symbol '#'
							for(k=0;k<=l;k++)
							{
								if((macro_line[i]-'0')==k)	
								{
									macro_line[i]=ALA[k];
								}
							}	
						}
						i++;
					}
					strcpy(&macro_line[0],&macro_line[1]);	  //removing MDTC 
					strcpy(&macro_line[0],&macro_line[1]);	  //and extra space ' ' from the MDT line before copying the line to output file 
				
					
					if(strcmp(macro_line,"MEND\n")!=0)		//checking if the line is MEND pseudo-op
					{
						fputs(macro_line,final);			//write the expanded line in output file if the line is not MEND
					}
					else
						break;
									
				}while(1);
				fclose(mdt);
			}
			
			else		//if Macro name not found
			{
				fputs(st,final);			//write into the output file
				if(strcmp(st,"END")==0)		//if END pseudo-op then close the files and terminate
				{
					printf("Pass 2 complete!!\n");
					fclose(exp);
					fclose(final);	
					exit(0);
				}
			}	
		}while(1);
		//End of Pass 2
}

