#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
#include "spmat.h"
using namespace std;

#ifdef SPARSE_DEBUG
#include "spmat-debug.h"    
// global instance of the debug helper
SparseMatrixDebugHelper debug;
#endif


// Define and initialize the constant static member npos here
const size_t SparseMatrix::npos = (size_t) - 1;


// ----------------------------------------------------------------------------
// ------------------ Coord struct Implementation -----------------------------
// ----------------------------------------------------------------------------

// Constructor
// COMPLETE 
SparseMatrix::Coord::Coord(size_t row, size_t col) :
    r(row), c(col)
{
}

// operator[] accessors
// To be completed - Must adhere it requirements in the .h
size_t& SparseMatrix::Coord::operator[](size_t index) {
    if (index==0)
    {
        return r;
    }
    else if (index==1)
    {
        return c;
    }
    else
    {
        throw std::out_of_range("out of range");
    }
}

size_t const& SparseMatrix::Coord::operator[](size_t index) const {
    if (index==0)
    {
        return r;
    }
    else if (index==1)
    {
        return c;
    }
    else
    {
        throw std::out_of_range("out of range");
    }
}

// operator== comparison function
// To be completed - Must adhere it requirements in the .h
bool SparseMatrix::Coord::operator==(const Coord& rhs) const {
    if ((rhs.r == r) && (rhs.c == c))
    {
        return true;
    }
    else
    {
        return false;
    }
}


// operator != comparison function
// COMPLETE
bool SparseMatrix::Coord::operator!=(const Coord& rhs) const
{
    return !operator==(rhs);
}

// ----------------------------------------------------------------------------
// ------------------ SparseItem struct Implementation ------------------------
// ----------------------------------------------------------------------------

// Constructor
// COMPLETE 
SparseMatrix::SparseItem::SparseItem(Coord coord, double v)
{
    this->val = v;
    this->coord = coord;
    this->prev[ROW] = this->prev[COL] = nullptr;
    this->next[ROW] = this->next[COL] = nullptr;
}

// ----------------------------------------------------------------------------
// ----------------- SparseMatrix class Implementation ------------------------
// ----------------------------------------------------------------------------

// Constructor
// To be completed - Must adhere to requirements in the .h
SparseMatrix::SparseMatrix(size_t n) : 
    /* Add code as needed */ n_(n)
{
    if(0U == n_) {
        throw std::invalid_argument("n must be greater than 0");
    }
    /* Add code as needed */
    //dynamically allocate double array of SparseItem, one for row and one for column
    for (size_t i=0; i<=1; i++)
    {
        lists_[i] = new SparseItem*[n];
    }
    
    //initialize all the pointers to nullptr
    //note that the total number of pointer is 2n
    for (size_t i=0; i<=1; i++)
    {
        for (size_t j=0; j<n; j++)
        {
            lists_[i][j] = nullptr;
        }
    }

}

// Destructor
// To be completed

SparseMatrix::~SparseMatrix() 
{
    for (size_t r=0; r<n_; r++)
    {
        SparseItem *curr = lists_[ROW][r];
        while (curr != nullptr)
        {
            //case where the next item is valid
            if (curr->next[ROW] != nullptr)
            {
                curr = curr->next[ROW];
                deleteNode(curr->prev[ROW]);
                //delete curr->prev[ROW];
            }
            //case where only one item is left (head->next[ROW] == nullptr)
            else
            {
                deleteNode(curr);
                break;
            }
        }
    }

    for (size_t i=0; i<=1; i++)
    {
        delete[] lists_[i];
    }

}

// COMPLETE
SparseMatrix::SparseItem* SparseMatrix::createSparseItem(const SparseMatrix::Coord& coord, double val)
{
    SparseItem* ptr = new SparseItem(coord, val);
#ifdef SPARSE_DEBUG    
    debug.addItem(ptr);
#endif
    return ptr;
}

// COMPLETE
SparseMatrix::SparseItem* SparseMatrix::lowerBound(size_t list, Coord target_coord ) const
{
    SparseItem* head = this->lists_[list][target_coord[list]];
    size_t target_index = target_coord[1-list];

    if(head == nullptr) {
        return nullptr;
    }
    else if( head->coord[1-list] > target_index ) {
        return nullptr;
    }
    else {
        while(  head->next[list] != nullptr && 
                head->next[list]->coord[1-list] <= target_index)
        {
            head = head->next[list];
        }
        return head;        
    }
}


