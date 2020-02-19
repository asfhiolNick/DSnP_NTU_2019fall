/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
//#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed
void
CirMgr::sweep(){
	vector<CirGate*> copy;
	_aigList.clear();
	_dfsList.clear();
	_undefined.clear();
	for(size_t i=0; i<O; ++i){
		_poList[i]->dfs();
		_poList[i]->visited = true;
	}
	
	bool check = 0;
	for(map<size_t, CirGate*>::iterator iter=_gateList.begin(); iter!=_gateList.end();){
		check = 0;
		CirGate* p = iter->second;
		if((p->visited==false)){
			if(p->_type == 'i'){
				p->used = false;
				_undefined.push_back(p);
			}
			else if(p->_type == 'a'){
				_gateList.erase(iter++);
				check = 1;
				--A;
				cout << "Sweeping: AIG(" << p->gateId << ") removed..." << endl; 
				delete p;
			}
			else if(p->_type== 'f'){
				_gateList.erase(iter++);
				check = 1;
				cout << "Sweeping: UNDEF(" << p->gateId << ") removed..." << endl;
				delete p;
			}
		}
	
	    if(check==0){
	    	p->visited = 0;
		    iter++;
		}
    }  
}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void
CirMgr::optimize()
{
	//CirGate* zero = new CirGate((size_t)0, 'z')
	//CirGate9 one  = new CirGate((size_t)0, 'z') and inverter
	//To count #fanout, do dfs(); if p is visited, visited++ and if p is removed-AND's fanin, then visited-- 
	//After all, if visited == 0 and in _dfsList, then used = 0
	//Need return/notice new gate's state(inverter, gate type...) to parent
	for(size_t i=0; i<O; ++i){
		++(_poList[i]->visitnum);
		_poList[i]->optdfs();
		++(_poList[i]->_fanin0->visitnum);
		_poList[i]->optupdate0();	
	}
	for(size_t i=0; i<_dfsList.size(); ++i){
		if(_dfsList[i]->visitnum==0) _dfsList[i]->used = false;
	}
	bool check = 0;
	for(map<size_t, CirGate*>::iterator iter=_gateList.begin(); iter!=_gateList.end();){
		check = 0;
		CirGate* p = iter->second;
		if(p->_type=='l'||p->_type=='r'||p->_type=='0'){
			_gateList.erase(iter++);
			check = 1;
			--A;
			delete p;			
		}
		if(check==0){
			p->visitnum = 0;
			p->optpt = false;
			iter++;
		}
    }
    _aigList.clear();
	_dfsList.clear();
	_undefined.clear(); 
    genDFSList();
}
/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/
