#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <cstring>
#include <vector>

using namespace std;
#define D_matrix std::vector<vector<double> >


D_matrix initNewMatrix(int r, int c, double val){

    D_matrix newMat;
    for(int i = 0; i < r; i++){
        std::vector<double> emptyRow(c, val);
        newMat.push_back(emptyRow);
    }

    return newMat;
}

D_matrix initMatWithRandom(int r, int c) {
    
    D_matrix newMat;
    int factor1 = 1, factor2 = 2;
    for (int i = 0; i < r; i++) {
        vector<double> row;
        for (int j = 0; j < c; j++) {
            double data = (((i + 1) * factor1) + (j + 1) * factor2) / 4.2;
            row.push_back(data);
            factor1 += (rand() % 5);
            factor2 += (rand() % 7);
        }
        newMat.push_back(row);
    }

    return newMat;
}


void getCofactor(D_matrix &matA, D_matrix &matTemp, int p, int q, int n) 
{ 
    int i = 0, j = 0; 
  
    // Looping for each element of the matrix 
    for (int row = 0; row < n; row++) { 
        for (int col = 0; col < n; col++) { 
            //  Copying into temporary matrix only those element 
            //  which are not in given row and column 
            if (row != p && col != q) { 
                matTemp[i][j++] = matA[row][col]; 
  
                // Row is filled, so increase row index and 
                // reset col index 
                if (j == n - 1) { 
                    j = 0; 
                    i++; 
                } 
            } 
        } 
    } 
}
 
// Recursive function for finding determinant of matrix. 
// n is current dimension of A[][]
double determinant(D_matrix &matA, int N) { 
    double det = 0; // Initialize result 
  
    // Base case: if matrix contains single element 
    if (N == 1) {
        return matA[0][0]; 
    }
  
    // to store cofactors 
    D_matrix matTemp = initNewMatrix(N, N, 0); 

    // to store sign multiplier
    int sign = 1;   

    // iterate for each element of first row 
    for (int f = 0; f < N; f++) {

        // getting Cofactor of A[0][f] 
        getCofactor(matA, matTemp, 0, f, N); 
        det += sign * matA[0][f] * determinant(matTemp, N - 1); 
  
        // terms are to be added with alternate sign 
        sign = -sign; 
    } 
  
    return det; 
} 
  
// Function to get adjoint of A[N][N] in adj[N][N]. 
void adjoint(D_matrix &matA, D_matrix &matAdj, int N) { 
    
    if (N == 1) { 
        matAdj[0][0] = 1; 
        return; 
    } 
  
    // matTemp is used to store cofactors of matA[][] 
    int sign = 1;
    D_matrix matTemp = initNewMatrix(N, N, 0); 
  
    for (int i = 0; i < N; i++) { 
        for (int j = 0; j < N; j++) {

            // Get cofactor of matA[i][j] 
            getCofactor(matA, matTemp, i, j, N); 
  
            // sign of adj[j][i] positive 
            // if sum of row and column indexes is even. 
            sign = ((i + j) % 2 == 0) ? 1 : -1; 
  
            // Interchanging rows and columns to get the 
            // transpose of the cofactor matrix 
            matAdj[j][i] = (sign)*(determinant(matTemp, N-1)); 
        } 
    } 
} 
  
// Function to calculate and return inverse, if not singular
// if singular, don't know what to do
D_matrix inverse(D_matrix &matA, int N, bool &singular) { 

    D_matrix matInverse = initNewMatrix(N, N, 0);

    // Find determinant of A[][] 
    double det = determinant(matA, N); 
    if (det == 0) { 
        cout << "Singular matrix, can't find its inverse"; 
        singular = true;

        // Check: What to do?
        return initNewMatrix(1, 1, 0); 
    } 

    // Find adjoint 
    D_matrix matAdj = initNewMatrix(N, N, 0); 
    adjoint(matA, matAdj, N); 
  
    // Find Inverse using formula "inverse(A) = adj(A)/det(A)" 
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matInverse[i][j] = matAdj[i][j] / double(det); 
        }
    }

    return matInverse; 
} 

void printMatrix(D_matrix mm){
    for(size_t i = 0; i < mm.size(); ++i){
        for(size_t j = 0; j < mm[0].size(); ++j){
            cout << mm[i][j] << " ";
        }
        cout << "\n";
    }
    return;
}
