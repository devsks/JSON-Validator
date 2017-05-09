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
// Global variables
int row, col;

void error(string msg1, string msg2="")
{
	cout << "Error : " << msg1 << " " << msg2 << endl;
	if (row)
		cout << "On line : " << row << " Column : " << col << endl;
	exit(EXIT_FAILURE);
}
void check(char*);
int main(int argc, char *argv[])
{
	// Checking for file or JSON String
	if (argc == 1)
		// Stop Execution
		error("Usage  'jvalid filename'","Retry !");
	else
		check(argv[1]);
	return 0;
}
void check(char *fileName)
{
		// Open File for Input JSON String
		FILE *json = fopen(fileName, "r");
		if (json == NULL)
		{
			error("Unable to open", fileName);
		}
				
		char symbol, bracket;
		int objectMode = 0;
		bool first_char = false, decimal = false;		
		string state[] = {" \n\t", ".0123456789,]}", "0123456789,]}", 
						"[{+-.0123456789tfn\"", ",}", ",]", 
						"0123456789", "r", "u", 
						"e", "a", "l", 
						"\"", "s", "n", 
						"nothing",":","{",
						"}","[","]",
						",",".","t",
						"f","+","-"};
		string valid_state = "{[\":,+-.0123456789truefalsenull]}", next_state = "{", prev_state; 
		row = 1;		
		stack <char> brac;		

		while(( symbol = fgetc(json) ) != EOF)
		{
		
			col++;

			// Object starts with '{'
			if ( state[0].find(symbol) == string::npos )
			{
				
				if (!first_char && symbol != '{')
					error("First character should be { ", "Check the expression");
				else
					first_char = true;			
				// When object is complete and we encounter extra characters.			
				if (next_state == state[15])
					error("Extra Characters at the end of the object .");		
			}
			// Validating expressions
			if ( next_state.find(symbol) == string::npos && (prev_state != state[12]) && ( next_state != state[12] ) && state[0].find(symbol)== string::npos )
			{
				cout << "\nExpected '" << next_state << "' not '" << symbol << "'\n";
				error("Invalid Expression");			
			}
			else if (next_state.find(symbol) != string::npos)
			{
				// Checking First Valid Character.
				if (symbol >= '0' && symbol <= '9')
				{
					prev_state = symbol;

					bracket = brac.top();
					if (!decimal)
						next_state = state[1];
					else
						next_state = state[1];
				}
				else					
				{
					switch(symbol)
					{	
						case '{':	brac.push('{');
									prev_state = state[17];
									// After '{' we expect "key"  only									
									next_state = state[12];
									objectMode = 0;
									break;

						case '[':	brac.push('[');
									prev_state = state[19];
									// Array elements can be a 'String', 'Number', 'boolean', 'null',' { object } ' or '[ array ]' only.
									next_state = state[3];
									break;	
					
						case '"':	if (prev_state != state[12])
										// We need "key" or "value" as String										
										next_state = state[12];
							  		else
									{ 	
										bracket = brac.top();
										// We need ':' to make a pair.
										if (bracket == '{' && !objectMode)
											next_state = state[16];
										// Check for other {object}'s or end of  {object}
										else if (bracket == '{' && objectMode)
											next_state = state[4];
										// Check for other elements or end of the array
										else
											next_state = state[5];
									}
									prev_state = state[12];
									break;

						case ':':	prev_state = state[16];
									// After ':' we expect a 'String', 'Number', 'boolean', 'null',' { object } ' or '[ array ]' only.
									next_state = state[3];
									objectMode = 1;
									break;

						case ',':	prev_state = state[21];
									decimal = false;
									bracket = brac.top();
									if (bracket == '{')
									{
										// After ',' we expect a { object }  only.
										next_state = state[12];
										objectMode = 0;
									}
									else
										// After ',' we expect a 'String', 'Number', 'boolean', 'null',' { object } ' or '[ array ]' only.	
										next_state = state[3];
									break;

						case '}':	if (!brac.empty())	
									{
										bracket = brac.top();
										if (bracket != '{')
										{
											cout<<bracket;											
											error("Braces does not matches!","Invalid JSON");
										}										

										brac.pop();
										if (brac.empty())
											next_state = "nothing";
										else
										{
											bracket = brac.top();
											if (bracket == '{')
												next_state = state[4];
											else
												next_state = state[5];										
										}
		
										prev_state = state[18];											
									}
									else
									{
										error("Extra '}'");
									}									
									break;

						case ']':	if (!brac.empty())	
									{
										bracket = brac.top();
										if (bracket != '[')
										{
											cout<<bracket;
											error("Braces does not matches!","Invalid JSON");
										}										
										brac.pop();
										if (brac.empty())
											next_state = "nothing";
										else
										{
											bracket = brac.top();
											if (bracket == '{')
												next_state = state[4];
											else
												next_state = state[5];										
										}	
										prev_state = state[20];	
									}
									else
									{
										error("Extra ']' ");
									}									
									break;

					case '-':		prev_state = state[26];
									next_state = state[6];
									break;

					case '+':		prev_state = state[25];
									next_state = state[6];
									break;

					case '.':		prev_state = state[22];
									decimal = true;
									next_state = state[6];
									break;

					case 't':		prev_state = state[23];
									next_state = state[7];
									break;

					case 'r':		prev_state = state[7];
									next_state = state[8];
									break;	

									// It can be 'trUe' or 'nUll'.
					case 'u':		if (prev_state == state[14])
										next_state = state[11];
									else
										next_state = state[9];
									prev_state = state[8];
									break;	

					case 'e':		prev_state = state[9];
									bracket = brac.top();
									if (bracket == '{')
										next_state = state[4];
									else
										next_state = state[5];
									break;

					case 'f':		prev_state = state[24];
									next_state = state[10];
									break;

					case 'a':		prev_state = state[10];
									next_state = state[11];
									break;

									/* It can be 'faLse' or 'nuLL'.*/
					case 'l':		if (prev_state == state[10])	
										next_state = state[13];
									else if (prev_state == state[8])
										next_state = state[11];
									else
									{
										bracket = brac.top();
										if (bracket == '{')
											next_state = state[4];
										else
											next_state = state[5];
									}									
									prev_state = state[11];									
									break;

					case 's':		prev_state = state[13];
									next_state = state[9];
									break;

					case 'n':		prev_state = state[14];
									next_state = state[8];
									break;
					}
			}
			
			//cout << prev_state << " "; // For debugging purpose
					
			}
			else if (symbol == '\n')
			{
				row++;
				col = 0;			
			}
			
		}
		if (brac.empty())
			cout << "ALL OKK!";
		else
			error("Error");
}	

