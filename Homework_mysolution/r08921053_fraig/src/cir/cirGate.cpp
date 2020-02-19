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

// TODO: Keep "CirGate::reportGate()", "CirGate::reportFanin()" and
//       "CirGate::reportFanout()" for cir cmds. Feel free to define
//       your own variables and functions.

extern CirMgr *cirMgr;

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
		cout << "================================================================================" << endl;
		cout << "= PI(" << gateId << ")";
		blank+=(6+to_string(gateId).length());
		if(symbol!=""){
			cout << "\"" << symbol << "\"";
			blank+=(2+symbol.length());
		}
	}
	else if(_type=='o'){
		cout << "================================================================================" << endl;
		cout << "= PO(" << gateId << ")";
		blank+=(6+to_string(gateId).length());
		if(symbol!=""){
			cout << "\"" << symbol << "\"";
			blank+=(2+symbol.length());
		}
	}
	else if(_type=='a'){
		cout << "================================================================================" << endl;
		cout << "= AIG(" << gateId << ")";
		blank+=(7+to_string(gateId).length());
	}
	else if(_type=='z'){
		cout << "================================================================================" << endl;
		cout << "= CONST(0)";
		blank+=10;
	}
	else if(_type=='f'){
		cout << "================================================================================" << endl;
		cout << "= UNDEF(" << gateId << ")";
		blank+=(9+to_string(gateId).length());
	}
	
	cout << ", line " << _line << endl;
	cout << "= FECs:" << endl;
	cout << "= Value: 00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000";
	cout << "================================================================================" << endl;
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
	if(level==0){
		cout << endl;
	}
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
}

void
CirGate::writeFanin(){
	if(this->gateId>cirMgr->mm) cirMgr->mm = this->gateId;
	if(this->_type=='i'){
		++cirMgr->ii;
		cirMgr->_piwrite.push_back(this); 
	}
	else if(this->_type=='o'){
		if(this->_fanin0->visited==false){
			CirGate* p = this->_fanin0;
			p->visited = true;
			p->writeFanin();
		}
	}
    else if(this->_type=='a'){
		if(this->_fanin0->visited==false){
			CirGate* p = this->_fanin0;
			p->visited = true;
			p->writeFanin();
	    }
		if(this->_fanin1->visited==false){
			CirGate* p = this->_fanin1;
			p->visited = true;
			p->writeFanin();
		}
		++cirMgr->aa;
		cirMgr->_aigwrite.push_back(this);
	}
	
}

void
CirGate::dfs(){
	if(this->_type=='i'||this->_type=='z'||this->_type=='f'){}
	else if(this->_type=='o'){
		if(this->_fanin0->visited==false){
			CirGate* p = this->_fanin0;
			p->visited = true;
			p->dfs();
		}
		else{
			this->_fanin0->visited = true;
		}
	}
	else if(this->_type=='a'){
		if(this->_fanin0->visited==false){
			CirGate* p = this->_fanin0;
			p->visited = true;
			p->dfs();
	    }
	    else{
		   this->_fanin0->visited = true;
		}
		if(this->_fanin1->visited==false){
			CirGate* p = this->_fanin1;
			p->visited = true;
			p->dfs();
		}
		else{
		   this->_fanin1->visited = true;
		}
	}
	if(this->_type!='f'){
		cirMgr->_dfsList.push_back(this);
		if(this->_type=='a') cirMgr->_aigList.push_back(this);
	}
}


void
CirGate::optdfs(){
	if(this->_type=='i'||this->_type=='z'||this->_type=='f'){}
	else if(this->_type=='o'){
		if(this->_fanin0->visitnum==0){
			this->_fanin0->optdfs();
		}
		++(this->_fanin0->visitnum);
		optupdate0();
	}
	else if(this->_type=='a'){
		if(this->_fanin0->visitnum==0){
			this->_fanin0->optdfs();
		}
		++(this->_fanin0->visitnum);
		optupdate0();     
		
		if(this->_fanin1->visitnum==0){
            this->_fanin1->optdfs();
		}
		++(this->_fanin1->visitnum);
		optupdate1();
		
		//Start to optimize
		if(this->_fanin0->gateId==0&&this->_invPhase0==1){
			_type = 'r';
			--(this->_fanin0->visitnum);
			--(this->_fanin1->visitnum);
		} 
		else if(this->_fanin1->gateId==0&&this->_invPhase1==1){
			_type = 'l';
			--(this->_fanin0->visitnum);
			--(this->_fanin1->visitnum);
		}
		else if(this->_fanin0->gateId==0&&this->_invPhase0==0){
			_type = '0';
			--(this->_fanin0->visitnum);
			--(this->_fanin1->visitnum);
		}
		else if(this->_fanin1->gateId==0&&this->_invPhase1==0){
			_type = '0';
			--(this->_fanin0->visitnum);
			--(this->_fanin1->visitnum);
		}
		else if(this->_fanin0->gateId==this->_fanin1->gateId&&_invPhase0==_invPhase1){
			_type = 'l';
			--(this->_fanin0->visitnum);
			--(this->_fanin1->visitnum);
		}
		else if(this->_fanin0->gateId==this->_fanin1->gateId&&_invPhase0!=_invPhase1){
			_type = '0';
			--(this->_fanin0->visitnum);
			--(this->_fanin1->visitnum);	
		}
		
	}
}