// COMPLETED
double SparseMatrix::get(const Coord& coord) const
{
    if( coord[ROW] >= n_ || coord[COL] >= n_ ) {
        throw std::invalid_argument("Bad coordinates");
    }
    SparseItem* head = lowerBound(ROW, coord );
    if(nullptr == head || head->coord[COL] != coord[COL]){
        return 0;
    }
    return head->val;
}

// COMPLETED
void SparseMatrix::print(std::ostream &os) const
{
    os << setprecision(7);
    for(size_t r = 0; r < this->n_; r++) {
        SparseItem* ptr = this->lists_[ROW][r];
        size_t col = 0;
        while(ptr != NULL) {
            while(col < ptr->coord[COL]) {
                os << setw(8) << 0;
                col++;
            }
            os << setw(8) << ptr->val;
            col++;
            ptr = ptr->next[ROW];
        }
        while(col < this->n_) {
            os << setw(8) << 0;
            col++;
        }
        os << endl;
    }
}


// To be completed - Must adhere it requirements in the .h
void SparseMatrix::deleteNode(SparseItem* node)
{
    if(nullptr == node){
        throw std::invalid_argument("argument must not be null");
    }
    /* Add necessary code to update all other pointers */
    /* Note: calling delete is provided for you below  */
    //define the first item of the row and col where the node is in
    SparseItem *rhead = lists_[ROW][node->coord[ROW]];
    SparseItem *chead = lists_[COL][node->coord[COL]];
    
    //outer case: the node has following node in row and col
    // this means that next exists in both row and col
    if ((node->next[ROW]!=nullptr) && (node->next[COL]!=nullptr))
    {
        //case: node is the head of row and col
        //update the head of row and col
        if ((node==rhead) && (node==chead))
        {
            //update head
            lists_[ROW][node->coord[ROW]] = node->next[ROW];
            lists_[COL][node->coord[COL]] = node->next[COL];
            //delete node
            node->next[ROW]->prev[ROW] = nullptr;
            node->next[COL]->prev[COL] = nullptr;
            //note that head must be updated before deleting the node
        }
        //case: the node is head of row but has prev node in col
        else if ((node==rhead) && (node!=chead))
        {
            //update head for row
            lists_[ROW][node->coord[ROW]] = node->next[ROW];
            //address the row
            node->next[ROW]->prev[ROW] = nullptr;
            //update info for prev and next item
            node->next[COL]->prev[COL] = node->prev[COL];
            node->prev[COL]->next[COL] = node->next[COL];
        }
        //case: the node is head of col but has prev node in row
        else if ((node!=rhead) && (node==chead))
        {
            //update head for col
            lists_[COL][node->coord[COL]] = node->next[COL];
            //address the col
            node->next[COL]->prev[COL] = nullptr;
            //update info for prev and next item
            node->next[ROW]->prev[ROW] = node->prev[ROW];
            node->prev[ROW]->next[ROW] = node->next[ROW];
        }
        //case: the node has prev node in row and col
        //the node is strictly in between nodes
        else if ((node!=chead) && (node!=rhead))
        {
            //address the col
            node->prev[COL]->next[COL] = node->next[COL];
            node->next[COL]->prev[COL] = node->prev[COL];
            //address the row
            node->next[ROW]->prev[ROW] = node->prev[ROW];
            node->prev[ROW]->next[ROW] = node->next[ROW];
        }
    }
    //end of first outer case
    //outer case: the node is the end of the col/has next node in row
    else if ((node->next[ROW]!=nullptr) && (node->next[COL]==nullptr))
    {
        //the node is head of row and col
        //the node is the only entry in col/head of row/has next item in row
        if ((node==rhead) && (node==chead))
        {
            //head of col is now nullptr
            lists_[COL][node->coord[COL]] = nullptr;
            //update head of row
            lists_[ROW][node->coord[ROW]] = node->next[ROW];
            //address the row
            node->next[ROW]->prev[ROW] = nullptr;
        }
        //the node is head of row/has next item in row/the end of the col with valid prev
        else if ((node==rhead) && (node!=chead))
        {
            //update head for row
            lists_[ROW][node->coord[ROW]] = node->next[ROW];
            //address the row
            node->next[ROW]->prev[ROW] = nullptr;
            //address the col
            node->prev[COL]->next[COL] = nullptr;
        }
        //node is the only entry in col/in the middle of the row
        else if ((node!=rhead) && (node==chead))
        {
            lists_[COL][node->coord[COL]] = nullptr;
            node->prev[ROW]->next[ROW] = node->next[ROW];
            node->next[ROW]->prev[ROW] = node->prev[ROW];
        }
        //node is in the moddle of the row/has prev in col
        else if ((node!=chead) && (node!=rhead))
        {
            node->next[ROW]->prev[ROW] = node->prev[ROW];
            node->prev[ROW]->next[ROW] = node->next[ROW];
            node->prev[COL]->next[COL] = nullptr;
        }
        //end of second outer case
    }
    //outer case: the node is the end of row/has valid next in col
    else if ((node->next[ROW]==nullptr) && (node->next[COL]!=nullptr))
    {
        //case: the node is only entry in the row/first entry in col
        if ((node==rhead) && (node==chead))
        {
            //head of row is now nullptr
            lists_[ROW][node->coord[ROW]] = nullptr;
            //update head of col
            lists_[COL][node->coord[COL]] = node->next[COL];
            //delete the node
            node->next[COL]->prev[COL] = nullptr;
        }
        //case: the node is the only entry in row/in the middle of col
        else if ((node==rhead) && (node!=chead))
        {
            //update head ptr of the roll to nullptr
            lists_[ROW][node->coord[ROW]] = nullptr;
            //address the col
            node->prev[COL]->next[COL] = node->next[COL];
            node->next[COL]->prev[COL] = node->prev[COL];
        }
        //case: the node is first item in col/end of row
        else if ((node!=rhead) && (node==chead))
        {
            //update the head of col
            lists_[COL][node->coord[COL]] = node->next[COL];
            node->next[COL]->prev[COL] = nullptr;
            node->prev[ROW]->next[ROW] = nullptr;
        }
        //case: the node is the end of row/in the middle of col
        else if ((node!=chead) && (node!=rhead))
        {
            node->prev[COL]->next[COL] = node->next[COL];
            node->next[COL]->prev[COL] = node->prev[COL];
            node->prev[ROW]->next[ROW] = nullptr;
        }
        //end of third outer case
    }
    //outer case: the node is the end of row and col
    else if ((node->next[ROW]==nullptr) && (node->next[COL]==nullptr))
    {
        //case: node is only entry in row and col
        if ((node==rhead) && (node==chead))
        {
            //update the head only
            lists_[COL][node->coord[COL]] = nullptr;
            lists_[ROW][node->coord[ROW]] = nullptr;
        }
        // the node is only entry in row/has prev in col
        else if ((node==rhead) && (node!=chead))
        {
            //update the head ptr of the row to null ptr
            lists_[ROW][node->coord[ROW]] = nullptr;
            //address the col, make the prev node the last one of the col
            node->prev[COL]->next[COL] = nullptr;
        }
        //the node is only entry in col/has prev in row
        else if ((node!=rhead) && (node==chead))
        {
            lists_[COL][node->coord[COL]] = nullptr;
            node->prev[ROW]->next[ROW] = nullptr;
        }
        // the node is last item in row and col
        else if ((node!=chead) && (node!=rhead))
        {
            node->prev[ROW]->next[ROW] = nullptr;
            node->prev[COL]->next[COL] = nullptr;
        }
        //end of fourth outer case
    }


    /* This code should not be altered and should end this function */
#ifdef SPARSE_DEBUG    
    debug.deleteItem(node);
#endif
    delete node;
}

