/*
 *This C program splits the input file into multiple files of specified sizes or lines. 
 *Input is the filename and spliting option (either size or line) with corresponding values
 *To run the program:
 *
 *    gcc main.c -o split
 *    
 *    ./split -s 1KB <input_file_name>
 *
 *Here the input file is splitted into sub files each of 1KB in size.
 *
 *    ./split -n 100 <input_file_name>
 *
 *Here the input file is splitted into sub files each consisting of 100 lines.
 *
 *Output:
 *
 *    Details of Splitted files with corresponding memory size or lines.
 *
 *
 *	Author : Joe Claiment Raja Xavier
 *	email: x.joeclament@gmail.com
 *
 *
 *
 *
 */



#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<sys/stat.h>
#include<dirent.h>

#define MAX_LINE_SIZE 1024
#define FILE_NAME_LENGTH 20
#define FILE_EXTENSION_LENGTH 4

//Function to calculate the size of the file in Bytes
int fsize(FILE *fp)
{
    int start=ftell(fp);
    fseek(fp, 0, SEEK_END);	// move the file pointer to the end of the file
    int size=ftell(fp);         // no of bytes from begining to the current position of the file pointer
    fseek(fp,start,SEEK_SET); 	// move the file pointer back to the begining of the file 
    return size;
}

//Function to convert the input string to integer
int string_to_number(char* a)
{
	int num=0;
	int i;
	for(i=0;!(isalpha(a[i])) && *(a+i)!='\0';i++)	// capture the numbers entered in input
	{
		num=num*10+*(a+i)-'0';
	}

	return num;
	
}

//Function to calculate the no of lines in the file
int line_count(FILE *fp)
{
	int lc=0;
	int c=0;
	while((c=getc(fp))!=EOF)	// check for the newline character and increments the lc variable
	{
		if(c=='\n')
			lc++;
	}
	rewind(fp);	// reset the file pointer to initial position 
	return lc;
}
int main(int argc,char** argv)
{
	FILE *fileptr;		// File pointer to the input file
	FILE *fileptr2;		// file pointer to the splitted files
	char buffer[MAX_LINE_SIZE];
	char *options;
	char filename[200];
	char split_filename[260];
	int lines_of_each_block=0;
	int linecount=0;
	int size_of_block=0;
	int size_type=0;
	int filesize=0;
	int segments=0;
	int count=0;
	int accumulator=0;
	char *ext;
	char *p1;
	ext=malloc(sizeof(char)*FILE_EXTENSION_LENGTH);
	p1=malloc(sizeof(char)*FILE_NAME_LENGTH);
 	strcpy(filename,*(argv+3));	
	fileptr=fopen(filename,"r"); 	// open the input file
	filesize=fsize(fileptr);	// size of the input file in bytes
	strcpy(p1,filename);// seperate filename and the extension

	if(strstr(p1,"."))
	{
	strcpy(p1,strtok(p1,"."));
	strcpy(ext,strchr(filename,'.'));
	}

	if(fileptr==NULL)
	{
		printf("File doesn't Exists!\n");
		exit(EXIT_FAILURE);
	}


	if(argv[1][1]=='s')
	{
		size_of_block=string_to_number(*(argv+2));
		if(strstr(*(argv+2),"KB"))
		{
			size_type=1024;	// 1KB=1024 Bytes
		}
		else if(strstr(*(argv+2),"MB"))
		{
			size_type=1024*1024;	//1MB=1024 KB
		}
		else if(strstr(*(argv+2),"B"))
		{
			size_type=1;
		}

		else
		{
			fprintf(stderr,"Split size should be B | KB | MB only!\n");
			exit(EXIT_FAILURE);
		}
		size_of_block*=size_type;
		segments=filesize/size_of_block+1;
		if(filesize>size_of_block)
		{
			printf("Splitting %s...\n",filename);
			printf("Splitting by %d subfilesof size %s each...\n",segments,*(argv+2));	
	
			for(int i=0;i<segments;i++)
			{
					
				sprintf(split_filename,"%s_%d",p1,i+1);
				strcat(split_filename,ext);
				fileptr2=fopen(split_filename,"w+");
				if(fileptr2)
				{
				accumulator=0;
				count=0;
				while(count<size_of_block)
				{	
					if((accumulator=getc(fileptr))==EOF) // copy each character to split files of given size
						break;

					putc(accumulator,fileptr2);
					count++;
				}

				}
				else
				{
					fprintf(stderr,"Failed to write file!\n");
					exit(EXIT_FAILURE);
				}
				printf("%d->%s of %d bytes\n",i+1,split_filename,fsize(fileptr2)); //Operations done
				fclose(fileptr2);
			}
			free(ext);
			free(p1);
		}
		
		else if(filesize==size_of_block)
		{
			fprintf(stderr,"Splitting size is equal to that of input file.\nNo need for splitting...\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			fprintf(stderr,"Splitting size larger than File size!\n");
			exit(EXIT_FAILURE);
		}
		fclose(fileptr);	

	}
	else if(argv[1][1]=='n')
	{
		int c;
		lines_of_each_block=string_to_number(*(argv+2));
		linecount=line_count(fileptr);
		segments=(lines_of_each_block==1)?linecount/lines_of_each_block:linecount/lines_of_each_block+1;
		if(lines_of_each_block<linecount)
		{
			printf("Splitting %s...\n",filename);
			printf("Splitting into %d files of %d lines each...\n",segments,lines_of_each_block);	
	
			for(int i=0;i<segments;i++)
			{
				sprintf(split_filename,"%s_%d",p1,i+1);	// opening the split files
				strcat(split_filename,ext);
				fileptr2=fopen(split_filename,"w+");
				if(fileptr2)
				{
				count=0;
				while(count<lines_of_each_block)
				{
					if(fgets(buffer,MAX_LINE_SIZE,fileptr)==NULL)	// copy each line to the split files
						break;

					fputs(buffer,fileptr2);
					count++;

				}
				}
				else
				{
					fprintf(stderr,"Failed to write file!\n");
					exit(EXIT_FAILURE);
				}
				fclose(fileptr2);
				fileptr2=fopen(split_filename,"r");
				printf("%d->%s of %d lines\n",i+1,split_filename,line_count(fileptr2));
		
				fclose(fileptr2);


			}
			free(ext);
			free(p1);
		}
		else if(linecount==lines_of_each_block)
		{
			fprintf(stderr,"Splitting lines is equal to the input file.\nNo need for Splitting...\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			fprintf(stderr,"Splitting lines larger than lines in the file!\n");
			exit(EXIT_FAILURE);
		}
		fclose(fileptr);	

		
									
	}
	else
	{
		fprintf(stderr,"Usage: -s for splitting in sizes | -n for splitting in lines\n");
		exit(EXIT_FAILURE);
	}	
	exit(EXIT_SUCCESS);
	return 0;
		

	

	//Main function
}