void
CirGate::strdfs(){
	if(this->_type=='o'){
		if(this->_fanin0->visitnum==0){
			this->_fanin0->strdfs();
		}
		++(this->_fanin0->visitnum);
		strupdate0();
	}
	else if(this->_type=='a'){
		if(this->_fanin0->visitnum==0){
			this->_fanin0->strdfs();
		}
		++(this->_fanin0->visitnum);
		strupdate0();     
		
		if(this->_fanin1->visitnum==0){
            this->_fanin1->strdfs();
		}
		++(this->_fanin1->visitnum);
		strupdate1();
		
		
		cirMgr->_taskHash.remove(TaskNode(gateId, 0));
		TaskNode* n = new TaskNode(gateId, 0); 
	    cirMgr->_taskHash.insert(*n);
	    size_t sz = getHashSize(23*(cirMgr->getM()));
	    for(size_t i=0; i<sz; ++i){
	    	size_t size = cirMgr->_taskHash._buckets[i].size();
	    	for(size_t j=0; j<size; ++j){
	    		if(cirMgr->_taskHash._buckets[i][j]==*n&&j>0){
	    			TaskNode normal = cirMgr->_taskHash._buckets[i][0];
	    			CirGate* pormal = cirMgr->getGate(normal.getName());
	    			CirGate* p = cirMgr->getGate(gateId);
	    			if(p->_type!='s'&&p->strash!=0&&p->strash->gateId==pormal->strash->gateId){
	    				//do not need to update, since it is the 1st element in vector=_buckets[i]
					}
					else{
	    			    p->_type = 's';
	    			    p->strash = pormal;
	    		    }
				}
			}
		}
    }
	
	
}

void
CirGate::optupdate0(){	
	if(this->_fanin0->_type=='l'){
		CirGate* p = this->_fanin0;
		if(p->optpt==false){
			p->optpt = true; 
	        cout << "Simplifying: " << p->_fanin0->gateId << " merging ";
	        if(p->_invPhase0==1) cout << "!";
	        cout << p->gateId << "..." << endl;
		}
		
		this->_invPhase0 = (this->_invPhase0 + this->_fanin0->_invPhase0)%2;
		this->_fanin0    = this->_fanin0->_fanin0;
		++(this->_fanin0->visitnum);
	}
	else if(this->_fanin0->_type=='r'){
		CirGate* p = this->_fanin0;
		if(p->optpt==false){
			p->optpt = true;
		    cout << "Simplifying: " << p->_fanin1->gateId << " merging ";
		    if(p->_invPhase1==1) cout << "!";
        	cout << p->gateId << "..." << endl;
		}

		this->_invPhase0 = (this->_invPhase0 + this->_fanin0->_invPhase1)%2;
		this->_fanin0    = this->_fanin0->_fanin1;
		++(this->_fanin0->visitnum);
	}
	else if(this->_fanin0->_type=='0'){
		CirGate* p = this->_fanin0;
		if(p->optpt==false){
			p->optpt = true;
			cout << "Simplifying: " << cirMgr->const0->gateId << " merging " << p->gateId << "..." << endl;
		}		
		
		this->_invPhase0 = this->_invPhase0+0;
		this->_fanin0    = cirMgr->const0;
		++(this->_fanin0->visitnum);
	}
}

void
CirGate::optupdate1(){
	if(this->_fanin1->_type=='l'){
		CirGate* p = this->_fanin1;
		if(p->optpt==false){
			p->optpt = true;
	        cout << "Simplifying: " << p->_fanin0->gateId << " merging ";
	        if(p->_invPhase0==1) cout << "!";
	        cout << p->gateId << "..." << endl;
		}		
		
		this->_invPhase1 = (this->_invPhase1 + this->_fanin1->_invPhase0)%2;
		this->_fanin1    = this->_fanin1->_fanin0;
		++(this->_fanin1->visitnum); 
	}
	else if(this->_fanin1->_type=='r'){
		CirGate* p = this->_fanin1;
		if(p->optpt==false){
			p->optpt = true;
		    cout << "Simplifying: " << p->_fanin1->gateId << " merging ";
		    if(p->_invPhase1==1) cout << "!";
        	cout << p->gateId << "..." << endl;
		}
		
		this->_invPhase1 = (this->_invPhase1 + this->_fanin1->_invPhase1)%2;
		this->_fanin1    = this->_fanin1->_fanin1;
		++(this->_fanin1->visitnum);
	}
	else if(this->_fanin1->_type=='0'){
		CirGate* p = this->_fanin1;
		if(p->optpt==false){
			p->optpt = true;
			cout << "Simplifying: " << cirMgr->const0->gateId << " merging " << p->gateId << "..." << endl;
		}
		
		this->_invPhase1 = this->_invPhase1+0;
		this->_fanin1    = cirMgr->const0;
		++(this->_fanin1->visitnum);
	}
}

void
CirGate::strupdate0(){
	if(this->_fanin0->_type=='s'){
		this->_fanin0 = this->_fanin0->strash;
		++(this->_fanin0->visitnum);
	}
}

void
CirGate::strupdate1(){
	if(this->_fanin1->_type=='s'){
		this->_fanin1 = this->_fanin1->strash;
		++(this->_fanin1->visitnum); 
	}
}
