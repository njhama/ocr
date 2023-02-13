#include "bigint.h"
#include <iostream>
using namespace std;
int main()  {
  BigInt b1("0");
  //cout << b1.to_string() << endl; // should print 0
  BigInt b2("00");
  //cout << b2.to_string() << endl; // should print 0
  BigInt b3("007");
  BigInt t1("2111111");
  BigInt t2("3000000");
  //cout << b3.to_string() << endl; // should print 7
  //BigInt b8 = t1 + t2;
  //cout << "sum: " << b8.to_string() << endl;
  //bool test = t2 < t1;
  //cout << "BOOL: " << test << endl;

  //cout << "!: " << endl;
  BigInt b9 = t2 - t1;
  //cout << "23" << endl;
  cout << b9.to_string() << endl;
  return 0;
}