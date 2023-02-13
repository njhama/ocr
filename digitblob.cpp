#include "digitblob.h"
#include <iostream>
// Add more includes if necessary
#include <vector>

using namespace std;

// TO DO - Complete this function
bool Location::operator<(const Location& other) const
{
    //returns true if the left hand side is further left (col is smalelr) than the rhs
    if (col < other.col)    {return true;}
    else if (col == other.col and row < other.row)  {return true;}
    return false;

}

// TO DO - Complete this function
DigitBlob::DigitBlob()
{
    img_ = NULL;
    digit_ = '!'; // dummy value
    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // ul_'s Location default constructor already initializes it to -1,-1

    // Initilaize h_ and w_ and any other data members
    h_ = 0;
    w_ = 0;

}

// TO DO - Complete this function
DigitBlob::DigitBlob(uint8_t** img, Location upperleft, int height, int width)
{
    img_ = img;
    digit_ = '!'; // dummy value

    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // Initilaize ul_, h_ and w_ and any other data members
    ul_ = upperleft;
    h_ = width;
    w_ = height;

}

// TO DO - Complete this function if necessary
DigitBlob::~DigitBlob()
{
    // Add code if it is necessary
    
}

// TO DO - Complete this function
void DigitBlob::classify()
{
    calc_bit_quads();
    calc_euler_number();
    // Call helper functions to calculate features
    calc_symmetry_vert();
    calc_symmetry_horiz();
    calc_thirds();
    // TO DO: use the results of helper functions to calculate features
    //    We suggest starting with the Euler number but you are free to
    //    change our structure
    calc_centers_of_mass();
    calc_aspect_ratio();
    calc_amt_symm_vert();
    calc_amt_symm_horiz();
    calc_amt_black();
    calc_middle_area();
    calc_distr_col();

    if(euler_  == -1)   {digit_ = '8';}
    else if (euler_ == 0)   {
      //test for 0
        if (verticalSymm >= 0.85) {digit_ = '0';}
        else if (LRsymm <= 0.75)  {digit_ = '4';}
        else if (thirds_top < thirds_mid and thirds_top < thirds_bot) {digit_ = '6';}
        else {digit_ = '9';}
    }
    //test for 1,2,3,5,7
    //,2,3,5
    else if (euler_ == 1)   {
      if (black_ratio <= 0.5 and TBsymm >= 1.5 and aspect_ratio < 1.7) {digit_ = '7';}
      //LRsymm is 0.232 for 3 arail
      else if (aspect_ratio >= 1.45 and verticalSymm >= 0.70 and middle_area <= 0.45 and LRsymm + TBsymm >= 1.8)  {digit_ = '1';}
      else if(verticalSymm + horizontalSymm > 1 and verticalSymm + horizontalSymm  < 1.4 and verticalSymm + horizontalSymm > 1 and LRsymm + horizontalSymm <= 1.2 and aspect_ratio >= 1.6)  {digit_ = '1';}
      else if (horizontalSymm < verticalSymm and verticalSymm >= 0.67) {digit_ = '3';}
      else if(middle_area < 0.255)  {digit_ = '2';}
      else {digit_ = '5';}
    }
    
}

// Complete - Do not alter
char DigitBlob::getClassification() const
{
    return digit_;
}

// TO DO - Complete this function
void DigitBlob::printClassificationResults() const
{
    //cout << "Digit blob at " << ul_.row << "," << ul_.col << " h=" << h_ << " w=" << w_ << endl;
    //cout << "Bit quads: 1, 2, D, 3, 4:";
    //cout << " " << bq1_ << " " << bq2_ << " " << bqd_;
    //cout << " " << bq3_ << " " << bq4_ << endl;
    //cout << "Euler number is " << euler_ << endl;
    // TO DO: Add any other couts to print classification test data
    
    
    cout << "Euler Number: " << euler_ << endl;
    cout << "Vertical Symm: " << verticalSymm << endl;
    cout << "horizontal Symm " << horizontalSymm << endl;
    cout << "LR Symm " << LRsymm << endl;
    cout <<"TB Symm " << TBsymm << endl;
    cout << "Thirds: Top:" << thirds_top << " Mid: " << thirds_mid << "Bot: " << thirds_bot << endl;
    cout << "Black ratio: " << black_ratio << endl;
    cout << "Aspect Ratio: " << aspect_ratio << endl;
    cout << "Mid " << middle_area << endl;


    cout << "****Classified as: " << digit_ << "\n\n" << endl;

}

