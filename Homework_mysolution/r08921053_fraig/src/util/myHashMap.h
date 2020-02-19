/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>

using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ==" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
template <class Data>
class HashSet
{
	friend class CirMgr;
        friend class CirGate;
public:
   HashSet(size_t b = 0) : _numBuckets(0), _buckets(0) { if (b != 0) init(b); }
   ~HashSet() { reset(); }

   // TODO: implement the HashSet<Data>::iterator
   // o An iterator should be able to go through all the valid Data
   //   in the Hash
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   class iterator
   {
      friend class HashSet<Data>;

   public:
   	  iterator(const HashSet* from):from(from) {}
   	  iterator(Data input, const HashSet* from):_data(input), from(from) {}
   	  ~iterator() {}
      const Data& operator * () const { return _data; }
      iterator& operator ++ () {
	      size_t id = from->bucketNum(_data);
		  typename vector<Data>::iterator iter=find(from->_buckets[id].begin(), from->_buckets[id].end(), _data);
		  ++iter;
		  
		  if(iter!=from->_buckets[id].end()){ _data = *iter; }
		  else{
		  	  ++id;
		  	  while( (id<from->_numBuckets) &&from->_buckets[id].empty()==true) ++id;
		  	  if(id == from->_numBuckets) _data = Data(0, 0);
		  	  else                  _data = from->_buckets[id].front();
		  }
		  return *this;
	  }
	  bool operator == (const iterator& i) const { return (_data==i._data); }
      bool operator != (const iterator& i) const { return !(_data==i._data); }
      void operator =  (iterator& i) { _data = i._data; }
   private:
   	  Data _data;
   	  const HashSet<Data>* from;
   };

   void init(size_t b) { _numBuckets = b; _buckets = new vector<Data>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   void clear() {
      for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<Data>& operator [] (size_t i) { return _buckets[i]; }
   const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const {
       size_t id = 0;
       while(id<_numBuckets&&_buckets[id].empty()==true){ ++id; }
       if(id==_numBuckets) return iterator(Data(0, 0), this);
       else                return iterator(_buckets[id].front(), this);
   }
   // Pass the end
   iterator end() const { return iterator(Data(0, 0), this); }
   // return true if no valid data
   bool empty() const { return (begin()==end()); }
   // number of valid data
   size_t size() const {
       size_t ans = 0;
       for(HashSet::iterator iter=begin(); iter!=end(); ++iter){
       	    ++ans;
	   }
	   return ans;
   }

   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const Data& d) const {
       size_t id = bucketNum(d);
       typename vector<Data>::iterator iter=find(_buckets[id].begin(), _buckets[id].end(), d);
       if(iter!=_buckets[id].end()) return true;
       else                         return false;
   }

   // query if d is in the hash...
   // if yes, replace d with the data in the hash and return true;
   // else return false;
   bool query(Data& d) const {
       size_t id = bucketNum(d);
	   typename vector<Data>::iterator iter=find(_buckets[id].begin(), _buckets[id].end(), d);
	   if(iter!=_buckets[id].end()){
	   	    d = *(iter);
	   	    return true;
	   }
	   return false;
   }

   // update the entry in hash that is equal to d (i.e. == return true)
   // if found, update that entry with d and return true;
   // else insert d into hash as a new entry and return false;
   bool update(const Data& d) {
       size_t id = bucketNum(d);
       typename vector<Data>::iterator iter=find(_buckets[id].begin(), _buckets[id].end(), d);
       if(iter!=_buckets[id].end()){
       	   *(iter) = d;
       	   return true;
	   }
	   _buckets[id].push_back(d);
	   return false;
   }

   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const Data& d) {
       size_t id = bucketNum(d);
       typename vector<Data>::iterator iter=find(_buckets[id].begin(), _buckets[id].end(), d);
       if(iter==_buckets[id].end()){
       	    _buckets[id].push_back(d);
       	    return true;
	   }
	   return false;
   }

   // return true if removed successfully (i.e. d is in the hash)
   // return fasle otherwise (i.e. nothing is removed)
   bool remove(const Data& d) {
       size_t id = bucketNum(d);
       typename vector<Data>::iterator iter=find(_buckets[id].begin(), _buckets[id].end(), d);
       if(iter!=_buckets[id].end()){
       	    typename vector<Data>::iterator tmp = iter;
       	    ++tmp;
       	    if(tmp!=_buckets[id].end()){
       	    	*iter = _buckets[id].back();
			   }
			_buckets[id].pop_back();
       	    return true;
	   }
	   return false;
   }

private:
   // Do not add any extra data member
   size_t            _numBuckets;
   vector<Data>*     _buckets;

   size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