// To be completed - Must adhere it requirements in the .h
void SparseMatrix::set(const Coord& coord, double val)
{
    if ((coord[0]>=n_) || (coord[1]>=n_))
    {
        throw std::out_of_range("out of range");
    }
    
    //introduce a new variable to get the value of the current coordinate
    double temp = get(coord);
    
    //note that nothing happens if val==0 and temp==0, so nothing is addressed here
    //case start from val==0 but temp!=0
    //delete the node in this case
    if(val == 0 && temp != 0)
    {
        //note that since temp!=0, lowerbound is nor nullptr
        SparseItem *tbd = lowerBound(ROW, coord);
        deleteNode(tbd);
    }
    //case: a node is to be replaced
    else if (val != 0 && temp != 0)
    {
        //note that since temp!=0, lowerbound is nor nullptr
        SparseItem *replacenode = lowerBound(ROW, coord);
        //update the value
        replacenode->val = val;
    }
    //case: the node is added to a valid new position
    else if (val != 0 && temp == 0)
    {
        //create a new node with parameter
        SparseItem *newnode = createSparseItem(coord, val);
        
        //get head of row and col for checking position
        SparseItem *rhead = lists_[ROW][coord[ROW]];
        SparseItem *chead = lists_[COL][coord[COL]];
        
        //get lowerbound for the use of checking position
        SparseItem *rownode = lowerBound(ROW, coord);
        SparseItem *colnode = lowerBound(COL, coord);
        
        //case1: both row and col are empty
        if (rhead==nullptr && chead==nullptr)
        {
            //update the head of row and col to be newnode
            lists_[ROW][coord[ROW]] = newnode;
            lists_[COL][coord[COL]] = newnode;
        }
        //case2: col is empty but row is not
        else if (rhead!=nullptr && chead==nullptr)
        {
            //update head of col
            lists_[COL][coord[COL]] = newnode;
            
            //innercase: new node is head pos of col(in front of current chead)
            if (coord[COL] < rhead->coord[COL])
            {
                //update head
                lists_[ROW][coord[ROW]] = newnode;
                newnode->next[ROW] = rhead;
                rhead->prev[ROW] = newnode;
            }
            //innercase: the new node is in the middle of the row
            else if (rownode->next[ROW] != nullptr)
            {
                newnode->next[ROW] = rownode->next[ROW];
                rownode->next[ROW]->prev[ROW] = newnode;
                newnode->prev[ROW] = rownode;
                rownode->next[ROW] = newnode;
            }
            //innercase: the new node is the last item of the rpw
            else if (rownode->next[ROW] == nullptr)
            {
                rownode->next[ROW] = newnode;
                newnode->prev[ROW] = rownode;
            }
        }
        //case3: row is empty but col is not
        //do similar steps as case2
        else if (rhead==nullptr && chead!=nullptr)
        {
            //update head of row
            lists_[ROW][coord[ROW]] = newnode;
            
            //innercase: new node is head pos of col(in front of current chead)
            if (coord[ROW] < chead->coord[ROW])
            {
                //update head
                lists_[COL][coord[COL]] = newnode;
                newnode->next[COL] = chead;
                chead->prev[COL] = newnode;
            }
            //innercase: the new node is in the middle of the col
            //this means that lowerbound is not nullptr, thus the condition is valid
            else if (colnode->next[COL] != nullptr)
            {
                newnode->next[COL] = colnode->next[COL];
                colnode->next[COL]->prev[COL] = newnode;
                newnode->prev[COL] = colnode;
                colnode->next[COL] = newnode;
            }
            //innercase: the new node is the last item of the col
            else if (colnode->next[COL] == nullptr)
            {
                colnode->next[COL] = newnode;
                newnode->prev[COL] = colnode;
            }
        }
        //case4: both row and col are not empty
        else if (rhead!=nullptr && chead!=nullptr)
        {
            //9 innercases
            //c1: newnode be head of row and col
            if ((coord[ROW] < chead->coord[ROW]) && (coord[COL] < rhead->coord[COL]))
            {
                //update row
                lists_[ROW][coord[ROW]] = newnode;
                newnode->next[ROW] = rhead;
                rhead->prev[ROW] = newnode;
                //update col
                lists_[COL][coord[COL]] = newnode;
                newnode->next[COL] = chead;
                chead->prev[COL] = newnode;
            }
            //c2&3: newnode be head of row, but not head of col
            else if ((coord[COL] < rhead->coord[COL]) && (coord[ROW] > chead->coord[ROW]))
            {
                //update row
                lists_[ROW][coord[ROW]] = newnode;
                newnode->next[ROW] = rhead;
                rhead->prev[ROW] = newnode;
                //update col
                //c2: node in the middle of col
                if (colnode->next[COL] != nullptr)
                {
                    newnode->next[COL] = colnode->next[COL];
                    colnode->next[COL]->prev[COL] = newnode;
                    newnode->prev[COL] = colnode;
                    colnode->next[COL] = newnode;
                }
                //c3: node is the last of col
                else if (colnode->next[COL] == nullptr)
                {
                    colnode->next[COL] = newnode;
                    newnode->prev[COL] = colnode;
                }
            }
            //c4&5&6: newnode in the middle of the row
            else if ((coord[COL] > rhead->coord[COL]) && (rownode->next[ROW]!=nullptr))
            {
                //update row
                newnode->next[ROW] = rownode->next[ROW];
                rownode->next[ROW]->prev[ROW] = newnode;
                newnode->prev[ROW] = rownode;
                rownode->next[ROW] = newnode;
                
                //c4: newnode head of col
                if ((coord[ROW] < chead->coord[ROW]))
                {
                    //update col
                    lists_[COL][coord[COL]] = newnode;
                    newnode->next[COL] = chead;
                    chead->prev[COL] = newnode;
                }
                
                //c5&6: newnode in the middle/in the lasat of col
                else if (coord[ROW] > chead->coord[ROW])
                {
                    //c5: newnode in the middle of col
                    if (colnode->next[COL]!=nullptr)
                    {
                        newnode->next[COL] = colnode->next[COL];
                        colnode->next[COL]->prev[COL] = newnode;
                        newnode->prev[COL] = colnode;
                        colnode->next[COL] = newnode;
                    }
                    //c6: newnode last item in col
                    else if (colnode->next[COL]==nullptr)
                    {
                        colnode->next[COL] = newnode;
                        newnode->prev[COL] = colnode;
                    }
                }
            }
            //c7&8&9: newnode the last item of the row
            else if ((coord[COL] > rhead->coord[COL]) && (rownode->next[ROW]==nullptr))
            {
                //update row
                rownode->next[ROW] = newnode;
                newnode->prev[ROW] = rownode;
                
                //c7: newnode head of col
                if ((coord[ROW] < chead->coord[ROW]))
                {
                    //update col
                    lists_[COL][coord[COL]] = newnode;
                    newnode->next[COL] = chead;
                    chead->prev[COL] = newnode;
                }
                
                //c8&9: newnode in the middle/in the lasat of col
                else if (coord[ROW] > chead->coord[ROW])
                {
                    //c8: newnode in the middle of col
                    if (colnode->next[COL]!=nullptr)
                    {
                        newnode->next[COL] = colnode->next[COL];
                        colnode->next[COL]->prev[COL] = newnode;
                        newnode->prev[COL] = colnode;
                        colnode->next[COL] = newnode;
                    }
                    //c9: newnode last item in col
                    else if (colnode->next[COL]==nullptr)
                    {
                        colnode->next[COL] = newnode;
                        newnode->prev[COL] = colnode;
                    }
                }
            }
        }
    }



/* Leave these as the last lines of this function */
#ifdef SPARSE_DEBUG    
    try {
        debug.checkConsistency(this);
    }
    catch (std::logic_error& e) {
        cerr << e.what();
    }
#endif
}

