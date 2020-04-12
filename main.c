#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include<conio.h>

bool isKeyword(char* str)     //to check among the available keywords of C. char* to take string of undefined length.
{
    if (!strcmp(str, "auto") || !strcmp(str, "default")
        || !strcmp(str, "signed") || !strcmp(str, "enum")
        ||!strcmp(str, "extern") || !strcmp(str, "for")
        || !strcmp(str, "register") || !strcmp(str, "if")
        || !strcmp(str, "else")|| !strcmp(str, "else if") || !strcmp(str, "int")
        || !strcmp(str, "while") || !strcmp(str, "do")
        || !strcmp(str, "break") || !strcmp(str, "continue")
        || !strcmp(str, "double") || !strcmp(str, "float")
        || !strcmp(str, "return") || !strcmp(str, "char")
        || !strcmp(str, "case") || !strcmp(str, "const")
        || !strcmp(str, "sizeof") || !strcmp(str, "long")
        || !strcmp(str, "short") || !strcmp(str, "typedef")
        || !strcmp(str, "switch") || !strcmp(str, "unsigned")
        || !strcmp(str, "void") || !strcmp(str, "static")
        || !strcmp(str, "struct") || !strcmp(str, "goto")
        || !strcmp(str, "union") || !strcmp(str, "volatile"))
        return (true);
    return (false);
}

