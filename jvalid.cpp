/*
* @brief JSON validator tool for coommand line
* @author Santosh Kumar Shaw (devsks)
* @contact santosh79.cse@gmail.com
* @date  05-05-2016
* @quote "Code Like There's No Tommorow !"
**/
#include <bits/stdc++.h>
using namespace std;
void error(int)
int main ( int argc, char *argv[] )
{
	string json;
	// Checking for file or JSON String
	if(argc == 1 && ( cin>>json == 0 ))
	{
		// Stop Execution
		cout<<"Fatal Error : Usage  'jvalid filename' OR 'jvalid <filename'."<<endl;
		return 1;
	}
	else
	{
		// Open File for Input JSON String
		FILE *f = fopen(argv[1],"r");
		if( f == NULL)
		{
			cout<<"Fatal Error : Unable to open "<<argv[i]<<"\nTry Again.";
			return 1;
		}
		gets(json,);
	}

	return 0;
}
