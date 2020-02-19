/****************************************************************************
  FileName     [ p2Json.h]
  PackageName  [ p2 ]
  Synopsis     [ Header file for class Json JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#ifndef P2_JSON_H
#define P2_JSON_H

#include <vector>
#include <string>
#include <unordered_set>
#include <iostream>
#include <iomanip>

using namespace std;

class JsonElem
{
public:
   // TODO: define constructor & member functions on your own
   JsonElem() {}
   JsonElem(const string& k, int v): _key(k), _value(v) {}
   
   /*To give the JsonElem._value*/
   int givevalue(){
   	    return _value;
   }   
   
   /*To print the JsonELem._key and JsonElem._value*/
   void print(){
   	    cout << " \"" << _key << "\"" << " : " << _value;
   }   
   
   /* To read in or eat a key*/
   void eatkey(const string& s) {_key=s;}
   
   /* To read in or eat a value*/
   void eatvalue(int v) {_value=v;}

   friend ostream& operator << (ostream&, const JsonElem&);

private:
   string  _key;   // DO NOT change this definition. Use it to store key.
   int     _value; // DO NOT change this definition. Use it to store value.
};

class Json
{
public:
   // TODO: define constructor & member functions on your own
   bool read(const string&);
   
   Json() {}
   
   /*To print the content of _obj[0], ..., _obj[n]*/
   void print() {
   	   cout << '{' << endl;
   	   if(_obj.size()>0){
   	   	   for(int i=0; i<_obj.size()-1; i++){
                            cout << " ";
   	   	   	    _obj[i].print();
   	   	   	    cout << "," << endl; 
			}
				cout << " ";
                                _obj[_obj.size()-1].print();
                                cout << endl;		 
		}
		  cout << '}' << endl;	    
   }
   
   /*To get the summation of all data*/
   void sum() {
   	   int answer=0;
   	   if(_obj.size()>0){
   	   	   for(int i=0; i<_obj.size(); i++){
   	   	   	    answer=answer+_obj[i].givevalue();
			}
			cout << "The summation of the values is: " << answer << "." << endl; 
		}
		else cout << "Error: No element found!!" << endl;
   }
   
   /*To get the average of all data*/
   void ave() {
   	    double answer=0;
   	    if(_obj.size()>0){
   	   	    for(int i=0; i<_obj.size(); i++){
   	   	   	    answer=answer+_obj[i].givevalue();
			}
			answer=answer/_obj.size();
			cout << "The average of the values is: " << fixed << setprecision(1) << answer << "." << endl;
		}
		else cout << "Error: No element found!!" << endl;
    }
    
    /*To find the maximum value of all data*/
    void max() {
    	int temp=0;
    	if(_obj.size()>0){
    		for(int i=0; i<_obj.size(); i++){
    			if( _obj[i].givevalue()>_obj[temp].givevalue() ) temp=i;
			}
			cout << "The maximum element is: {";
			_obj[temp].print();
			cout << " }." << endl;
		}
		else cout << "Error: No element found!!" << endl;
	}
	
	/*To find the minimum value of all data*/
	void min() {
		int temp=0;
		if(_obj.size()>0){
			for(int i=0; i<_obj.size(); i++){
				if( _obj[i].givevalue()<_obj[temp].givevalue() ) temp=i;
			}
			cout << "The minimum element is: {";
			_obj[temp].print();
			cout << " }." << endl;
		}
		else cout << "Error: No element found!!" << endl;
	}
	
	/*To add a JsonElem data into Json file*/
	void add(const string& key, int value){
		JsonElem temp( key, value);
		_obj.push_back(temp);
	}

private:
   vector<JsonElem>       _obj;  // DO NOT change this definition.
                                 // Use it to store JSON elements.
};

#endif // P2_TABLE_H