// Complete - Do not alter
const Location& DigitBlob::getUpperLeft() const
{
    return ul_;
}

// Complete - Do not alter
int DigitBlob::getHeight() const
{
    return h_;
}

// Complete - Do not alter
int DigitBlob::getWidth() const
{
    return w_;
}

// Complete - Do not alter
bool DigitBlob::operator<(const DigitBlob& other)
{
    // Use Location's operator< for DigitBlob operator<
    return ul_ < other.ul_;
}

// Complete - Do not alter
void DigitBlob::calc_euler_number()
{
    euler_ = (bq1_ - bq3_ - 2*bqd_) / 4;
}

// TO DO - Complete this function to set bq1_, bq2_, etc.
void DigitBlob::calc_bit_quads()
{
  //int bq0_,bq1_,bq2_,bq3_,bq4_,bqd_;

  int sum = 0;
  //have the ul location
  for (int i = ul_.row -1; i < ul_.row + h_ + 1; i++)  {
    for (int j = ul_.col - 1; j < ul_.col + w_ + 1; j++)  {
      sum = 0;
      sum = img_[i][j] + img_[i+1][j] + img_[i][j+1] + img_[i+1][j+1];
      if (sum == 0) {bq4_++;}
      if (sum == 255) {bq3_++;}
      if (sum == 255* 3) {bq1_++;}
      if (sum == 255* 4) {bq0_++;}
      if (sum == 255* 2) {
        //check for diag and normal
        if (img_[i][j] == 0 and img_[i+1][j+1] == 0 or img_[i+1][j] == 0 and img_[i][j+1] == 0)   {bqd_++;}
        else {bq2_++;}
        }
    }
  }

}

// Add more private helper function implementations below
void DigitBlob::calc_symmetry_vert() {
    //calculates if there are more pixels on the top or bottom
    int top = 0;
    int bot = 0;


    for (int i = ul_.row; i < ul_.row + h_ / 2; i++)    {
        for (int j =ul_.col; j < ul_.col + w_; j++) {
            //cout << img_[i][j];
            if (img_[i][j] == 0)    {top++;}
        }
    }
    for (int i =  ul_.row + h_ / 2 ; i < ul_.row + h_ ; i++)    {
        for (int j =ul_.col; j < ul_.col + w_; j++) {
            if (img_[i][j] == 0)    {bot++;}
        }
    }
    //cout << "top: " << top << endl;
    //cout << "bottom : " << bot << endl;
    
    double divi = double(top) / bot;
    //cout << "TB: " << divi << endl;
    TBsymm = divi;
}

void DigitBlob::calc_symmetry_horiz() {
    int left = 0;
    int right = 0;

    for (int i = ul_.row; i < ul_.row + h_ + 1; i++)    {
        for (int j =ul_.col; j < ul_.col + w_ / 2 + 1; j++) {
            if (img_[i][j] == 0)    {left++;}
        }
    }

    for (int i = ul_.row; i < ul_.row + h_ + 1; i++)    {
        for (int j = ul_.col + w_ / 2 + 1; j < ul_.col + w_; j++) {
            if (img_[i][j] == 0)    {right++;}
        }
    }


    //cout << "left: " << left << endl;
    //cout << "right: " << right << endl;
    double divi = double(left) / right;
    //cout << "LR: " << divi << endl;
    LRsymm = double(left) / right; 
}

void DigitBlob::calc_thirds()   {
    int top = 0;
    int mid = 0;
    int bot = 0;

    //top
    for (int i = ul_.row; i < ul_.row +  h_/3 + 1;i++)    {
        for (int j =ul_.col; j < ul_.col + w_; j++) {
            if (img_[i][j] == 0)    {top++;}
        }
    }   

    //mid
    for (int i = ul_.row + h_/3 + 1 ; i < ul_.row +   2 * h_/3 + 1;i++)    {
        for (int j =ul_.col; j < ul_.col + w_; j++) {
            if (img_[i][j] == 0)    {mid++;}
        }
    }

    //bot
    for (int i = ul_.row +  2 * h_/3 + 1 ; i < ul_.row +  h_ +1;i++)    {
        for (int j =ul_.col; j < ul_.col + w_; j++) {
            if (img_[i][j] == 0)    {bot++;}
        }
    }


    //cout << "thirds: " << endl;
    //cout << "TOP : " << top << endl;
    //cout << "MID : " << mid << endl;
    //cout << "BOT : " << bot << endl;
    thirds_top = top;
    thirds_mid = mid;
    thirds_bot = bot;
}


