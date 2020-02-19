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
#include <list>

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   friend class CirGate;
   CirMgr() {}
   ~CirMgr() {}

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

   // Member functions about circuit reporting
   void printAll()     const;
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) const;

private:
	size_t M=-1, I=-1, L=-1, O=-1, A=-1;
	map<size_t, CirGate*> _gateList;
	vector<CirGate*>      _piList;
	vector<CirGate*>      _poList;
	vector<CirGate*>      _aigList;
	vector<CirGate*>      _dfsList;
	vector<CirGate*>      _undefined;
};


#endif // CIR_MGR_H
