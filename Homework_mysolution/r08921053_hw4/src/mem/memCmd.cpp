/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}



//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO
   try{
   	    vector<string> options;
   	    if(CmdExec::lexOptions(option, options)==0) throw "CMD_MTNewCmd_EXEC_ERROR";
	    if(options.empty()==1) throw "CMD_MTNewCmd_OPT_MISSING";
	    bool havearray = 0;
	    int arraysize = 0;
	    int numObjects = 0;
	    for(int i=0, n=options.size(); i<n; i++){
	    	if(myStrNCmp("-Array", options[i], 2) == 0){
	    		havearray = 1;
	    		if((i+1)>=n) throw "CMD_MTNewCmd_NO_ARRAYSIZE";
	    		else if(arraysize!=0) throw "CMD_MTNewCmd_TWO_ARRAYSIZE";
	    		else if(myStr2Int(options[i+1],arraysize)==0) throw "CMD_MTNewCmd_ARRAYSIZE_NOT_INT";
	    		else if(arraysize<=0) throw "CMD_MTNewCmd_ARRAYSIZE_NOT_POSITIVE";
	    		else ;//get arraysize
			}
			else{
				if(i==0){
					if(numObjects!=0) throw "CMD_MTNewCmd_TWO_NUMOBJECTS"; 
					else if(myStr2Int(options[i],numObjects)==0) throw "CMD_MTNewCmd_NUMOBJECTS_NOT_INT";
					else if(numObjects<=0) throw "CMD_MTNewCmd_NUMOBJECTS_NOT_POSITIVE";
					else; //get numObjects
				}
				else{
					if(myStrNCmp("-Array", options[i-1], 2) != 0){
						if(numObjects!=0) throw "CMD_MTNewCmd_TWO_NUMOBJECTS";
						else if(myStr2Int(options[i],numObjects)==0) throw "CMD_MTNewCmd_NUMOBJECTS_NOT_INT";
						else if(numObjects<=0) throw "CMD_MTNewCmd_NUMOBJECTS_NOT_POSITIVE";
						else ;//get numObjects
					}
					else ;//options[i]=arraysize
				}
			}
		}
		if(numObjects==0) throw "CMD_MTNewCmd_NO_NUMOBJECTS";
		size_t temp_num = (size_t) numObjects;
		
		if(havearray==0){
			mtest.newObjs(temp_num);
		} 
		else{
			if(arraysize==0) throw "CMD_MTNewCmd_NO_ARRAYSIZE";
			size_t temp_arr = (size_t) arraysize;
            
			mtest.newArrs(temp_num, temp_arr);
		}
   }
   catch(const char * str){
   	cerr << "Exception raised: " << str << endl;
   }
   catch(bad_alloc &memoryAllocationException){
   	cout << "Exception occurred: " << endl;
   } 
   catch(...){
        cout << "Some other problem in catch" << endl;
   }
   // Use try-catch to catch the bad_alloc exception
   return CMD_EXEC_DONE;
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO
   	try{
   	    vector<string> options;
   	    if(CmdExec::lexOptions(option, options)==0) throw "CMD_MTDeleteCmd_EXEC_ERROR";
	    if(options.empty()==1) throw "CMD_MTDeleteCmd_OPT_MISSING";
	    bool havearray = 0;
	    int  objId     = -1;
	    int  numRandId = 0;
	    for(int i=0, n=options.size(); i<n; i++){
	    	if(myStrNCmp("-Array", options[i], 2) == 0) havearray = 1;
	    	else if(myStrNCmp("-Index", options[i], 2) == 0){
	    		if((i+1)>=n) throw "CMD_MTDeleteCmd_NO_objId";
	    		else if(objId!=-1) throw "CMD_MTDeleteCmd_TWO_objId";
	    		else if(myStr2Int(options[i+1], objId)==0) throw "CMD_MTDeleteCmd_objId_NOT_INT";
	    		else if(objId<0) throw "CMD_MTDeleteCmd_objId_NEGATIVE";
	    		else ;//get objId
			}
			else if(myStrNCmp("-Random", options[i], 2) == 0){
	    		if((i+1)>=n) throw "CMD_MTDeleteCmd_NO_numRandId";
	    		else if(numRandId!=0) throw "CMD_MTDeleteCmd_TWO_numRandId";
	    		else if(myStr2Int(options[i+1], numRandId)==0) throw "CMD_MTDeleteCmd_numRandId_NOT_INT";
	    		else if(numRandId<=0) throw "CMD_MTDeleteCmd_numRandId_NOT_POSITIVE";
	    		else ;//get numRandid
			}
			else if(i==0) throw "CMD_MTDeleteCmd_UNDEFINITED_WORD(i=0)";
			else{
				if(myStrNCmp("-Array", options[i-1], 2)!=0 &&
				   myStrNCmp("-Index", options[i-1], 2)!=0 &&
				   myStrNCmp("-Random", options[i-1], 2)!=0) throw "CMD_MTDeleteCmd_UNDEFINITED_WORD(i-1 not -a, -i, -r)";
			}	
		}
		if(objId==-1&&numRandId==0) throw "CMD_MTDeleteCmd_NO_INDEX_NO_RANDOM";
		else if(objId!=-1&&numRandId!=0) throw "CMD_MTDeleteCmd_BOTH_INDEX_RANDOM";
		else if(objId!=-1&&numRandId==0){
			size_t temp_obj = (size_t) objId;
			if(havearray == 0 ){
				size_t objsz = mtest.getObjListSize();
				if(temp_obj >= objsz){
					cerr << "objId = " << objId << " and objsize = " << objsz << endl; 
					throw "CMD_MTDeletedCmd_objId>=getObjListSize()";
				} 
				mtest.deleteObj(temp_obj);
			} 
			else if(havearray == 1){
				size_t arrsz = mtest.getArrListSize();
				if(temp_obj >= arrsz){
					cerr << "objId = " << objId << "and arrsize = " << arrsz << endl;
					throw "CMD_MTDeletedCmd_objId>=getArrListSize()";
			    }
				 mtest.deleteArr(temp_obj);
			}
		}
		else if(objId==-1&&numRandId!=0){
			if(havearray == 0){
				size_t objsz = mtest.getObjListSize(); 
                                if(objsz==0) throw "SIZE_OF_OBJ_LIST_IS_0";
				for(int i=0; i<numRandId; i++){
					unsigned randobj = rnGen((unsigned)objsz);
				    randobj = (size_t) randobj;
				    mtest.deleteObj(randobj);
				}
			}
			else if(havearray == 1){
				size_t arrsz = mtest.getArrListSize();
				if(arrsz==0) throw "SIZE_OF_ARR_LIST_IS_0";
				for(int i=0; i<numRandId; i++){
					unsigned randarr = rnGen((unsigned)arrsz);
					randarr = (size_t) randarr;
					mtest.deleteArr(randarr);
				}
			}
		}
	}
   catch(const char * str){
   	cerr << "Exception raised: " << str << endl;
   }


   return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


