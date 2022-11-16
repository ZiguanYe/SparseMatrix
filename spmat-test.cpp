#include <iostream>
#include "spmat.h"
using namespace std;

int main()
{
    cout << "\n\n==================== m1 Tests =======================" << endl;
    SparseMatrix m1(4);
    SparseMatrix::Coord pts1[] =   
        { {0,2}, {0,3}, {1,1}, {1,2}, {2,0},
          {3,2}, {3,3}
        };
    double vals1[] = {2.6, 0.1, 2.3, 3.5, 5.1, 8.7, 4.3};
    for(size_t i = 0; i < sizeof(pts1)/sizeof(SparseMatrix::Coord); i++)
    {
        m1.set(pts1[i], vals1[i]);
    }
    cout << "Original" << endl;
    m1.print(std::cout);

    m1.set({0,0},1.8);
    cout << "Setting point 0,0 to 1.8" << endl;
    m1.print(std::cout);

    m1.set({1,2},0);
    cout << "Setting point 1,2 to 0" << endl;
    m1.print(std::cout);

    m1.copyDim({3,SparseMatrix::npos}, {SparseMatrix::npos,2});
    cout << "Copied row 3 to col 2" << endl;
    m1.print(std::cout);


    cout << "\n\n==================== M2 Tests =======================" << endl;

    SparseMatrix m2(1);
    cout << "Original" << endl;
    m2.print(std::cout);

    m2.set({0,0}, 1.2);

    cout << "Non-zero" << endl;
    m2.print(std::cout);

    cout << "Sum of row 0: " << m2.sumDim({0,SparseMatrix::npos}) << endl;
    cout << "Sum of col 0: " << m2.sumDim({SparseMatrix::npos,0}) << endl;

    m2.copyDim({0,SparseMatrix::npos}, {0, SparseMatrix::npos});
    cout << "Copied row 0 to row 0" << endl;
    m2.print(std::cout);

    m2.set({0,0},0);
    cout << "Delete a point by setting to 0" << endl;
    m2.print(std::cout);

    cout << "\n\n==================== m3 Tests =======================" << endl;
    SparseMatrix m3(10);
    SparseMatrix::Coord pts3[] =   
        { {0,1}, {7,5}, {0,5}, {0,3}, {0,8},
          {0,0}, {3,5}, {8,5}, {2,5}
        };
    for(size_t i = 0; i < sizeof(pts3)/sizeof(SparseMatrix::Coord); i++)
    {
        m3.set(pts3[i], pts3[i][0] + pts3[i][1]*.1);
    }
    cout << "Original" << endl;
    m3.print(std::cout);

    m3.set(pts3[8],0);
    cout << "Delete a point by setting to 0" << endl;
    m3.print(std::cout);

    cout << "Sum of row 0: " << m3.sumDim({0,SparseMatrix::npos}) << endl;
    cout << "Sum of col 5: " << m3.sumDim({SparseMatrix::npos,5}) << endl;

    m3.copyDim({0,SparseMatrix::npos}, {0,SparseMatrix::npos});
    cout << "Copied row 0 to row 0" << endl;
    m3.print(std::cout);

    m3.copyDim({0,SparseMatrix::npos}, {1,SparseMatrix::npos});
    cout << "Copied row 0 to row 1" << endl;
    m3.print(std::cout);

    m3.copyDim({SparseMatrix::npos,5}, {SparseMatrix::npos,6});
    cout << "Copied col 5 to 6" << endl;
    m3.print(std::cout);

    m3.copyDim({0,SparseMatrix::npos}, {SparseMatrix::npos,1});
    cout << "Copied row 0 to col 1" << endl;
    m3.print(std::cout);

    m3.copyDim({3,SparseMatrix::npos}, {SparseMatrix::npos,1});
    cout << "Copied row 3 to col 1" << endl;
    m3.print(std::cout);

    m3.copyDim({SparseMatrix::npos,8}, {3,SparseMatrix::npos});
    cout << "Copied col 8 to row 3" << endl;
    m3.print(std::cout);

    cout << "Getting location 9,9: (should be 0) " << m3.get({9,9}) << endl;
    cout << "Now getting location 10,10: (should throw) " << endl;
    try {
        m3.get({10,10});
        cout << "Should not print this -- should have thrown exception" << endl;
    }
    catch(std::invalid_argument&) {
        cout << "Correctly threw exception." << endl;
    }


    cout << "\n\n==================== m4 Tests =======================" << endl;
    SparseMatrix m4(4);
    SparseMatrix::Coord pts4[] =   
        { {0,1}, {0,3}
        };
    double vals4[] = {0.1, 0.3};
    for(size_t i = 0; i < sizeof(pts4)/sizeof(SparseMatrix::Coord); i++)
    {
        m4.set(pts4[i], vals4[i]);
    }
    cout << "Original" << endl;
    m4.print(std::cout);

    m4.copyDim({0,SparseMatrix::npos}, {SparseMatrix::npos,1});
    cout << "Copied row 0 to col 1" << endl;
    m4.print(std::cout);    
    return 0;
}