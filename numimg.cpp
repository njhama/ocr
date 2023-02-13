#include "digitblob.h"
#include "numimg.h"
#include "bmplib.h"
#include <algorithm> // for std::sort
#include <deque>
#include <iomanip>
#include <iostream>

using namespace std;

// TO DO: Complete this function
NumImg::NumImg(const char* bmp_filename)
{
    //  Note: readGSBMP dynamically allocates a 2D array 
    //    (i.e. array of pointers (1 per row/height) where each  
    //    point to an array of unsigned char (uint8_t) pixels)

    // TO DO:
    // call readGSBMP to initialize img_, h_, and w_;

    img_ = readGSBMP(bmp_filename, h_, w_);
    // Leave this check
    if(img_ == NULL) {
        throw std::logic_error("Could not read input file");
    }
    
    // Convert to Black and White using a fixed threshold 
    for(int i =0; i < h_; i++){
        for(int j = 0; j < w_; j++){
            if(img_[i][j] > 150){
                img_[i][j] = 255;
            }
            else {
                img_[i][j] = 0;
            }
        }
    }
    // Perform any other initialization you need
}

// TO DO: Complete this function
NumImg::~NumImg()
{
    // Add code here if necessary
   for (int i = 0; i < h_;  i++)  {
     delete [] img_[i];
   }
  delete [] img_;
}

// TO DO: Complete this function
//return num digitblobs
size_t NumImg::findAndCreateDigitBlobs()
{
    
  bool** visited = new bool*[h_] ;
  for (int i = 0; i < h_; i++)  {
    bool* visited_rows = new bool[w_];
    visited[i] = visited_rows;
  }
    
  //initalize array
  for (int i = 0; i < h_; i++)  {
    for (int j = 0; j < w_; j++)  {
        visited[i][j] = 0;
    }
  }
   
    //deque<Location> myDeq;

    int min_row = h_;
    int min_col = w_;
    int max_row = 0;
    int max_col = 0;
    
    for (int j = 0; j < w_; j++)  { 
      for(int i = 0; i < h_; i++) {
        
            //check for black pixel that hasnt been visited
            if(img_[i][j] == 0 and visited[i][j] == 0) {
                DigitBlob newBlob = createDigitBlob(visited, i, j);
                blobs_.push_back(newBlob);
            }               
        }
    }
   for (int i = 0; i < h_; i++)  {
      delete [] visited[i];
    }
    delete [] visited;


}



// Complete - Do not alter
std::string NumImg::classify(bool withDebug)
{
    std::string res;
    for(size_t i = 0; i < blobs_.size(); i++){
        blobs_[i].classify();
        if(withDebug){
            blobs_[i].printClassificationResults();
        }
        char c = blobs_[i].getClassification();
        res += c;
    }
    return res;
}

// Complete - Do not alter
void NumImg::printBoundingBoxes() const
{
    cout << setw(2) << "i" << setw(6) << "ULRow" << setw(6) << "ULCol" << setw(4) << "Ht." << setw(4) << "Wi." << endl;
    for(size_t i = 0; i < blobs_.size(); i++){
        const DigitBlob& b =     blobs_[i];
        cout << setw(2) << i << setw(6) << b.getUpperLeft().row << setw(6) << b.getUpperLeft().col 
        << setw(4) << b.getHeight() << setw(4) << b.getWidth()  << endl;
        // cout << "Blob " << i << " ul=(" << b.getUpperLeft().row << "," << b.getUpperLeft().col 
        //     << ") h=" << b.getHeight() << " w=" << b.getWidth() << endl;
    }

}

// Complete - Do not alter
const DigitBlob& NumImg::getDigitBlob(size_t i) const
{
    if(i >= blobs_.size()){
        throw std::out_of_range("Index to getDigitBlob is out of range");
    }
    return blobs_[i];
}

// Complete - Do not alter
size_t NumImg::numDigitBlobs() const
{
    return blobs_.size();
}

// Complete - Do not alter
void NumImg::sortDigitBlobs()
{
    std::sort(blobs_.begin(), blobs_.end());
}

// Complete - Do not alter
void NumImg::drawBoundingBoxesAndSave(const char* filename)
{
    for(size_t i=0; i < blobs_.size(); i++){
        Location ul = blobs_[i].getUpperLeft();
        int h = blobs_[i].getHeight();
        int w = blobs_[i].getWidth();
        for(int i = ul.row-1; i < ul.row + h + 1; i++){
            img_[i][ul.col-1] = 128;
            img_[i][ul.col+w] = 128;
        }
        for(int j = ul.col-1; j < ul.col + w + 1; j++){
            img_[ul.row-1][j] = 128;
            img_[ul.row+h][j] = 128;
        }
    }
    writeGSBMP(filename, img_, h_, w_);
}

// TO DO:
// Add other (helper) function definitions here

DigitBlob NumImg::createDigitBlob(bool** explored, int pr, int pc)
//pr and pc starting r
{
    // Arrays to help produce neighbors easily in a loop
    // by encoding the **change** to the current location.
    // Goes in order N, NW, W, SW, S, SE, E, NE
    int neighbor_row[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int neighbor_col[8] = {0, -1, -1, -1, 0, 1, 1, 1};

    // Add your code here
    int min_row = pc;
    int min_col = pr;
    int max_col = pr;
    int max_row = pc;
    deque<Location> myDeq;
    //mark the start location as explored
    explored[pr][pc] = 1;

    //add start location to q
    Location start(pr, pc);
    myDeq.push_back(start);
                 
    //while q is not empty do:
    while (not myDeq.empty())   {
    //set loc = extract the item from the front of q
        Location current(myDeq.front().col, myDeq.front().row);
        myDeq.pop_front();
        if (current.col < min_col) {min_col = current.col;}
        if (current.col > max_col) {max_col = current.col;}
        if (current.row < min_row) {min_row = current.row;}
        if (current.row > max_row) {max_row = current.row;}
        //for each neighbor (i.e., N,NW,W,SW,S,SE,E, and NE) of loc do:
        for (int i = 0; i < 8; i++) {
            if(img_[current.col + neighbor_col[i]][current.row + neighbor_row[i]] == 0 and explored[current.col + neighbor_col[i]][current.row + neighbor_row[i]] == 0)  {
                if (current.col + neighbor_col[i] < min_col) {min_col = current.col + neighbor_col[i];}
                if (current.col + neighbor_col[i] > max_col) {max_col = current.col + neighbor_col[i];}
                if (current.row + neighbor_row[i] < min_row) {min_row = current.row + neighbor_row[i];}
                if (current.row + neighbor_row[i] > max_row) {max_row = current.row + neighbor_row[i];}
                                                     
                explored[current.col + neighbor_col[i]][current.row + neighbor_row[i]] = 1;
                myDeq.push_back(Location(current.col + neighbor_col[i],current.row + neighbor_row[i]));
            }
        }
    }
    Location upperleft(min_col, min_row);
    int height = max_row - min_row + 1;
    int width = max_col - min_col + 1;

    //cout << "NEW BLOB" << endl;
    //cout << "width = " << width << "max " << max_col << " min: " << min_col << endl;
    //cout << "height: " << height << "max  " << max_row << "  min " << min_row<<  endl;
    //cout << endl;
    DigitBlob temp(img_,upperleft, height, width);
    return temp;


}