// To be completed - Must adhere it requirements in the .h
//   Be sure to meet the run-time requirements
double SparseMatrix::sumDim(const Coord& coord) const
{
    if ((coord[0]>=n_ && coord[1]==npos) || (coord[1]>=n_ && coord[0]==npos))
    {
        throw std::out_of_range("out of range");
    }
    else if (coord[0]==npos && coord[1]==npos)
    {
        throw std::invalid_argument("only one can be npos");
    }
    else if (coord[0]!=npos && coord[1]!=npos)
    {
        throw std::invalid_argument("there should be one npos");
    }

    if (coord[1]==npos)
    {
        SparseItem *curr = lists_[ROW][coord[ROW]];
        
        if (curr == nullptr)
        {
            return 0;
        }
        
        double sum = curr->val;
        while (curr!=nullptr && curr->next[ROW]!=nullptr)
        {
            curr = curr->next[ROW];
            sum += curr->val;
        }
        return sum;
            
    }
    else
    {
        SparseItem *curr = lists_[COL][coord[1]];
        
        
        if (curr == nullptr)
        {
            return 0;
        }
        
        double sum = curr->val;
        while (curr!=nullptr && curr->next[COL]!=nullptr)
        {
            curr = curr->next[COL];
            sum += curr->val;
        }
        return sum;
        
    }
}

