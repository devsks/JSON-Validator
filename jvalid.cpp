/*
* @brief JSON validator tool for terminal
* @author Santosh Kumar Shaw (devsks)
* @contact santosh79.cse@gmail.com
* @date  05-05-2016
* @quote "Code Like There's No Tommorow !"
* @Dedicated to Debabrata Acharya , CTO - Oncalve Systems.
**/
#include <bits/stdc++.h>
using namespace std;

int row, col;

void error(string msg1, string msg2="")
{
	cout<<"Error : "<<msg1<<" "<<msg2<<endl;
	if(row)
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
				
		char ch, prev_state = '^', *next_state = "{", *valid_state = "{[\":,+-.0123456789truefalsenull]}",bracket;
				
		int line_no = 1, obj = 0;
		bool first_char = false, decimal = false;
		
		row = 1 , col = 0;		
		stack <char> brac;		

		while( ( ch = fgetc(json) ) != EOF )
		{
		
			col++;
			// Object starts with '{'
			if( !strchr(" \n\t",ch) && !first_char )
			{
				first_char = true;
				if( ch != '{' )
					error("First character should be { ", "Check the expression");
			}
			// When object is complete and we encounter extra characters.			
			if( !strchr(" \n\t", ch) && strcmp(next_state,"nothing")==0)
			{
				error("Extra Characters at the end of the object .");		
			}
			// Validating expressions
			if( strchr(next_state,ch) == NULL  && ( prev_state!='"' && strcmp(next_state,"\"")) && !strchr(" \n\t",ch) )
			{
				cout<<"\nExpected '"<<next_state<<"' not '"<<ch<<"'\n";
				error("Invalid Expression");			
			}
			else if( strchr(next_state,ch) )
			{
				// Checking First Valid Character.
				if(ch>='0' && ch<='9')
				{
					prev_state = ch;
					
					bracket = brac.top();
					if( !decimal)
						next_state = ".0123456789,]}";
					else
						next_state = "0123456789,]}";
				}
				else					
				{
					switch(ch)
					{	
						case '{':	brac.push('{');
									prev_state = '{';
									// After '{' we expect "key"  only									
									next_state = "\"";
									obj = 0;
									break;

						case '[':	brac.push('[');
									prev_state = '[';
									// Array elements can be a 'String', 'Number', 'boolean', 'null',' { object } ' or '[ array ]' only.
									next_state = "[{+-.0123456789tfn\"";
									break;	
					
						case '"':	if( prev_state != '"')
										// We need "key" or "value" as String										
										next_state = "\"";
							  		else
									{ 	
										bracket = brac.top();
										// We need ':' to make a pair.
										if( bracket == '{' && !obj)
											next_state=":";
										// Check for other {object}'s or end of  {object}
										else if( bracket == '{' && obj )
											next_state=",}";
										// Check for other elements or end of the array
										else
											next_state=",]";
									}
									prev_state = '"';
									break;

						case ':':	prev_state=':';
									// After ':' we expect a 'String', 'Number', 'boolean', 'null',' { object } ' or '[ array ]' only.
									next_state = "[{+-.0123456789tfn\"";
									obj = 1;
									break;

						case ',':	prev_state = ',';
									decimal = false;
									bracket = brac.top();
									if( bracket == '{' )
									{
										// After ',' we expect a { object }  only.
										next_state="\"";
										obj = 0;
									}
									else
										// After ',' we expect a 'String', 'Number', 'boolean', 'null',' { object } ' or '[ array ]' only.	
										next_state = "[{+-.0123456789tfn\"";
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
											next_state = "nothing";
										else
										{
											bracket = brac.top();
											if(bracket == '{')
												next_state = ",}";
											else
												next_state = ",]";											
										}
		
										prev_state = '}';											
									}
									else
									{
										error("Extra '}'");
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
											next_state = "nothing";
										else
										{
											bracket = brac.top();
											if(bracket == '{')
												next_state = ",}";
											else
												next_state = ",]";											
										}	
										prev_state = ']';	
									}
									else
									{
										error("Extra ']' ");
									}									
									break;
					case '-':		prev_state = '-';
									next_state = "0123456789";
									break;

					case '+':		prev_state = '+';
									next_state = "0123456789";
									break;

					case '.':		prev_state = '.';
									decimal = true;
									next_state = "0123456789";
									break;

					case 't':		prev_state = 't';
									next_state = "r";
									break;

					case 'r':		prev_state = 'r';
									next_state = "u";
									break;	

									// It can be 'trUe' or 'nUll'.
					case 'u':		if( prev_state == 'n')
										next_state = "l";
									else
										next_state = "e";
									prev_state = 'u';
									break;	

					case 'e':		prev_state = 'e';
									bracket = brac.top();
									if( bracket == '{' )
										next_state = ",}";
									else
										next_state = ",]";
									break;

					case 'f':		prev_state = 'f';
									next_state = "a";
									break;

					case 'a':		prev_state = 'a';
									next_state = "l";
									break;

									// It can be 'faLse' or 'nuLL'.
					case 'l':		if(prev_state == 'a')	
										next_state = "s";
									else if( prev_state == 'u')
										next_state = "l";
									else
									{
										bracket = brac.top();
										if( bracket == '{' )
											next_state = ",}";
										else
											next_state = ",]";
									}									
									prev_state = 'l';									
									break;

					case 's':		prev_state = 's';
									next_state = "e";
									break;

					case 'n':		prev_state = 'n';
									next_state = "u";
									break;
					}
			}
			
			//cout<<prev_state<<" "; // For debugging purpose
					
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

