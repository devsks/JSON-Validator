/*
* @brief JSON validator tool for coommand line
* @author Santosh Kumar Shaw (devsks)
* @contact santosh79.cse@gmail.com
* @date  05-05-2016
* @quote "Code Like There's No Tommorow !"
**/
#include <bits/stdc++.h>
using namespace std;

void error(string, string );

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
		char ch;
		int line_no = 1, index = 0;
		stack <char> brac;
		while( ( ch = fgetc(json) ) != EOF )
		{
			index++;
			//cout<<ch;
			if( ch == '{'  )
				brac.push(ch);
			else if( ch == '}' )
			{
				if( !brac.empty() )
					brac.pop();
				else
				{
					cout<<"Line no : "<<line_no<<" Index : "<<index<<endl;
					error("Check This pair Bracket","Retry");
				}	
			}	
			else if( ch == '\n')
			{
				line_no++;
				index = 0;
			}	
			
		}
		if(brac.empty())
			cout<<"All OK !"<<endl;
		else
			cout<<"Unequal Pairs of brackets ! \n";	
	}
	
	return 0;
}
void error(string msg1, string msg2="")
{
	cout<<"Error : "<<msg1<<" "<<msg2<<endl;
	exit(EXIT_FAILURE);
}
