/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
CirGate*
CirMgr::getGate(unsigned id) const
{
	map<size_t, CirGate*>::const_iterator iter = _gateList.find(id);
	if(iter==_gateList.end()) return 0;
	else                      return iter->second;
	
}

bool
CirMgr::readCircuit(const string& fileName)
{
        reset();
        ifstream fin(fileName);
        readHeader(fin);
        readInput(fin);
        readOutput(fin);
        readAig(fin);
        readSym(fin);
        CirGate* const0 = new CirGate((size_t)0, 'z');
        _gateList.insert(pair<size_t, CirGate*>((size_t)0, const0));
        connect();
        genDFSList();
        
        fin.close();
        return true;
}


bool
CirMgr::readHeader(ifstream& fin){
	++lineNo;
	string Header;
	fin >> Header >> M >> I >> L >> O >> A;
	return true;
}

bool 
CirMgr::readInput(ifstream& fin){
	for(size_t i=0; i<I; ++i){
		++lineNo;
		size_t input;
		fin >> input; 
		input = input/2;
		CirGate* pi = new CirGate(input, 'i', lineNo);
		_piList.push_back(pi);
		_gateList.insert(pair<size_t, CirGate*>(input, pi));
	} 	
	return true;
}

bool
CirMgr::readOutput(ifstream& fin){
	for(size_t i=0; i<O; ++i){
		++lineNo;
		size_t output;
		fin >> output;
		bool sgn = output%2;
		output   = output/2;
		size_t   ID = M+1+i;
		CirGate* fan0 = (CirGate*)output;
		CirGate* po = new CirGate(ID, fan0, (CirGate*)((size_t)-1), sgn, 0, 'o', lineNo);
		_poList.push_back(po);
		_gateList.insert(pair<size_t, CirGate*>(ID, po));
	}
	return 0;
} 

bool
CirMgr::readAig(ifstream& fin){
	for(size_t i=0; i<A; ++i){
		++lineNo;
		size_t tmpA, tmpB, tmpC;
		fin >> tmpA >> tmpB >> tmpC;
		
		bool invB = tmpB%2, invC = tmpC%2;
		tmpA = tmpA/2, tmpB = tmpB/2, tmpC = tmpC/2;
		CirGate* fan0 = (CirGate*)tmpB;
		CirGate* fan1 = (CirGate*)tmpC;
        CirGate* pa = new CirGate(tmpA, fan0, fan1, invB, invC, 'a', lineNo);
        _gateList.insert(pair<size_t, CirGate*>(tmpA, pa));
	}
	return 0;
}

void
CirMgr::readSym(ifstream & fin){
	++lineNo;
	char type;
	fin >> type;
	if(fin.fail()) return;
	while(type=='i'||type=='o'){
			size_t num;
			string s;
			fin >> num;
			getline(fin, s);
			s.erase(0, 1);
			if(type=='i')      _piList[num]->symbol = s;
			else if(type=='o') _poList[num]->symbol = s;
			fin >> type;
			if(fin.fail()) return;
	}
	return;
}

void
CirMgr::connect(){
	for(map<size_t, CirGate*>::iterator iter=_gateList.begin(); iter!=_gateList.end(); iter++){
		CirGate* p    = iter->second;
		if(p->_type=='z'||p->_type=='i'||p->_type=='f') {}
		else if(p->_type=='o'){ connectleft(p); }
		else if(p->_type=='a'){ connectleft(p); connectright(p); }
	}
}

bool
CirMgr::connectleft(CirGate* &p){
	size_t fan0 = (size_t)(p->_fanin0);
	map<size_t, CirGate*>::iterator left = _gateList.find(fan0);
	if(left == _gateList.end()){
		p->_fanin0 = new CirGate(fan0, 'f');
		p->_fanin0->used = true;
		_gateList.insert(pair<size_t, CirGate*>(fan0, p->_fanin0));
		return false;
	}
	p->_fanin0 = left->second;
	p->_fanin0->used = true;
	return true;
}

bool
CirMgr::connectright(CirGate* &p){
	size_t fan1 = (size_t)(p->_fanin1);
	map<size_t, CirGate*>::iterator right = _gateList.find(fan1);
	if(right == _gateList.end()){
		p->_fanin1 = new CirGate(fan1, 'f');
		p->_fanin1->used = true;
		_gateList.insert(pair<size_t, CirGate*>(fan1, p->_fanin1));
		return false; 
    }
	p->_fanin1 = right->second;
	p->_fanin1->used = true;
	return true;
}

void
CirMgr::genDFSList(){
	for(size_t i=0; i<O; ++i){
		_poList[i]->visited = true;
		_poList[i]->dfs();
	}
	for(map<size_t, CirGate*>::iterator iter=_gateList.begin(); iter!=_gateList.end(); iter++){
		CirGate* p = iter->second;
		if((p->visited==false)&&(p->_type=='i'||p->_type=='a')) _undefined.push_back(p);
	    p->visited = false;
    }   
}