int main()
{

 {  FILE *f1;
    int tab = 0;  // for counting number of tabs for proper indentation.
    int m = 0;    // to check if it is inside main.
    printf("\t\t\t\t\t\tC to Python Converter\n\n");
    printf("import math\n");
    printf("import sys\n");
    printf("import string\n");
    printf("if __name__== '__main__':\n");
    f1 = fopen("Cfile.txt","r");
    if(f1==NULL)
    {
        perror("Unable to open file.");
        exit(1);    //exiting due to error.
    }
    char chunk[200];
    while(fgets(chunk,sizeof(chunk),f1)!=NULL)   //getting a line of code from the C file.
    {
    char code[200]="";
    strcpy(code,chunk);
    int i;
    int count = 0;
    char str[200]=""; char key;
    for(i = 0; code[i]!='\0'; i++)
    {
        if(code[i]=='(')
        {
            count++;
            for(int j = 0; j<i; j++)   //reading characters before opening bracket to check the input keyword.
            {
                strncat(str,&code[j],1);
            }
        }
    }
    int length = i; // length of code used for removing line terminator where ever necessary.
    if(count==0)   //no opening bracket found in the input.
    {
        for(i = 0; code[i]!='\0'; i++)
        {
            if(code[i]==';')   //for keywords which don't contain brackets but has line terminator at the end.
            {
                for(int j = 0; j<i; j++)   //reading characters before line terminator to check the input keyword.
                {
                    strncat(str,&code[j],1);
                }
            }
        }
    }

    if(m==1)        //inside main.
    {
        if(str[0]=='\0')     //str is empty.
            strcpy(str,"else");  //putting else in an empty str. Exception case of else.
    }

    int ind = 0;
    char iden[15]="";
    for(int i=0;i<=8;i++)
    {
        strncat(iden,&code[i],1);
        if(!strcmp(iden,"int")||!strcmp(iden,"char")||!strcmp(iden,"float")||!strcmp(iden,"short")||    //declaration check.
           !strcmp(iden,"double")||!strcmp(iden,"long")||!strcmp(iden,"unsigned")||!strcmp(iden,"signed"))
        {
            ind++;
        }
        if(!(strcmp(iden,"int")))    //inside main function.
            m=1;
    }
    if(ind==1)      //if line has variable declaration.
        continue;
    if(code[0]=='{')    //tab count for proper indentation in python.
    {
        tab++;
        continue;
    }
    else if(code[0]=='}')
    {
        tab--;
        continue;
    }  //end of tab calculation

    key = isKeyword(str) ? 'Y' : 'N';   //checking if the given keyword is available in C or not.

    if(!strcmp(str, "printf")) //conversion of printf.
    {       int k,j;
            int ind = 6;  //to get index of commas before the variables in the print statement.
            strcpy(str,"print");
            for(j=6;code[j]!=';';j++) //adding the bracket part of print statement.
            {
                if(code[j]=='%')     //for replacing placeholders in print statement.
                {
                    for(k=ind;k<length;k++)
                    {

                        if(code[k]==',')
                        {
                            char s = '"';
                            char b = ')';
                            if(code[j-1]=='"' && code[j+2]=='"')   //checking limits on placeholders. (only one)
                            {
                                str[j-1] = '\b';  //for removing " after opening bracket.
                                strncat(str,&code[k+1],1);
                                strncat(str,&b,1);
                                code[j+2] = ';'; //terminating for loop.
                            }

                            else if(code[k+2]==')')  //if it is the last variable in the print statement.
                            {
                                strncat(str,&s,1);
                                strncat(str,&code[k],1);
                                strncat(str,&code[k+1],1);
                                strncat(str,&code[k+2],1);  //adding closing bracket at the end of the print statement.
                                k = length;
                                if(code[j+2]=='"')
                                    code[j+2] = ';';   //for terminating the for loop.
                            }
                            else
                            {
                                strncat(str,&s,1);
                                strncat(str,&code[k],1);
                                strncat(str,&code[k+1],1);
                                strncat(str,&code[k],1);
                                strncat(str,&s,1);
                                ind = k+2;               // change the index for the next comma in the print statement.
                                k = length;              // terminating for loop.
                            }
                        }

                    }
                    j++;
                }
                else if(code[j]=='"' && code[j+1]==',')  //finding index just before the variables for termination of loop.
                    code[j+1]=';';
                else
                    strncat(str,&code[j],1);
            }
            for(int i=1;str[i]!='\0';i++)   //to remove " " together in the str.
            {
                if(str[i-1]=='"' && str[i]=='"')
                {
                    str[i-1] = ' ';
                }
            }
            for(int i=1;str[i]!='\0';i++)   //to remove , , together in str.
            {
                if((str[i-3]==',' && str[i]==',')||(str[i-3]=='('&&str[i]==','))
                {
                    str[i] = '\b';
                }
            }
            for(int t=1;t<=tab;t++)  //printing number of tabs if blocks are present.
            {
                printf("\t");
            }
            printf("%s\n",str);
    }    //end of printf conversion.

    else if(!strcmp(str, "scanf"))  // conversion of scanf.
    {
        int pos,k,i;
        int pos1 = 0;
        for(pos=0;code[pos]!=',';pos++)    //for finding the position of comma in the code of scanf to get the variables.
        {
            pos1++;
        }
        for(i=0;code[i]!=';';i++)
        if(code[i]=='%')
        {
            if(code[i+1]=='d'||(code[i+1]=='l'&&code[i+2]=='d'))
                strcpy(str,"int(input())");  //if the input is an integer or long integer.
            else if(code[i+1]=='f'||(code[i+1]=='l'&&code[i+2]=='f'))
                strcpy(str,"float(input())");  //if the input is a float or double;
            else
                strcpy(str,"input()");
            char var[20]="";
            for(k=pos1+1;code[k]!=',';k++)
            {
                if(code[k]==')')
                {
                    code[k+1]=',';
                }
                else
                    strncat(var,&code[k],1);
            }
            pos1 = k;   //changing position of starting comma for the next variable name.
            for(int t=1;t<=tab;t++)  //printing number of tabs if blocks are present.
            {
                printf("\t");
            }
            printf("%s = %s\n",var,str);
            if(code[i+2]=='"'||code[i+3]=='"')
            {
                break;
            }
        }

    }                         //end of scanf conversion.

    else if(key == 'Y')  // if keyword is present.
    {
        if(!strcmp(str, "for")||!strcmp(str, "while")||!strcmp(str, "if")||!strcmp(str, "else")||!strcmp(str, "break")||
           !strcmp(str, "continue")||!strcmp(str, "return"))
        {
            strcpy(str,str);   // same for Python.
            if(!strcmp(str, "break")||!strcmp(str, "continue"))
            {
                for(int t=1;t<=tab;t++)  //printing number of tabs if blocks are present.
                {
                    printf("\t");
                }
                printf("%s\n",str);
            }
        }
        else if(!strcmp(str, "sizeof"))
        {
            strcpy(str,"sys.getsizeof");
            for(int j=6;code[j]!=';';j++)
            {
                strncat(str,&code[j],1);
            }
            for(int t=1;t<=tab;t++)  //printing number of tabs if blocks are present.
            {
                printf("\t");
            }
            printf("%s\n",str);
        }
        else if(!strcmp(str,"else if"))  //conversion of else if.
        {
            char s = ':';
            char b = ')';
            strcpy(str,"elif");
            for(int j=7;code[j]!=')';j++) //adding the bracket part of else if.
            {
                if((code[j]=='&' && code[j+1]=='&')||(code[j]=='|' && code[j+1]=='|'))  //changing && and ||
                {
                    char sign[3]=""; char and[3] = "and"; char or[2] = "or";
                    j++;
                    if(code[j]=='&')
                        strncat(sign,and,3);
                    else if(code[j]=='|')
                        strncat(sign,or,2);
                    strncat(str,sign,3);
                }
                else
                    strncat(str,&code[j],1);
            }
            strncat(str,&b,1);
            strncat(str,&s,1);
            for(int t=1;t<=tab;t++)  //printing number of tabs if blocks are present.
            {
                printf("\t");
            }
            printf("%s\n",str);
        }     // end of else if conversion.
        else
        {
            printf("WARNING : %s is not a Keyword available in Python.\n", str);
        }
    }
    else if(key=='N')  //if it is not a keyword in C.
    {
        for(int t=1;t<=tab;t++)  //printing number of tabs if blocks are present.
        {
            printf("\t");
        }
        if(strcmp(str,""))   //if str is not empty then only print, else don't go to the new line.
            printf("%s\n",str);
    }

char ch;
char s[10]="";    //initializing start,end and increment limit of for loop.
char e[10]="";
char inc[10]="";
int k=0;

if(!strcmp(str,"for"))   //conversion of for loop.
{
    for(int i=0; code[i]!='\0'; i++)
    {
        if(code[i]=='(')
        {
            ch = code[i+1];
        }
        else if(code[i]==';')
            k++;
        if(code[i]>='0' && code[i]<='9')   //ASCII checking for integers in the code to find limits.
        {
            if(k==1)
                strncat(e,&code[i],1);
            else if(k==2)                    //checking the incremental part of for loop.
            {
                if(code[i-1]=='-')          //adding minus sign if decrementing.
                {   char s = '-';
                    strncat(inc,&s,1);
                }
                strncat(inc,&code[i],1);
            }
            else
                strncat(s,&code[i],1);
        }
    }
    for(int t=1;t<=tab;t++)  //printing number of tabs if blocks are present.
    {
        printf("\t");
    }
    printf("for %c in range(%s,%s,%s):\n", ch,s,e,inc);
}     //end of for loop conversion.

else if(!strcmp(str,"while")||!strcmp(str,"if")||!strcmp(str,"else"))  //conversion of while loop, if and else.
{
    for(int i=0;code[i]!='\0';i++)
    {
        if((code[i]=='&' && code[i+1]=='&')||(code[i]=='|' && code[i+1]=='|'))   // for changing && and ||
        {
            if(code[i]=='&')
            {
                code[i]='a';
                code[i+1]='n';
                code[i+2]='d';
            }
            else if(code[i]=='|')
            {
                code[i]='o';
                code[i+1]='r';
            }
        }
    }

    code[length-1]=':';
    for(int t=1;t<=tab;t++)  //printing number of tabs if blocks are present.
    {
        printf("\t");
    }
    printf("%s\n",code);
}  //end of while loop conversion.

    }
 fclose(f1);  //closing the C code file.
 }
return(0);
}

