/****************************************************************************
  FileName     [ p2Main.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define main() function ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2016-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <string>
#include "p2Json.h"

using namespace std;

int main()
{
    Json json;

    // Read in the csv file. Do NOT change this part of code.
    string jsonFile;
    cout << "Please enter the file name: ";
    cin >> jsonFile;
    if (json.read(jsonFile))
       cout << "File \"" << jsonFile << "\" was read in successfully." << endl;
    else {
       cerr << "Failed to read in file \"" << jsonFile << "\"!" << endl;
       exit(-1); // jsonFile does not exist.
    }

    // TODO read and execute commands
    // cout << "Enter command: ";
    string input;
    while (true) {
        // cout << "Enter command: ";
        cout << "Enter command: ";
        cin >> input;
        if(input=="PRINT"){
      	    json.print();
      	}
      	else if(input=="SUM"){
      		json.sum();
		} 
		else if(input=="AVE"){
			json.ave();
		}
		else if(input=="MAX"){
			json.max();
		}
		else if(input=="MIN"){
			json.min();
		}
		else if(input=="ADD"){
			string key;
			int value;
			cin >> key >> value;
			json.add(key, value);
		}
		else if(input=="EXIT"){
			break;
		}
		else{
			cout << "There are something wrong about the input commands!" << endl;
		}
   }
   return 1;
}