void
CirMgr::reset(){
	for(map<size_t, CirGate*>::iterator iter=_gateList.begin(); iter!=_gateList.end(); iter++){
		delete (iter->second);
	}
	lineNo = 0;
}
/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
void
CirMgr::printAll() const
{
	for(map<size_t, CirGate*>::const_iterator iter=_gateList.begin(); iter!=_gateList.end(); ++iter){
		iter->second->printGate(); 
	}
}

/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
	cout << endl;
	size_t S = I+O+A; 
	cout << "Circuit Statistics" << endl;
	cout << "==================" << endl;
	cout << "  " << "PI ";
	for(size_t i=0; i<(11-to_string(I).length()); ++i) cout << " ";
	cout << I << endl;
	
	cout << "  " << "PO ";
	for(size_t i=0; i<(11-to_string(O).length()); ++i) cout << " ";
	cout << O << endl;
	

	cout << "  " << "AIG";
	for(size_t i=0; i<(11-to_string(A).length()); ++i) cout << " ";
	cout << A << endl;
	
	cout << "------------------" << endl;
	cout << "  Total";
	for(size_t i=0; i<(9-to_string(S).length()); ++i) cout << " ";
	cout << S << endl;
}

void
CirMgr::printNetlist() const
{
	cout << endl;
    for(size_t i=0; i<_dfsList.size(); ++i){
        cout << "[" << i << "] ";
        CirGate* p = _dfsList[i];
		if(p->_type=='i'){				
			cout << "PI  " << p->gateId;
			if(p->symbol != "") cout << " (" << p->symbol << ")";
			cout << endl;
		} 
		else if(p->_type=='a'){
			cout << "AIG " << p->gateId << " ";
			if(p->_fanin0->_type=='f') cout << "*";
			if(p->_invPhase0==true) cout << "!";
			cout << p->_fanin0->gateId << " ";
			if(p->_fanin1->_type=='f') cout << "*";
			if(p->_invPhase1==true) cout << "!";
			cout << p->_fanin1->gateId << endl;
		}
		else if(p->_type=='o'){
			cout << "PO  " << p->gateId << " ";
			if(p->_fanin0->_type=='f') cout << "*";
			if(p->_invPhase0==true) cout << "!";
			cout << p->_fanin0->gateId;
			if(p->symbol != "") cout << " (" << p->symbol << ")";
			cout << endl;
		}
		else if(p->_type=='z'){
			cout << "CONST0" << endl;
		}
		else if(p->_type=='f'){}	
	}
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   size_t size = _piList.size();
   for(size_t i=0; i<size; ++i){
   	cout << " " << _piList[i]->gateId;
   }
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   size_t size = _poList.size();
   for(size_t i=0; i<size; ++i){
   	cout << " " << _poList[i]->gateId;
   }
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
	bool isprint = false;
	for(map<size_t, CirGate*>::const_iterator iter=_gateList.begin(); iter!=_gateList.end(); iter++){
		CirGate* p = iter->second;
		if( (p->_type=='o') && (p->_fanin0->_type=='f') ){
			if(isprint == false){
				cout << "Gates with floating fanin(s):";
				isprint = true;
			}
			cout << " " << p->gateId;
		}
		else if( (p->_type=='a') && ((p->_fanin0->_type=='f')||(p->_fanin1->_type=='f')) ){
            if(isprint == false){
            	cout << "Gates with floating fanin(s):";
            	isprint = true;
			}
			cout << " " << p->gateId;
		}
    }
    if(isprint == true) cout << endl;
    
    bool isempty = _undefined.empty();
    size_t size  = _undefined.size();
    if(isempty==false){
        cout << "Gates defined but not used  :";
		for(size_t i=0; i<size; ++i){
			CirGate* p = _undefined[i];
			if(p->used == false) cout << " " << p->gateId;
		}	
	}
	if(isempty == false) cout << endl;
}


void
CirMgr::writeAag(ostream& outfile) const
{
	size_t a = _aigList.size();
	outfile << "aag " << M << " " << I << " " << L << " " << O << " " << a << endl;
	for(size_t i=0; i<I; ++i){
		cout << 2*(_piList[i]->gateId) << endl;
	}
	for(size_t i=0; i<O; ++i){
		CirGate* p = _poList[i];
		cout << 2*(p->_fanin0->gateId)+(p->_invPhase0) << endl;
	}
	for(size_t i=0; i<a; ++i){
		CirGate* p = _aigList[i];
		cout << 2*(p->gateId) << " " << 2*(p->_fanin0->gateId)+(p->_invPhase0) << " " << 2*(p->_fanin1->gateId)+(p->_invPhase1) << endl;
	}
	for(size_t i=0; i<I; ++i){
		CirGate* p = _piList[i];
		if(p->symbol!="") cout << "i" << i << " " << p->symbol << endl; 
	}
	for(size_t i=0; i<O; ++i){
		CirGate* p = _poList[i];
		if(p->symbol!="") cout << "o" << i << " " << p->symbol << endl; 
	}
	cout << 'c' << endl;
	cout << "AAG output by Chung-Yang (Ric) Huang" << endl;
}
