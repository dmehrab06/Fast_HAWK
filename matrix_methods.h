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

D_matrix initUnitMatrix(int r, int c){

    D_matrix newMat;
    for(int i = 0; i < r; i++){
        std::vector<double> emptyRow;
        for(int j = 0; j < c; j++){
            if(i == j){
                emptyRow.push_back(1);
            } else {
                emptyRow.push_back(0);
            }
        }
        newMat.push_back(emptyRow);
    }

    return newMat;
}

D_matrix initMatWithRandom(int r, int c) {
    
    D_matrix newMat;
    int factor1 = 7, factor2 = 3;
    for (int i = 0; i < r; i++) {
        vector<double> row;
        for (int j = 0; j < c; j++) {
            double data = (((i + 1) * factor1) + (j + 1) * factor2) / 4.2;
            row.push_back(data);
            factor1 += (rand() % 13);
            factor2 += (rand() % 7);
        }
        newMat.push_back(row);
    }

    return newMat;
}

D_matrix multiply(D_matrix m1, D_matrix m2){
    D_matrix ans;
    for(int i = 0; i<m1.size(); ++i){
        std::vector<double>line(m2[0].size(),0);
        ans.push_back(line);
    }
    if(m1[0].size()!=m2.size()){
        cout<<"cannot multiply\n";
        return ans;
    }
    for(size_t i = 0; i<m1.size(); ++i){
        for(size_t j = 0; j<m2[0].size(); ++j){
            for(size_t k = 0; k<m1[0].size(); ++k){
                ans[i][j] = ans[i][j] + m1[i][k]*m2[k][j];
            }
        }
    }
    return ans;
}

// D_matrix inverse(D_matrix matOriginal, int n, bool &singular){
//     D_matrix matLower, matUpper;
//     luDecomposition(matOriginal, n, matLower, matUpper);

//     return inverseLU(matLower, matUpper, n, singular);
// }

// Doolittle algorithm
void luDecomposition(D_matrix matOriginal, int n, D_matrix &matLower, D_matrix &matUpper) { 
    
    matLower = initNewMatrix(n, n, 0);
    matUpper = initNewMatrix(n, n, 0);
    
    // Decomposing matrix into Upper and Lower 
    // triangular matrix 
    for (int i = 0; i < n; i++) { 
        // Upper Triangular 
        for (int k = i; k < n; k++) { 
            // Summation of L(i, j) * U(j, k) 
            double sum = 0; 
            for (int j = 0; j < i; j++) {
                sum += (matLower[i][j] * matUpper[j][k]); 
            }
            // Evaluating U(i, k) 
            matUpper[i][k] = matOriginal[i][k] - sum; 
        } 

        // Lower Triangular 
        for (int k = i; k < n; k++) { 
            if (i == k) 
                matLower[i][i] = 1; // Diagonal as 1 
            else { 

                // Summation of L(k, j) * U(j, i) 
                double sum = 0; 
                for (int j = 0; j < i; j++){
                    sum += (matLower[k][j] * matUpper[j][i]); 
                }

                // Evaluating L(k, i) 
                matLower[k][i] = (matOriginal[k][i] - sum) / matUpper[i][i]; 
            } 
        } 
    } 
} 

D_matrix inverse(D_matrix matOriginal, int n, bool &singular){

    D_matrix matLower, matUpper;
    luDecomposition(matOriginal, n, matLower, matUpper);

    D_matrix matInverse = initNewMatrix(n, n, 0);
    
    for(int inverse_col = 0; inverse_col < n; inverse_col++){

        vector<double> b;
        for(int j = 0; j < n; j++){
            if(inverse_col == j){
                b.push_back(1);
            } else {
                b.push_back(0);
            }
        }
        vector<double> y(n, 0);

        // Forward substitution. Solve: Ly=b
        y[0] = b[0];
        for(int original_row = 1; original_row < n; original_row++){
            double sum = 0;
            for(int original_col = 0; original_col < n; original_col++){
                sum += matLower[original_row][original_col] * y[original_col];
            }
            y[original_row] = b[original_row] - sum;
        }

        vector<double> x(n, 0);
        // Backward substitution. Solve: Ux=y
        x[n-1] = y[n-1] / matUpper[n - 1][n - 1];
        for(int original_row = n - 2; original_row > -1; original_row--){
            double sum = 0;
            for(int original_col = original_row + 1; original_col < n; original_col++){
                sum += matUpper[original_row][original_col] * x[original_col];
            }
            x[original_row] = (y[original_row] - sum) / matUpper[original_row][original_row];
        }

        for(int j = 0; j < n; j++){
            matInverse[j][inverse_col] = x[j];
        }
    }
    
    singular = false;
    return matInverse;
}

void printMatrix(D_matrix mm){
    for(size_t i = 0; i < mm.size(); ++i){
        for(size_t j = 0; j < mm[0].size(); ++j){
            cout << setw(10) << mm[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
    return;
}
