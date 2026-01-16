# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# define DimensionA 5
# define DimensionB 4
# define DimensionC 3
# define Epsilong 1e-5

typedef struct Matrix{
    int Dimension;
    double **OMat;
    double **LMat;
    double **UMat;
}matrix;

matrix InitMatrix(int Dimension, double InitData[][DimensionA]);
void Doolittle(matrix Mat);
void PrintMatrix(matrix Mat);
void SolveFunction(matrix Mat, double *Bias, double *Solution);
double InversePowerMethod(matrix Mat, double *InitialX, double *Vector, double epsilon);

int main(){
    double initDataA[DimensionA][DimensionA] = {
        { 1.0/9, 1.0/8, 1.0/7, 1.0/6, 1.0/5 },
        { 1.0/8, 1.0/7, 1.0/6, 1.0/5, 1.0/4 },
        { 1.0/7, 1.0/6, 1.0/5, 1.0/4, 1.0/3 },
        { 1.0/6, 1.0/5, 1.0/4, 1.0/3, 1.0/2 },
        { 1.0/5, 1.0/4, 1.0/3, 1.0/2, 1.0 }
    };
    double initDataB[DimensionA][DimensionA] = {
        {4.00, -1.0, 1.00, 3.0},
        {16.0, -2.0, -2.0, 5.0},
        {16.0, -3.0, -1.0, 7.0},
        {6.00, -4.0, 2.00, 9.0}
    };
    matrix A = InitMatrix(DimensionA, initDataA);
    matrix B = InitMatrix(DimensionB, initDataB);

    double *InitialAX = (double *)malloc(DimensionA * sizeof(double));
    for (int i = 0; i < DimensionA; i++){InitialAX[i] = 1;}
    double *InitialBX = (double *)malloc(DimensionB * sizeof(double));
    for (int i = 0; i < DimensionB; i++){InitialBX[i] = 1;}
    double *VectorA = (double *)malloc(DimensionA * sizeof(double));
    double *VectorB = (double *)malloc(DimensionB * sizeof(double));

    PrintMatrix(A);
    double lambdaA = InversePowerMethod(A, InitialAX, VectorA, Epsilong);
    printf("lambda = %.5lf \nX: (", lambdaA);
    for (int i = 0; i < A.Dimension; i++){
        printf("%.5lf, ", VectorA[i]);
    }
    printf(")\n\n");

    PrintMatrix(B);
    double lambdaB = InversePowerMethod(B, InitialBX, VectorB, Epsilong);
    printf("lambda = %.5lf \nX: (", lambdaB);
    for (int i = 0; i < B.Dimension; i++){
        printf("%.5lf, ", VectorB[i]);
    }
    printf(")\n");

    free(InitialAX), free(InitialBX), free(VectorA), free(VectorB);
    return 0;
}

matrix InitMatrix(int Dimension, double InitData[][DimensionA]){
    matrix NewMatrix;
    NewMatrix.Dimension = Dimension;
    NewMatrix.OMat = (double **)malloc(NewMatrix.Dimension * sizeof(double *));
    NewMatrix.LMat = (double **)malloc(NewMatrix.Dimension * sizeof(double *));
    NewMatrix.UMat = (double **)malloc(NewMatrix.Dimension * sizeof(double *));
    for (int i = 0; i < NewMatrix.Dimension; i++){
        NewMatrix.OMat[i] = (double *)malloc(NewMatrix.Dimension * sizeof(double));
        NewMatrix.LMat[i] = (double *)malloc(NewMatrix.Dimension * sizeof(double));
        NewMatrix.UMat[i] = (double *)malloc(NewMatrix.Dimension * sizeof(double));
    }

    for (int i = 0; i < Dimension; i++){
        for (int j = 0; j < Dimension; j++) {
            NewMatrix.OMat[i][j] = InitData[i][j];
        }
    }
    Doolittle(NewMatrix);
    return NewMatrix;
}

