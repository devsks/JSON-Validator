/*
* @brief JSON validator tool for terminal
* @author Santosh Kumar Shaw (devsks)
* @contact santosh79.cse@gmail.com
* @date  05-05-2016
* @quote "Code Like There's No Tommorow !"
* Dedicated to Debabrata Acharya , CTO - Oncalve Systems.
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
		char ch, prev = '^', *next = "{", *valid = "{[\":,0123456789truefalsenull]}",bracket;
		int line_no = 1, obj = 0;
		bool first_char = false;
		row = 1 , col = 0;		
		stack <char> brac;		
		while( ( ch = fgetc(json) ) != EOF )
		{
		
			col++;
			// Object starts with '{'
			if( !strchr(" \n\t",ch) && !first_char )
			{
				first_char = true;
				if(ch!='{')
					error("First character should be { ", "Check the expression");
			}
			// When object in complete			
			if( !strchr(" \n\t",ch) && strcmp(next,"nothing")==0)
			{
				error("Extra Characters at the end of the object .");		
			}
			// Validating expressions
			if( strchr(next,ch) == NULL  && ( prev!='"' && strcmp(next,"\""))  && strchr(valid,ch)  )
			{
				cout<<"\nExpected '"<<next<<"' not '"<<ch<<"'\n";
				error("Invalid Expression");			
			}
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
						case '{':	brac.push('{');
									prev = '{';
									// After '{' we expect "key"  only									
									next="\"";
									obj = 0;
									break;
						case '[':	brac.push('[');
									prev = '[';
									// Array elements can be a 'String', 'Number', 'boolean', 'null',' { object } ' or '[ array ]' only.
									next="[{0123456789tfn\"";
									break;						
						case '"':	if( prev != '"')
										// We need "key" or "value" as String										
										next = "\"";
							  		else
									{ 	
										bracket = brac.top();
										// We need ':' to make a pair.
										if( bracket == '{' && !obj)
											next=":";
										else if( bracket == '{' && obj )
											next=",}";
										else
											next=",]";
									}
									prev = '"';
									break;
						case ':':	prev=':';
									// After ':' we expect a 'String', 'Number', 'boolean', 'null',' { object } ' or '[ array ]' only.
									next = "[{0123456789tfn\"";
									obj = 1;
									break;
						case ',':	prev = ',';
									bracket = brac.top();
									if( bracket == '{' )
									{
										// After ',' we expect a { object }  only.
										next="\"";
										obj = 0;
									}
									else
										// After ',' we expect a 'String', 'Number', 'boolean', 'null',' { object } ' or '[ array ]' only.	
										next = "[{0123456789tfn\"";
									break;
						case '}':	if(!brac.empty())	
									{
										bracket = brac.top();
										if( bracket != '{' )
										{
											cout<<bracket;											
											error("Braces does not matches!","Invalid JSON");
										}										

										brac.pop();
										if(brac.empty())
											next = "nothing";
										else
										{
											bracket = brac.top();
											if(bracket == '{')
												next = ",}";
											else
												next = ",]";											
										}
		
										prev = '}';											
									}
									else
									{
										error(" } error");
									}									
									break;
						case ']':	if(!brac.empty())	
									{
										bracket = brac.top();
										if( bracket != '[' )
										{
											cout<<bracket;
											error("Braces does not matches!","Invalid JSON");
										}										
										brac.pop();
										if(brac.empty())
											next = "nothing";
										else
										{
											bracket = brac.top();
											if(bracket == '{')
												next = ",}";
											else
												next = ",]";											
										}	
										prev = ']';	
									}
									else
									{
										error("] error");
									}									
									break;
					case 't':		prev = 't';
									next = "r";
									break;
					case 'r':		prev = 'r';
									next = "u";
									break;	
									// It can be 'trUe' or 'nUll'.
					case 'u':		if( prev == 'n')
										next = "l";
									else
										next = "e";
									prev = 'u';
									break;	
					case 'e':		prev = 'e';
									bracket = brac.top();
									if( bracket == '{' )
										next = ",}";
									else
										next = ",]";
									break;
					case 'f':		prev = 'f';
									next = "a";
									break;
					case 'a':		prev = 'a';
									next = "l";
									break;
									// It can be 'faLse' or 'nuLL'.
					case 'l':		if(prev == 'a')	
										next = "s";
									else if( prev == 'u')
										next = "l";
									else
									{
										bracket = brac.top();
										if( bracket == '{' )
											next = ",}";
										else
											next = ",]";
									}									
									prev = 'l';									
									break;
					case 's':		prev = 's';
									next = "e";
									break;
					case 'n':		prev = 'n';
									next = "u";
									break;
					}
			}
			
			//cout<<prev<<" "; // For debugging purpose
					
			}
			else if( ch == '\n')
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

