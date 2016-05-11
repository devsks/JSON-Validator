/*
* @brief JSON validator tool for terminal
* @author Santosh Kumar Shaw (devsks)
* @contact santosh79.cse@gmail.com
* @date  05-05-2016
* @quote "Code Like There's No Tommorow !"
**/
#include <bits/stdc++.h>
using namespace std;
int row, col;
void error(string msg1, string msg2="")
{
	cout<<"Error : "<<msg1<<" "<<msg2<<endl;
	cout<<"On line : "<<row<<" Column : "<<col<<endl;
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
		char ch,prev='^',*next="{",*valid="{[\":,]}";
		int line_no = 1, index = 0,obj=0,arr=0,i=0;
		row = col = 1;		
		stack <char> brac;
		while( ( ch = fgetc(json) ) != EOF )
		{
			// Character Index
			col++;
			
			// Scanning Valid Symbols.			
			if( strchr(valid,ch)  || (obj && strchr(next,ch)))
			{
				// Checking First Valid Character.
				if(strchr(next,ch) == NULL)
				{
					cout<<next<<" - "<<ch<<" - "<<prev<<endl;					
					error("Invalid Json");
				}				 
				else
				{
					switch(ch)
					{	
						case '{':	if(strchr("^{[:,",prev)==NULL)
									{
										cout<<next<<" { - "<<ch<<endl;										
										error("Invalid Json");
									}
									brac.push('{');
									prev = '{';
									obj=0;
									next="{\"";
									break;
						case '[':	brac.push('[');
									prev = '[';
									obj = 2;
									next="[{0123456789truefalse\"";
									break;						
						case '"':	if( prev != '\"')
										next = "\"";
							  		else
									 	if( !obj)
											next=":";
										else if( obj==1)
										{
											next=",}";
											obj = 0;
										}							  			
										else if( obj==2)
											next=",]";	
									prev = '"';
									break;
						case ':':	prev=':';
									next="[{0123456789truefalse\"";
									obj = 1;
									break;
						case ',':	if(obj == 2)
										next="0123456789truefalse\"";
									else if(prev=='}')
										next="{";
									else
										next="\"";
									prev=',';											
									break;
						case '}':	if(!brac.empty())	
									{
										char bracket = brac.top();
										if( bracket != '{' )
										{
											error("Braces does not matches!","Invalid JSON");
										}										
										brac.pop();
											next = "{,}]";
										prev = '}';											
									}
									else
									{
										error("Fatal error");
									}									
									break;
						case ']':	if(!brac.empty())	
									{
										char bracket = brac.top();
										if( bracket != '[' )
										{
											error("Braces does not matches!","Invalid JSON");
										}										
										brac.pop();	
										next = ",}";
										prev = ']';	
										obj = 0;										
									}
									else
									{
										error("Fatal error");
									}									
									break;		
					}

					
				}

			}
			else if(ch=='\n')
			{
				row++;
				col = 1;			
			}
			
		}
		if(brac.empty())
			cout<<"ALL OKK!";
		else
			error("Error");
		
	}
	
	return 0;
}

