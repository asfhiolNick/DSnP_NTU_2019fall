/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"
#include "sat.h"

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
class CirGate
{
public:
	friend class CirMgr;
	friend class TaskNode;
    CirGate() {}
    CirGate(size_t ID, char type):gateId(ID), _type(type)  {}
    CirGate(size_t ID, char type, size_t line):gateId(ID), _type(type),_line(line)  {}
    CirGate(size_t ID, CirGate* fan0, CirGate* fan1, bool sgn0, bool sgn1, char type, size_t line):
    gateId(ID), _fanin0(fan0), _fanin1(fan1), _invPhase0(sgn0), _invPhase1(sgn1), _type(type), _line(line) {}
    ~CirGate() {}

   // Basic access methods
   string getTypeStr() const { return ""; }
   unsigned getLineNo() const { return 0; }
   bool isAig() const { return false; }

   // Printing functions
   void printGate() const;
   void reportGate() const;
   void reportFanin(int level);
   void dfsFanin(int, int, bool);
   void reportFanout(int level) const;
   void writeFanin();
   void dfs();
   void optdfs();
   void strdfs();
   void optupdate0();
   void optupdate1();
   void strupdate0();
   void strupdate1();
   
private:
	size_t   gateId;
	CirGate* _fanin0 = (CirGate*)((size_t)-1);
	CirGate* _fanin1 = (CirGate*)((size_t)-1);
	bool     _invPhase0 = false;
	bool     _invPhase1 = false;
	char     _type;
	bool     visited = false;
	int      visitnum = 0;
	string   symbol = "";
	size_t   _line = 0; 
	bool     used    = false;
	bool     optpt  = false;
	CirGate* strash = 0;

protected:
};

#endif // CIR_GATE_H
