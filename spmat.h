#ifndef SPMAT_H
#define SPMAT_H

#include <iostream>

// Models a 2D n x n sparse grid (i.e. locations with
// value 0 are not stored).
class SparseMatrix {
public:
    //use ROW and COL to replace 0 and 1 for better understanding
    enum Dim { ROW = 0, COL = 1, INVALID_DIM = 2};
    static const size_t npos;
    struct Coord {
        size_t r; /// row
        size_t c; /// column
        /// Constructor with default arugments of 0,0
        Coord(size_t row = 0, size_t col = 0); 

        /**
         * @brief Allows r and c member to be access with array
         *  indices 0 and 1, respectively.  May facilitate 
         *  more generic access to each member.
         * 
         * @param index 0 = row, 1 = column
         * @return size_t& reference to r or c member
         * @throw std::out_of_range if index is 2 or more
         */
        size_t& operator[](size_t index);
        size_t const & operator[](size_t index) const;

        /**
         * @brief Compares two Coord objects
         * 
         * @param rhs right hand side operand
         * @return true if the Coord objects are equal (not equal) for operator == (!=) 
         * @return false otherwise
         */
        bool operator==(const Coord& rhs) const;
        bool operator!=(const Coord& rhs) const;
    };

    /// Types and struct definitions
    struct SparseItem {
        double val;
        Coord coord;
        SparseItem* prev[2];
        SparseItem* next[2];
        SparseItem(Coord coord, double v);
    };

public:
    /**
     * @brief Intializes an n x n sparse matrix that is emtpy (all values of 0)
     * 
     * @param n 
     * @throws std::invalid_argument if n is 0
     */
    SparseMatrix(size_t n);

    /**
     * @brief Destroy the Sparse Matrix object
     * 
     */
    ~SparseMatrix();

    /**
     * @brief If the cell at the given coordinates does not exist (i.e. it is 
     * currently 0), create and insert the new cell provided the value
     * is not zero.  If the cell at the given coordinates does exists, 
     * update its value unless the value is 0 in which case the cell 
     * should be removed from the lists. Attempting to set a non-existent
     * cell (i.e. that is already implicitly 0) with an explicit value of 0
     * should simply have no effect.  
     * 
     * @note Must run in O(|r| + |c|) where |r| and |c| are the length of the 
     * items in the lists in the row and column dimension and not O(n)  
     * 
     * @param coord Coordinates of the cell to set. Index 0 is the row and 
     *              index 1 is the column
     * @param val Value to set the cell (0 indicates removal of the cell)
     * @throw std::out_of_range if either coordinate is not between 0 and n-1
     */
    void set(const Coord& coord, double val);

    /**
     * @brief Returns the value of the cell at the given coordinates
     * 
     * @param coord Coordinates of the cell to get. Index 0 is the row and 
     *              index 1 is the column
     * @return double Value of the cell (0 if the cell is not stored)
     * @throw std::out_of_range if either coordinate is not between 0 and n-1
     */
    double get(const Coord& coord) const;


    /**
     * @brief Returns the sum of a given row or column (i.e. dimension). 
     * 
     * @note Must run in O(k) where k is the length of the 
     * items in the specified dimension's list and not O(n)
     * 
     * @param coord Coordinate range to iterate over. The coordinates must 
     * contain exactly 1 value equal to SparseMatrix::npos representing the 
     * dimension to iterate over.  
     * @return double Sum of the given dimension
     * @throw std::invalid_argument if one coordinate value is not
     * SparseMatrix::npos
     * @throw std::out_of_range if the other coordinate is not
     *  between 0 and n-1
     */
    double sumDim(const Coord& coord) const;

    /**
     * @brief  Copies the source row or column to the destination row or 
     * column
     * 
     * @note Must run in O(max(k1,k2)^2) where k1 and k2 are the length of the 
     * items in the lists in the src and dest dimension and not O(n)  
     * 
     * @param srcCoord Coordinate of the source range to copy.
     * The coordinates must contain exactly 1 value equal to 
     * SparseMatrix::npos representing the dimension to iterate over. 
     * @param destCoord Coordinate of the destination range.
     * The coordinates must contain exactly 1 value equal to 
     * SparseMatrix::npos representing the dimension to iterate over. 
     * @throw std::invalid_argument if one coordinate value in either the
     * source or destination coordinates is not SparseMatrix::npos
     * @throw std::out_of_range if the other coordinate in either the
     * source or destination coordinates is not between 0 and n-1
     */
    void copyDim(const Coord& srcCoord, const Coord& dstCoord);

    /** 
     * @brief Prints the n x n matrix to the given output stream with 
     * precision of 7 digits and 8 spaces per cell (showing 0 valued)
     * cells.
     */
    void print(std::ostream &os) const;

private:
// Do not change the following lines which are needed for 
// automated testing
#ifdef CS104TEST
// Make public for unit testing purposes
public:
#endif
    /**
     * @brief Returns a pointer to the SparseItem in the given list dimension
     *        (ROW or COL) that is equal or less than the target coordinate 
     *        or NULL if either the list is empty or the target_coord would 
     *        come before the first item in the list.    
     * 
     * @pre list must be either ROW (0) or COL (1)
     * @param list  ROW (0) or COL (1) over which to iterate
     * @param target_coord Target row / column index 
     * @return SparseItem*  pointer to the item before the target_coord, or 
     *                      NULL if no item would come before the target_coord
     */
    SparseItem* lowerBound( size_t list, Coord target_coord ) const;

    /**
     * @brief Single "factory" method to centralize and abstract creation of 
     *        SparseItem objects to aide debugging.
     * 
     * @return SparseItem* 
     */
    SparseItem* createSparseItem(const SparseMatrix::Coord& coord, double val);

    /**
     * @brief Removes node from any lists it is a member of and abstracts freeing
     *        of the memory    
     * 
     * @param node  node to delete
     * @throws std::invalid_argument if node is nullptr
     */
    void deleteNode(SparseItem* node);

#ifdef SPARSE_DEBUG
    friend class SparseMatrixDebugHelper;
#endif


    // --------------------------------------------------------------
    //  Data Members
    // --------------------------------------------------------------
    // 2 linear arrays of head pointers for linked lists representing
    // rows and columns lists_[ROW][] should be head pointers to the rows and
    // lists_[COL][] should be head pointers to the columns
    // of row, column linked list head ptrs
    //  Dimensions are 2xn requiring dynamic allocation.
    size_t n_;
    SparseItem** lists_[2];
};

#endif
