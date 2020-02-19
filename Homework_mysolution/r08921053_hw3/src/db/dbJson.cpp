/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include "dbJson.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream&
operator << (ostream& os, const DBJsonElem& j)
{
   os << "\"" << j._key << "\" : " << j._value;
   return os;
}

istream& operator >> (istream& is, DBJson& j)
{
   // TODO: to read in data from Json file and store them in a DB 
   // - You can assume the input file is with correct JSON file format
   // - NO NEED to handle error file format
   assert(j._obj.empty());
   string input;
   string temp_key = {};
   int    temp_value = 0;
   while( is >> input){
   	   if(input[0]=='"'){
   	   	    int leng = input.length(); 
			input=input.substr(1, leng-2);
			temp_key = input;
		  }
	   else if(input[0]=='-'||input[0]=='0'||input[0]=='1'||input[0]=='2'||input[0]=='3'||input[0]=='4'||input[0]=='5'||input[0]=='6'||input[0]=='7'||input[0]=='8'||input[0]=='9'){
	   	    int leng = input.length();
			if(input[leng-1]==','){
				input=input.substr(0, leng-1);
			}
			int transnumber=stoi(input);
			temp_value = transnumber;
			DBJsonElem temp(temp_key, temp_value);
			j._obj.push_back(temp); 
	   } 
           else if(input[0]== '}') break;
	   else if( input[0]=='{' || input[0]==',' || input[0]==':'){}
	   else{
		   //Here is a mistake
		   cout << "Sometime wrong in istream& operator >>" << endl;
	   }
   }
   j.readed = true;
   return is;
}

ostream& operator << (ostream& os, const DBJson& j)
{
   // TODO
   os << '{' << endl;
   int size = j._obj.size();
   if(size>0){
   	   for(int i=0; i<size-1; i++){
   	   	    os << " ";
		    os << "\"" << j._obj[i].key() << "\" : " << j._obj[i].value() << "," << endl;
		  } 
	   os << " ";
	   os << "\"" << j._obj[size-1].key() << "\" : " << j._obj[size-1].value() << endl;
   }
   os << '}' << endl; 
   return os;
}

/**********************************************/
/*   Member Functions for class DBJsonElem    */
/***i*******************************************/
/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/
void
DBJson::reset()
{
   // TODO
   _obj.clear();
}

// return false if key is repeated
bool
DBJson::add(const DBJsonElem& elm)
{
   // TODO
   int size = _obj.size();
   for(int i=0; i<size; i++){
   	   if(elm.key() == _obj[i].key()) return false; 
   }
   _obj.push_back(elm);
   return true;
}

// return NAN if DBJson is empty
float
DBJson::ave() const
{
   // TODO
   if(_obj.empty()==1) return NAN;
   else{
   	   int size = _obj.size();
	   double sum=0.0;
   	   for(int i=0; i<size; i++){
   	   	    sum=sum+_obj[i].value();
		  }
	   double ans = sum / ((double) size);
	   return ans;
   }
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::max(size_t& idx) const
{
   // TODO
   if(_obj.empty()==0){
   	   int size = _obj.size();
   	   int index = 0;
   	   for(int i=0; i<size; i++){
   	   	    if(_obj[index].value() < _obj[i].value()) index = i;
		  }
	   idx = index;
	   return _obj[idx].value();
   }
   idx = size();
   int maxN = INT_MIN;
   return  maxN;
}

// If DBJson is empty, set idx to size() and return INT_MAX
int
DBJson::min(size_t& idx) const
{
   // TODO
   if(_obj.empty()==0){
   	   int size = _obj.size();
	   int index = 0;
	   for(int i=0; i<size; i++){
	   	   if(_obj[index].value() > _obj[i].value())   index = i;
	   } 
	   idx = index;
	   return _obj[idx].value();
	   
   }
   idx = size();
   int minN = INT_MAX;
   return  minN;
}

void
DBJson::sort(const DBSortKey& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void
DBJson::sort(const DBSortValue& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

// return 0 if empty
int
DBJson::sum() const
{
   // TODO
   if(_obj.empty()==0){
   	   int size = _obj.size();
   	   int sum = 0;
		  for(int i=0; i<size; i++){
   	   	    sum=sum+_obj[i].value();
		  }
		  return sum;
   }
   int s = 0;
   return s;
}
