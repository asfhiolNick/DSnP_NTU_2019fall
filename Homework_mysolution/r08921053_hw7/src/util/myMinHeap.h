/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { return _data.front(); }
   void insert(const Data& d) {
       _data.push_back(Data("", 0));
	   size_t t = _data.size();
	   while(t>1){
	   	   size_t p = t/2;
	   	   if(!(d < _data[p-1])) break;
	   	   _data[t-1] = _data[p-1];
	   	   t = p;
	   }
	   _data[t-1] = d;
   }
   void delMin() {
       size_t p = 1, t = 2*p, n=_data.size();
       while(t<=n){
       	   if(t<n && (_data[t+1-1] < _data[t-1])) ++t;
       	   if(!(_data[t-1] < _data[n-1])) break;
       	   _data[p-1] = _data[t-1];
       	   p = t;
       	   t = 2*p;
	   }
	   _data[p-1] = _data[n-1];
	   _data.pop_back();
   }
   void delData(size_t i) {
   	   size_t given = i+1;
   	   Data d = _data[_data.size()-1];
   	   size_t t = given;
   	   while(t>1){
   	       size_t p=t/2;
   	       if(!(d < _data[p-1])) break;
   	       _data[t-1] = _data[p-1];
   	       t = p;
	   }
	   _data[t-1] = d;
	   
   	   Data tmp = _data[given-1];
   	   size_t p = given; t = 2*p;
	   size_t n=_data.size();
   	   while(t<=n){
   	   	    if(t<n && (_data[t+1-1]<_data[t-1])) ++t;
   	   	    if(!(_data[t-1] < _data[n-1])) break;
   	   	    _data[p-1] = _data[t-1];
   	   	    p = t;
   	   	    t = 2*p;
		  }
		  _data[p-1] = tmp;
		  _data.pop_back();
	}

private:
   // DO NOT add or change data members
   std::vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
