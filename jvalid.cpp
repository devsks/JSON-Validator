/*
* @brief JSON validator tool for coommand line
* @author Santosh Kumar Shaw (devsks)
* @contact santosh79.cse@gmail.com
* @date  05-05-2016
* @quote "Code Like There's No Tommorow !"
**/
#include <bits/stdc++.h>
using namespace std;

void error(string msg1, string msg2="")
{
	cout<<"Error : "<<msg1<<" "<<msg2<<endl;
	exit(EXIT_FAILURE);
}

int main ( int argc, char *argv[] )
{
	// Checking for file or JSON String
	if(argc == 1)
	{
		// Stop Execution
		error("Usage  'jvalid filename'","Retry !");
	}
	else
	{
		// Open File for Input JSON String
		FILE *json = fopen(argv[1],"r");
		if( json == NULL)
		{
			error("Unable to open",argv[1]);
		}
		char booli[500];		
		char ch,prev='^',*next,*valid="{[\":,]}";
		int line_no = 1, index = 0,obj=0,arr=0,i=0;
		stack <char> brac;
		while( ( ch = fgetc(json) ) != EOF )
		{
			
			// Scanning Valid Symbols.			
			if( strchr(valid,ch)  )
			{
				// Checking First Valid Character.
				if( prev == '^' && ch != '{')
					error("Invalid Json1");
				else if(prev == '^' || ch == '{')
				{
					brac.puch('{');					
					prev = '{';
					next = "{\"";
				}
				else if(strchr(next,ch) == NULL)
					error("Invalid Json");
				else
				{
					switch(ch)
					{	
						case '\"':	
									if(prev!='\"')
										next="\"";
							  		else if(prev=='\"' && !obj)
										next=":";
									else if(prev=='\"' && obj)
										next=",}";
							  		else if(prev=='\"' && arr)
										next=",";	
									prev = '\"';
									break;
						case ':':	prev=" : ";
									obj = 1;
								
					}

					
				}
			}

		}
		else if(prev==':')
		{
			booli[i++]=ch;
			booli[i]='\0';
			if( !strcmp(booli,"true") || !strcmp(booli,"false") || atoi(booli))
			{	prev='-';
			}
		}
		
	}
	
	return 0;
}

