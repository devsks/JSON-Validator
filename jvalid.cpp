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
		char ch,prev='^',*next="{",*valid="{[\":,0123456789truefalsenull]}";
		int line_no = 1, index = 0,obj=0,arr=0,i=0;
		row = 1 , col = 0;		
		stack <char> brac;		
		while( ( ch = fgetc(json) ) != EOF )
		{
		
			col++;
			if( prev !='"' && strchr(valid,ch) && strchr(next,ch)==NULL )
			{
				cout<<next<<" - "<<prev<<" - "<<ch<<endl;
				error("Not \" ");			
			}
			// Scanning Valid Symbols.			
			else if( strchr(next,ch) )
			{
				// Checking First Valid Character.
				if(ch>='0' && ch<='9')
				{
					prev = ch;
					next = "0123456789,}]";
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
									if(obj!=2)									
										obj=0;
									next="\"";
									break;
						case '[':	brac.push('[');
									prev = '[';
									obj = 2;
									next="[{0123456789truefalsenull\"";
									break;						
						case '"':	
									if( prev != '"')
										next = "\"";
							  		else
									{ 	
										next = ":,]}";										
										/*if( !obj )
											next=":";
										else
										{
											next=",]}";
										}*/
									}
									prev = '"';
									break;
						case ':':	prev=':';
									next="[{0123456789truefalsenull\"";
									obj = 1;
									break;
						case ',':	next="0123456789truefalsenull\"[{";
									prev=',';											
									break;
						case '}':	if(!brac.empty())	
									{
										char bracket = brac.top();
										if( bracket != '{' )
										{
											cout<<bracket;											
											error("Braces does not matches!","Invalid JSON");
										}										
										brac.pop();
										next = "{,}]";
										prev = '}';											
									}
									else
									{
										error("} error");
									}									
									break;
						case ']':	if(!brac.empty())	
									{
										char bracket = brac.top();
										if( bracket != '[' )
										{
											cout<<bracket;
											error("Braces does not matches!","Invalid JSON");
										}										
										brac.pop();
											
										next = ",]}";
										prev = ']';	
																				
									}
									else
									{
										error("] error");
									}									
									break;
					case 't':		prev='t';
									next="r";
									break;
					case 'r':		prev='r';
									next="u";
									break;	
					case 'u':		if(prev=='n')
										next="l";
									else
										next="e";
									prev='u';
									break;	
					case 'e':		prev='e';
									next=",]}";
									break;
					case 'f':		prev='f';
									next="a";
									break;
					case 'a':		prev='a';
									next="l";
									break;
					case 'l':		if(prev=='a')	
										next="s";
									else if(prev=='u')
										next="l";
									else
										next=",]}";
									prev='l';									
									break;
					case 's':		prev='s';
									next="e";
									break;
					case 'n':		prev='n';
									next="u";
									break;
					}
			}
			cout<<prev<<" ";
					
			}
			else if(ch=='\n')
			{
				row++;
				col = 0;			
			}
			
		}
		if(brac.empty())
			cout<<"ALL OKK!";
		else
			error("Error");
		
	}
	
	return 0;
}