void DigitBlob::calc_centers_of_mass()  {
    //sum of pix in a row * row # divide by total 3 of black

    int totalBlack = 0;
    int sum = 0;
    int rowSum = 0;

    for (int i = ul_.row; i < ul_.row + h_ + 1; i++)    {
        for (int j = ul_.col; j < ul_.col + w_ + 1; j++) {
            if (img_[i][j] == 0)    {
                rowSum++;
                totalBlack++;
            }
        }
        sum += rowSum * i;
        rowSum = 0;
    }   
    double center = double(sum) / totalBlack;
   // cout << "center of mass" << center;
    //cout << "space top " <<  center - ul_.row << endl;
    //cout << "space bot" << ul_.row + h_ + 1 - center;
}


void DigitBlob::calc_aspect_ratio() {
    //width / hegith

    //cout << "width: " << w_ << endl;
    //cout << "height: " << h_ << endl;
    //cout << "aspect ratio : " << double(h_) / w_ << endl;
    aspect_ratio = double(h_) / w_;
}

void DigitBlob::calc_amt_symm_vert()    {
    //across the midline ul.row + h_/2

    int countSymm = 0;
    int totalCount = 0;

    for (int i = ul_.row; i < ul_.row + h_ / 2; i++)  {
        for (int j = ul_.col; j < ul_.col + w_; j++)  {
            int dist = ul_.row + h_ / 2 - i;
            if (img_[i][j] == img_[i + 2 *dist - 1][j])    {
                countSymm++;
            }
            totalCount++;
        }
    }

    //cout << "Vrtical symm : " << double(countSymm) / totalCount << endl;
    verticalSymm = double(countSymm) / totalCount;
}


void DigitBlob::calc_amt_symm_horiz()    {
    //across the midline ul.row + h_/2
    //0.98 for 0 arial
    
    int countSymm = 0;
    int totalCount = 0;

   for (int i = ul_.row; i < ul_.row + h_; i++)  {
        for (int j = ul_.col; j < ul_.col + w_ / 2; j++)  {
            int dist = ul_.col + w_ / 2 - j ;
            if (img_[i][j] == img_[i][j + 2 *dist - 1])    {
                countSymm++;
            }
            totalCount++;
        }
    }

    //cout << "Horizontal symm: " << double(countSymm) / totalCount << endl;
    horizontalSymm =  double(countSymm) / totalCount;
    //cout << "total count: " << totalCount << endl;
}

//test that counts how many black pixels compared to 

void DigitBlob::calc_amt_black()  {
  int black = 0;
  int total = 0;


  for (int i = ul_.row; i < ul_.row + h_; i++)  {
    for (int j = ul_.col; j < ul_.col + w_; j++)  {
      if (img_[i][j] == 0)  {black++;}
      total++;
    }
  }

  //cout << "Black ratio : " << double(black) / total << endl;
  black_ratio = double(black) / total;
}

void DigitBlob::calc_middle_area()  {
  int count  = 0;
  int black_count = 0;
 

  for (int i = ul_.row + h_ / 4; i < ul_.row + 3 * h_/4 ; i++)  {
    for (int j = ul_.col; j < ul_.col + w_; j++)  {
      if (img_[i][j] == 0)  {black_count++;}
       count++;
    }
   
  }
  //cout << "MIDDLE AREA : "<< double(black_count) / count << endl;
  middle_area = double(black_count) / count ;
}

//heavily weight center 
void DigitBlob::calc_distr_col()  {
  int count =0;
  int blackCount =0;
  int count2 = 0;
  int blackCount2 = 0;
  for (int i = ul_.row; i < ul_.row + h_; i++)  {
    //just look at the center columns and calulate how much 
    for (int j = ul_.col + double(4 * w_) / 10; j <ul_.col + double(6* w_) / 10; j++)  {
      if (img_[i][j] == 0)  {blackCount++;}
      count++;
    }
  }

  for (int i = ul_.row; i < ul_.row + h_; i++)  {
    //just look at the center columns and calulate how much 
    for (int j = ul_.col + double(8 * w_) / 10; j <ul_.col + double(9* w_) / 10; j++)  {
      if (img_[i][j] == 0)  {blackCount2++;}
      count2++;
    }
  }

  //cout << "CENTER WEIGHT  : " << double(blackCount) / count << endl;
  //cout << "CENTER WEIGHT RIGHT : " << double(blackCount2) / count2 << endl;
}


