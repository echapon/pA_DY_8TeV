#ifndef bin_h
#define bin_h

#include <utility>

using namespace std;

// a simple template class to store a bin and overload the equality operator

// define a few common uses of the template class
template <typename T> class binT : public pair<T,T> {
   public:
      binT(T a, T b) : pair<T,T>(a,b) {};
      binT() : pair<T,T>() {};
      T low() const {return this->first;}
      T high() const {return this->second;}
};
typedef binT<double> bin;
typedef binT<float> binF;
typedef binT<int> binI;

#endif // #ifndef bin_h
