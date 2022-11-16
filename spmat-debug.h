#ifndef SPMATDEBUG_H
#define SPMATDEBUG_H
#include <set>

// forward declarations to avoid #include "spmat.h" since
// we are only using pointers
class SparseMatrix;

// Debugger assumes all creation happens in SparseMatrix::createSparseItem()
// and all items are destroyed in SparseMatrix::deleteNode()
class SparseMatrixDebugHelper
{
    std::set<void*> items_;
public:
    void addItem(void* item);
    void deleteItem(void* item);
    /**
     * @brief Validates if pointers in the linked list refer to items that 
     *        were actually created and that previous pointers really do
     *        point to the previous item in the list. Will throw if an
     *        inconsistency is found.
     * 
     * @param matrix pointer to the SparseMatrix to check
     * @throws std::logic_error if an inconsistency is found. Can use
     *         what() for a more informative message
     */
    void checkConsistency(const SparseMatrix* matrix) const;
};
#endif