void PrintMatrix(matrix Mat){
    printf("Mat: \n");
    for (int i = 0; i < Mat.Dimension; i++){
        for (int j = 0; j < Mat.Dimension; j++){
            printf("%.2lf", Mat.OMat[i][j]);
            if ((j + 1) % Mat.Dimension != 0)printf("\t");
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < Mat.Dimension; i++){
        for (int j = 0; j < Mat.Dimension; j++){
            printf("%.2lf", Mat.LMat[i][j]);
            if ((j + 1) % Mat.Dimension != 0)printf("\t");
        }
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < Mat.Dimension; i++){
        for (int j = 0; j < Mat.Dimension; j++){
            printf("%.2lf", Mat.UMat[i][j]);
            if ((j + 1) % Mat.Dimension != 0)printf("\t");
        }
        printf("\n");
    }
    printf("\n");
}

void Doolittle(matrix Mat){
    for (int k = 0; k < Mat.Dimension; k++){
        Mat.LMat[k][k] = 1; // 对角线上的元素为1

        // 计算 U 矩阵的第 k 行
        for (int j = k; j < Mat.Dimension; j++){
            Mat.UMat[k][j] = Mat.OMat[k][j];
            for (int p = 0; p < k; p++) {
                Mat.UMat[k][j] -= Mat.LMat[k][p] * Mat.UMat[p][j];
            }
        }

        // 计算 L 矩阵的第 k 列
        for (int i = k + 1; i < Mat.Dimension; i++){
            Mat.LMat[i][k] = Mat.OMat[i][k];
            for (int p = 0; p < k; p++) {
                Mat.LMat[i][k] -= Mat.LMat[i][p] * Mat.UMat[p][k];
            }
            Mat.LMat[i][k] /= Mat.UMat[k][k];
        }
    }
}

void SolveFunction(matrix Mat, double *Bias, double *Solution){
    double *y = (double *)malloc(Mat.Dimension * sizeof(double));

    // 前代法求解 Ly = b
    for (int i = 0; i < Mat.Dimension; ++i) {
        double sum = 0;
        for (int j = 0; j < i; ++j) {
            sum += Mat.LMat[i][j] * y[j];
        }
        y[i] = Bias[i] - sum;
    }

    // 回代法求解 Ux = y
    for (int i = Mat.Dimension - 1; i >= 0; --i) {
        double sum = 0;
        for (int j = i + 1; j < Mat.Dimension; ++j) {
            sum += Mat.UMat[i][j] * Solution[j];
        }
        Solution[i] = (y[i] - sum) / Mat.UMat[i][i];
    }

    // 释放内存
    free(y);
}

double InversePowerMethod(matrix Mat, double *InitialX, double *Vector, double epsilon){
    double *XK = (double *)malloc(Mat.Dimension * sizeof(double));
    double *NextXK = (double *)malloc(Mat.Dimension * sizeof(double));
    double Max, Change; int flag = 0;

    // 初始化迭代向量
    for (int i = 0; i < Mat.Dimension; i++){XK[i] = InitialX[i];}

    do {
        Max = 0, Change = 0;
        for (int i = 0; i < Mat.Dimension; i++){Max = (fabs(XK[i]) > Max) ? fabs(XK[i]) : Max;}
        for (int i = 0; i < Mat.Dimension; i++){Vector[i] = XK[i] / Max;}
        SolveFunction(Mat, Vector, NextXK);
        printf("i = %d:\nX: ", flag++);
        for (int i = 0; i < Mat.Dimension; i++){printf("%.2lf & ", NextXK[i]);}
        printf("\nY: ");
        for (int i = 0; i < Mat.Dimension; i++){printf("%.2lf &  ", Vector[i]);}
        printf("\n\n");
        for (int i = 0; i < Mat.Dimension; i++){
            Change = (fabs(NextXK[i] - XK[i]) > Change) ? fabs(NextXK[i] - XK[i]) : Change;
        }
        for (int i = 0; i < Mat.Dimension; i++){XK[i] = NextXK[i];}
    }while(Change > epsilon);

    double lambda; Max = 0;
    for (int i = 0; i < Mat.Dimension; i++){
        Max = (fabs(NextXK[i]) > Max) ? fabs(NextXK[i]) : Max;
    }
    lambda = 1 / Max;
    free(XK), free(NextXK);
    return lambda;
}
