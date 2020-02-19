/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include "p2Json.h"
#include <fstream>

using namespace std;

// Implement member functions of class Row and Table here
/*To do the Json.read() following by Ric*/
bool
Json::read(const string& jsonFile)
{ 
	/*To readin the file word by word*/
 	fstream fin;
	fin.open(jsonFile, ios::in);
	string input;
	JsonElem temp;
	while(fin >> input){               
		/*Using the first character of words to distinguish the situation*/
		/*The case: "key" */
		if(input[0]=='"'){
			input=input.substr(1, input.length()-2);
			temp.eatkey(input);
		} 
		/*The case: negative integer(probably , here)*/
		/*else if(input[0]=='-'){
			if(input[input.length()-1]==','){
				input=input.substr(1, input.length()-2);
			}
			int transnumber=-1*stoi(input);
			temp.eatvalue(transnumber);
			_obj.push_back(temp);
		}*/ 
		/*The case: positive integer(probably , here)*/
		else if(input[0]=='-'||input[0]=='0'||input[0]=='1'||input[0]=='2'||input[0]=='3'||input[0]=='4'||input[0]=='5'||input[0]=='6'||input[0]=='7'||input[0]=='8'||input[0]=='9'){
			if(input[input.length()-1]==','){
				input=input.substr(0, input.length()-1);
			}
			int transnumber=stoi(input);
			temp.eatvalue(transnumber);
			_obj.push_back(temp);
		}
		/*The case: non-important character*/
		else if( input[0]=='{' || input[0]=='}' || input[0]==',' || input[0]==':'){}
		else{
			cout << "There is a mistake on Json-read-while with input= " << input << endl;
		}
	}
   return true; // TODO
}

ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}

