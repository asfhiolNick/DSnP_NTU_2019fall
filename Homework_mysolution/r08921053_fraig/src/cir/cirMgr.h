/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

#include "cirDef.h"

class TaskNode
{
public:
	TaskNode():_name(0), _load(0) {} 
	TaskNode(size_t id, size_t l):_name(id), _load(0) {}
	~TaskNode() {}
	
	void operator += (size_t l) { _load += l; }
	bool operator == (const TaskNode& n) const { return _name==n._name; }
    bool operator < (const TaskNode& n) const { return _load < n._load; }
	const size_t& getName() const { return _name; }
	size_t getLoad() const { return _load; }
	
	size_t operator () () const;
	
	friend ostream& operator << (ostream& os, const TaskNode& n){
	 	return os << "(" << n._name << ")";
	};
	
private:
	size_t   _name;
    size_t   _load;
	
};

extern CirMgr *cirMgr;

class CirMgr
{
public:
   friend class CirGate;
   CirMgr() {}
   ~CirMgr() {}
   CirGate* const0; 

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const;

   // Member functions about circuit construction
   bool readCircuit(const string&);
   bool readHeader(ifstream &);
   bool readInput(ifstream &);
   bool readOutput(ifstream &);
   bool readAig(ifstream &);
   void readSym(ifstream &);
   void connect();
   bool connectleft(CirGate* &);
   bool connectright(CirGate* &);
   void genDFSList();
   void reset();

   // Member functions about circuit optimization
   void sweep();
   void optimize();

   // Member functions about simulation
   void randomSim();
   void fileSim(ifstream&);
   void setSimLog(ofstream *logFile) { _simLog = logFile; }

   // Member functions about fraig
   void strash();
   void printFEC() const;
   void fraig();

   // Member functions about circuit reporting
   void printAll()     const;
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void printFECPairs() const;
   void writeAag(ostream&) const;
   void writeGate(ostream&, CirGate*);
   
   // Adding by me
   size_t getM() const { return M; } 

private:
   ofstream           *_simLog;
	size_t M=-1, I=-1, L=-1, O=-1, A=-1;
	size_t mm=-1, ii=-1, ll=-1, oo=-1, aa=-1;
	vector<CirGate*>      _piwrite;
	vector<CirGate*>      _aigwrite;
	map<size_t, CirGate*> _gateList;
	vector<CirGate*>      _piList;
	vector<CirGate*>      _poList;
	vector<CirGate*>      _aigList;
	vector<CirGate*>      _dfsList;
	vector<CirGate*>      _undefined;
	HashSet<TaskNode>     _taskHash;
};
#endif // CIR_MGR_H
