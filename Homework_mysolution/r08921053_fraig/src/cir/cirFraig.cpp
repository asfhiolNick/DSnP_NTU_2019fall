/****************************************************************************
  FileName     [ cirFraig.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir FRAIG functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
#include "myHashMap.h"
#include "util.h"
#include <algorithm>
#include <iostream>
#include <cstring> 

using namespace std;

// TODO: Please keep "CirMgr::strash()" and "CirMgr::fraig()" for cir cmd.
//       Feel free to define your own variables or functions


/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/*******************************************/
/*   Public member functions about fraig   */
/*******************************************/
// _floatList may be changed.
// _unusedList and _undefList won't be changed
void
CirMgr::strash()
{
	size_t sz = getHashSize(23*M);
	HashSet<TaskNode>* tmp = new HashSet<TaskNode>(sz);
    _taskHash = *tmp;
    //Add aig gate into TaskNode and clarify them in HashSet<TaskNode> _taskHash 
	for(size_t i=0; i<_aigList.size(); ++i){
		CirGate* p = _aigList[i];
		TaskNode* element = new TaskNode(p->gateId, 0); 
		_taskHash.insert(*element);
	}
    for(size_t i=0; i<sz; ++i){
    	if(_taskHash._buckets[i].size()>1){
    		vector<TaskNode> buck = _taskHash._buckets[i];
    		TaskNode normal = buck[0];
    		CirGate* pormal = cirMgr->getGate(normal.getName());
    		pormal->strash = pormal; 
			for(size_t j=1; j<buck.size(); ++j){
    			TaskNode n = buck[j];
    			CirGate* p = cirMgr->getGate(n.getName());
    			p->_type = 's';
    			p->strash = pormal;
			}
		} 
	}
	//do strdfs
	for(size_t i=0; i<O; ++i){
		++(_poList[i]->visitnum);
		_poList[i]->strdfs();
		++(_poList[i]->_fanin0->visitnum);
		_poList[i]->strupdate0();
	}
	//erase type 's' gate and clear their visitnum/strash
	bool check=0;
	for(map<size_t, CirGate*>::iterator iter=_gateList.begin(); iter!=_gateList.end();){
		check = 0;
		CirGate* p = iter->second;
		if(p->_type=='s'){
			_gateList.erase(iter++);
			check = 1;
			--A;
			if(p->strash!=0){
				cout << "Strashing: " << p->strash->gateId << " merging " << p->gateId << "..." << endl;
			}
			delete p;			
		}
		if(check==0){
			p->visitnum = 0;
			p->strash = 0; 
			iter++;
		}
    }
    _aigList.clear();
	_dfsList.clear();
	_undefined.clear(); 
    genDFSList();
}

void
CirMgr::fraig()
{
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/
