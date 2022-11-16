#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "spmat-debug.h"
// now we #include the full definition of spmat.h
#include "spmat.h"

using namespace std;

void SparseMatrixDebugHelper::addItem(void* item)
{
    items_.insert(item);
}
void SparseMatrixDebugHelper::deleteItem(void* item)
{
    items_.erase(item);
}
void SparseMatrixDebugHelper::checkConsistency(const SparseMatrix* matrix) const
{
    // check row consistency
    for(size_t i = 0; i < matrix->n_; i++) {
        SparseMatrix::SparseItem* head = matrix->lists_[SparseMatrix::ROW][i];
        if(nullptr != head){
            if( items_.find(head) == items_.end() ){
                std::stringstream ss;
                ss << "Invalid pointer at head of row " << i << endl;
                // item at coordinates: " << head->coord[0] << "," << head->coord[1] << 
                throw std::logic_error(ss.str());
            }
            if( head->prev[SparseMatrix::ROW] != nullptr) {
                std::stringstream ss;
                ss << "Invalid prev row pointer in first item of row " << i << ". Should be nullptr" << endl;
                // item at coordinates: " << head->coord[0] << "," << head->coord[1] << 
                throw std::logic_error(ss.str());
            }
            while(nullptr != head->next[SparseMatrix::ROW]) {
                if( items_.find(head->next[SparseMatrix::ROW]) == items_.end() ){
                    std::stringstream ss;
                    ss << "Invalid next row pointer at coordinates: " 
                        << head->next[SparseMatrix::ROW]->coord[0] << "," 
                        << head->next[SparseMatrix::ROW]->coord[1] << endl;
                    throw std::logic_error(ss.str());
                }
                if( head != head->next[SparseMatrix::ROW]->prev[SparseMatrix::ROW] ){
                    std::stringstream ss;
                    ss << "Invalid prev row pointer at coordinates: " 
                        << head->next[SparseMatrix::ROW]->coord[0] << "," 
                        << head->next[SparseMatrix::ROW]->coord[1] << endl; 
                    throw std::logic_error(ss.str());
                }
                head = head->next[SparseMatrix::ROW];
            } // end while
        } // end if (nullptr != head)
    } // end for

    // check column consistency
    for(size_t i = 0; i < matrix->n_; i++) {
        SparseMatrix::SparseItem* head = matrix->lists_[SparseMatrix::COL][i];
        if(nullptr != head){
            if( items_.find(head) == items_.end() ){
                std::stringstream ss;
                ss << "Invalid pointer at head of col " << i << endl;
                throw std::logic_error(ss.str());
            }
            if( head->prev[SparseMatrix::COL] != nullptr) {
                std::stringstream ss;
                ss << "Invalid prev col pointer in first item of col " << i << ". Should be nullptr" << endl;
                // item at coordinates: " << head->coord[0] << "," << head->coord[1] << 
                throw std::logic_error(ss.str());
            }
            while(nullptr != head->next[SparseMatrix::COL]) {
                if( items_.find(head->next[SparseMatrix::COL]) == items_.end() ){
                    std::stringstream ss;
                    ss << "Invalid next col pointer at coordinates: " 
                        << head->next[SparseMatrix::COL]->coord[0] << "," 
                        << head->next[SparseMatrix::COL]->coord[1] << endl;
                    throw std::logic_error(ss.str());
                }
                if( head != head->next[SparseMatrix::COL]->prev[SparseMatrix::COL] ){
                    std::stringstream ss;
                    ss << "Invalid prev col pointer at coordinates: " 
                        << head->next[SparseMatrix::COL]->coord[0] << "," 
                        << head->next[SparseMatrix::COL]->coord[1] << endl; 
                    throw std::logic_error(ss.str());
                }
                head = head->next[SparseMatrix::COL];
            } // end while
        } // end if (nullptr != head)
    } // end for
}