// To be completed - Must adhere it requirements in the .h
//   Be sure to meet the run-time requirements
void SparseMatrix::copyDim(const Coord& srcCoord, const Coord& dstCoord)
{
    // Ignore self-copy - leave these as the first lines of the function 
    if( dstCoord == srcCoord) {
        return;
    }

    // Add code to check for exception cases and, if valid, perform the copy
    // of the indicated dimension
    if ((srcCoord[0]==npos && srcCoord[1]==npos) || (dstCoord[0]==npos && dstCoord[1]==npos))
    {
        throw std::invalid_argument("only one can be npos");
    }
    else if ((srcCoord[0]!=npos && srcCoord[1]!=npos) || (dstCoord[0]!=npos && dstCoord[1]!=npos))
    {
        throw std::invalid_argument("there should be one npos");
    }
    else if ((srcCoord[0] >= n_ && srcCoord[1] == npos) || (srcCoord[1] >= n_ && srcCoord[0] == npos) ||
             (dstCoord[0] >= n_ && dstCoord[1] == npos) || (dstCoord[1] >= n_ && dstCoord[0] == npos))
    {
        throw std::out_of_range("out of range");
    }
    
    
    
    
    
    
    SparseItem *inter;
    double interval;
    
    //address intersection point, presave
    //row to col
    if ((srcCoord[COL]==npos) && (dstCoord[ROW]==npos))
    {
        //create new node to hold intersection point for future use
        inter = createSparseItem({srcCoord[ROW], dstCoord[COL]}, get({srcCoord[ROW], dstCoord[COL]}));
        interval = get({srcCoord[ROW], dstCoord[COL]});
    }
    else if ((srcCoord[ROW]==npos) && (dstCoord[COL]==npos))
    {
        //create new node to hold intersection point for future use
        inter = createSparseItem({dstCoord[ROW], srcCoord[COL]}, get({dstCoord[ROW], srcCoord[COL]}));
        interval = get({dstCoord[ROW], srcCoord[COL]});
    }
    
    //deleting process
    //delete target row
    if (dstCoord[COL] == npos)
    {
        //initialize to be head of the target row
        SparseItem *curr = lists_[ROW][dstCoord[ROW]];
        //the row is not empty
        if (curr != nullptr)
        {
            while (curr->next[ROW] != nullptr)
            {
                curr = curr->next[ROW];
                deleteNode(curr->prev[ROW]);
            }
            //delete the last curr if valid
            deleteNode(curr);
        }
    }
    //delete target col
    else if (dstCoord[ROW] == npos)
    {
        //initialize to be head of the target col
        SparseItem *curr = lists_[COL][dstCoord[COL]];
        //the row is not empty
        if (curr != nullptr)
        {
            while (curr->next[COL] != nullptr)
            {
                curr = curr->next[COL];
                deleteNode(curr->prev[COL]);
            }
            //delete the last curr if valid
            deleteNode(curr);
        }
    //deletion finished
    }
    
    //4 total cases: row to row/col to col/row to col/col to row
    //case: row to row
    if ((srcCoord[COL]==npos) && (dstCoord[COL]==npos))
    {
        //initialize curr to be head of source row
        SparseItem *curr = lists_[ROW][srcCoord[ROW]];
        while (curr != nullptr)
        {
            set({dstCoord[ROW], curr->coord[COL]}, curr->val);
            curr = curr->next[ROW];
        }
    }
    //case: col to col
    else if ((srcCoord[ROW]==npos) && (dstCoord[ROW]==npos))
    {
        //initialize curr to be head of source col
        SparseItem *curr = lists_[COL][srcCoord[COL]];
        while (curr != nullptr)
        {
            set({curr->coord[ROW], dstCoord[COL]}, curr->val);
            curr = curr->next[COL];
        }
    }
    //case: row to col
    else if ((srcCoord[COL]==npos) && (dstCoord[ROW]==npos))
    {
        cout << "this is the intersection value " << interval << endl;
        
        //initialize curr to be head of source row
        SparseItem *curr = lists_[ROW][srcCoord[ROW]];
        while (curr != nullptr)
        {
            set({curr->coord[COL], dstCoord[COL]}, curr->val);
            curr = curr->next[ROW];
        }
        
        set({inter->coord[COL], dstCoord[COL]}, interval);
        delete inter;

    }
    //case: col to row
    else if ((srcCoord[ROW]==npos) && (dstCoord[COL]==npos))
    {
        //create new node to hold intersection point for future use

        cout << "this is the intersection value " << inter->val << endl;
        
        //initialize curr to be head of source col
        SparseItem *curr = lists_[COL][srcCoord[COL]];
        while (curr != nullptr)
        {
            set({dstCoord[ROW], curr->coord[ROW]}, curr->val);
            curr = curr->next[COL];
        }
        
        set({dstCoord[ROW], inter->coord[ROW]}, interval);
        delete inter;
        
    }
    

/* Leave these as the last lines of this function */
#ifdef SPARSE_DEBUG    
    try {
        debug.checkConsistency(this);
    }
    catch (std::logic_error& e) {
        cerr << e.what();
    }
#endif
}

