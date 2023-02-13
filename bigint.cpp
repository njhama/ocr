#include <iostream>
#include "bigint.h"

using namespace std;

// Write your implementation below
BigInt::BigInt(string s, int base)  {
  for (int i = s.length(); i > 0; i--)  {
    int temp;
    if (s[i-1] >= 48 and s[i-1] <= 57)  {
      temp = static_cast<int>(s[i-1]) - '0';
    }
    else  {
      temp = 10 + static_cast<int>(s[i-1]) - 'A';
    }
    nums.push_back(temp);
  }
  myBase = base;
  this->removeLeadingZeroes();
}

string BigInt::to_string()  const {
  string out = "";
  char temp;
  for (int i = 0; i < nums.size(); i++) {
    int index = nums.size() - i -1;
    if(nums[index] >= 0 and nums[index] <= 9) {
      temp = static_cast<char>(nums[index] + 48);
    }
    else  {
      temp = static_cast<char>(nums[index] -10 + 'A');
    }
    out += temp;
  }
  return out;
}

void BigInt::add(BigInt b)  {
  int sizeFirst = nums.size();
  int sizeSecond = b.nums.size();

  bool firstLarger;

  if (sizeFirst >= sizeSecond) {firstLarger = true;}
  else {firstLarger = false;}
  
  while (sizeFirst != sizeSecond) {
    if (firstLarger)  {b.nums.push_back(0);}
    else {nums.push_back(0);}
    sizeFirst = nums.size();
    sizeSecond = b.nums.size();
  }


  //vectors are now same size

  int index = 0;
  int carryover = 0, rowsum, keep;
  vector<int> sumVect;

  while (nums.size() != index)  {
    //do for index = 0;
    rowsum = nums[index] + b.nums[index] + carryover;
    if (rowsum == myBase) {
      keep = 0;
      carryover = 1;
    }
    else if (rowsum > myBase) {
      keep = rowsum % myBase;
      carryover = rowsum / myBase;
    }
    else  {
      keep = rowsum;
      carryover = 0;
    }
  sumVect.push_back(keep);
  
  index++;
  if (nums.size() == index and carryover != 0)  {sumVect.push_back(carryover);}
  //carryover = 0;
  keep = 0;
  
  }

  nums = sumVect;
} 


void BigInt::removeLeadingZeroes()  {
  //if size is not just 0 or empty
  if (nums.size() > 1) {
    //go to back of the list and check if 0
    for (int i = nums.size() - 1; i > 0; i--) {
      if (nums.back() == 0) {
        nums.pop_back();
      }
    }
  }
}


BigInt BigInt::operator+(const BigInt& rhs) const {

  //create deep copies of the nums and rhs.nums 
  BigInt temp("0", rhs.myBase);
  temp.nums.pop_back();
  //create deep copy
  for(int i = 0; i < nums.size(); i++)  {
    temp.nums.push_back(nums[i]);
  }

  //call add function
  temp.add(rhs);

  return temp;


}

bool BigInt::operator<(const BigInt& rhs) const {
  //returns true if the rhs is larger than the left

  //check lengths
  if (nums.size() > rhs.nums.size())
  {
      return false;
  }
  else if (nums.size() < rhs.nums.size())
  {
      return true;
  }

  //check first digit 
  //if digits are of the same length
  //start at the back
  for (int i = nums.size() - 1; i >= 0; i--) {
    if (nums[i] > rhs.nums[i])  {
      return false;
    }
    else if (nums[i] < rhs.nums[i]) {
      return true;
    }
  }
  //if theyre equal return false
  return false;
}



BigInt BigInt::operator-(const BigInt& rhs) const {
  //create deep copies of the two then reverse the add function
  BigInt left("0", myBase);
  BigInt right("0", myBase);

  //create deep copy of the lhs
  left.nums.pop_back();
  right.nums.pop_back();
  //cout << "nums size: IMP: " << nums.size() << endl;
  for (int i = 0; i < nums.size(); i++) {
    left.nums.push_back(nums[i]);
  }
  //cout << "after imp : " << left.nums.size() << endl;
  //create deep copy of the rhs
  for (int i = 0; i < rhs.nums.size(); i++) {
    right.nums.push_back(rhs.nums[i]);
  }
  
  int sizeFirst = left.nums.size();
  int sizeSecond = right.nums.size();

  //cout << "first size: "<< sizeFirst << endl;
  //cout << "seocnd size: " << sizeSecond << endl;
  bool firstLarger;

  if (sizeFirst < sizeSecond) {firstLarger = false;}
  else {firstLarger = true;}
  
  while (sizeFirst != sizeSecond) {
    if (firstLarger)  {right.nums.push_back(0);}
    else {left.nums.push_back(0);}
    sizeFirst = left.nums.size();
    sizeSecond = right.nums.size();
  }

  //cout << "LSH:" << left.to_string();
  //cout << "subtract " <<left.to_string() << " and " << right.to_string() << endl;


  //vectors are now same size
  
  vector<int> resultVect;

  vector<int> top;
  vector<int> bottom;
  //0 for lhs or 1 for rhs greater
  int largerVector = -1;
  if (left < right)  {
    top = right.nums;
    bottom = left.nums;
    largerVector = 1;
    
  }
  else {
    largerVector = 0;
    top = left.nums;
    bottom = right.nums;
  }

  int result;
  int carry = 0;

  for (int i =0; i < top.size(); i++)  {
    //if top is larger
    if (carry == -1) {
      //cout << "carried over" << endl;
      top[i]-= 1;
    }
    carry = 0;
    if (top[i] >= bottom[i])  {
      result = top[i] - bottom[i];
    }
    //if top is ssmaller
    else if (top[i] < bottom[i])  {
      result = top[i] - bottom[i] + 10;
      carry = -1;
    }
    //cout << "result:" << result << endl;
    resultVect.push_back(result);
  }

  BigInt resultBigInt("0", myBase);
  resultBigInt.nums.pop_back();
  for (int i = 0; i < resultVect.size(); i++) {
    resultBigInt.nums.push_back(resultVect[i]);
    //cout << "BIGINT: " << resultBigInt.nums[i] << endl;
    //cout << "SIZE: " <<resultBigInt.nums.size() << endl;
  }

    resultBigInt.removeLeadingZeroes();
    return resultBigInt;
}

