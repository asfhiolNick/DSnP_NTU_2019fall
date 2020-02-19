/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void 
CirGate::printGate() const
{
	cout << "=======================================" << endl;
	cout << "type = " << _type;
	cout << "; gateId = " << gateId;
	cout << "; _line = " << _line << endl;
	if(_type=='o'){
		cout << "; _fanin0 = " << _fanin0->gateId << "; _invPhase0 = " << _invPhase0 << endl;
		
	}
	else if(_type=='a'){
		cout << "; _fanin0/1 = " << _fanin0->gateId << " " << _fanin1->gateId;
		cout << "; _invPhase0/1 = " << _invPhase0 << " " << _invPhase1 << endl; 
	}
	cout << "=======================================" << endl;
	cout << endl;
}

void
CirGate::reportGate() const
{
	size_t blank = 0;
	if(_type=='i'){
		cout << "==================================================" << endl;
		cout << "= PI(" << gateId << ")";
		blank+=(6+to_string(gateId).length());
		if(symbol!=""){
			cout << "\"" << symbol << "\"";
			blank+=(2+symbol.length());
		}
	}
	else if(_type=='o'){
		cout << "==================================================" << endl;
		cout << "= PO(" << gateId << ")";
		blank+=(6+to_string(gateId).length());
		if(symbol!=""){
			cout << "\"" << symbol << "\"";
			blank+=(2+symbol.length());
		}
	}
	else if(_type=='a'){
		cout << "==================================================" << endl;
		cout << "= AIG(" << gateId << ")";
		blank+=(7+to_string(gateId).length());
	}
	else if(_type=='z'){
		cout << "==================================================" << endl;
		cout << "= CONST(0)";
		blank+=10;
	}
	else if(_type=='f'){
		cout << "==================================================" << endl;
		cout << "= UNDEF(" << gateId << ")";
		blank+=(9+to_string(gateId).length());
	}
	
	cout << ", line " << _line;
	blank+=(7+to_string(_line).length());
	for(size_t i = blank; i < 49; ++i) cout << " ";
	cout << "=" << endl;
	cout << "==================================================" << endl;
}

void
CirGate::reportFanin(int level)
{
   assert (level >= 0);
   dfsFanin(level, 0, false);
	for(map<size_t, CirGate*>::iterator iter=cirMgr->_gateList.begin(); iter!=cirMgr->_gateList.end(); iter++){
	    iter->second->visited = false;
    }   
}

void
CirGate::dfsFanin(int level, int depth, bool _invPhase)
{

	for(int i=0; i<depth; ++i) cout << "  ";
	if(_invPhase==true) cout << "!";
	
	if(_type=='i')      cout << "PI";
	else if(_type=='o') cout << "PO";
	else if(_type=='a') cout << "AIG";
	else if(_type=='z') cout << "CONST";
	else if(_type=='f') cout << "UNDEF";
	cout << " " << gateId;
	
	if(level>0){
		if(_type=='i'||_type=='z'||_type=='f'){
			visited = true;
		    cout << endl;
		}
		else if(visited == true){
			cout << " (*)" << endl;
		}
		else{
			visited = true;
			cout << endl;
			
			if(_type=='o'){
				_fanin0->dfsFanin((level-1), (depth+1), _invPhase0);
			}
			else if(_type=='a'){
				_fanin0->dfsFanin((level-1), (depth+1), _invPhase0);
				_fanin1->dfsFanin((level-1), (depth+1), _invPhase1);
			}
		}
	}	
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
}

void
CirGate::dfs(){
	if(this->_type=='i'||this->_type=='z'||this->_type=='f'){}
	else if(this->_type=='o'){
		if((this->_fanin0)->visited==false){
			CirGate* p = this->_fanin0;
			p->visited = true;
			p->dfs();
		}
	}
	else if(this->_type=='a'){
		if((this->_fanin0)->visited==false){
			CirGate* p = this->_fanin0;
			p->visited = true;
			p->dfs();
	    }
		if((this->_fanin1)->visited==false){
			CirGate* p = this->_fanin1;
			p->visited = true;
			p->dfs();
		}
	}
	if(this->_type!='f'){
		cirMgr->_dfsList.push_back(this);
		if(this->_type=='a') cirMgr->_aigList.push_back(this);
	}
}
