# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <math.h>

# define IRIS_ROW 5
# define IRIS_COL 150
# define DATA_ROW 4

double **GenNormMatrix(int ROW, int COL);
void PrintMatrix(double** Matrix, int ROW, int COL);
double **Dot_Product(double **MatrixA, double **MatrixB, int ROW, int COLA, int COLB);
double **TransMatrix(double **Matrix, int ROW, int COL);
double **Jacobi(double **Matrix, int Dimension, double epsilon, double *eigenvalues);
void SVD(double **Mat, double **U, double **V, double **S, int ROW, int COL);
double **Jacobi_Gauss(double **Matrix, int Dimension, double epsilon, double *eigenvalues);
void Gauss_elimination(double **S_Matrix, double *solution, int Dimension);
void Swap_Matrix(double **Matrix, double *B, int row_A, int row_B, int Dimension);


int main(){
    printf("==================== 任务 (a) ====================\n");
    double **Mat = GenNormMatrix(4, 3);
    printf("随机矩阵：\n");
    PrintMatrix(Mat, 4, 3);
    double **U = (double **)malloc(4 * sizeof(double *));
    double **V = (double **)malloc(3 * sizeof(double *));
    double **S = (double **)malloc(4 * sizeof(double *));
    for (int i = 0; i < 4; i++){
        U[i] = (double *)malloc(4 * sizeof(double));
        S[i] = (double *)malloc(3 * sizeof(double));
        if (i < 3){
            V[i] = (double *)malloc(3 * sizeof(double));
        }
    }

    SVD(Mat, U, V, S, 4, 3);

    printf("奇异值分解结果：\nU 矩阵：\n");
    PrintMatrix(U, 4, 4);
    printf("Sigma 矩阵：\n");
    PrintMatrix(S, 4, 3);
    printf("V^T 矩阵：\n");
    PrintMatrix(TransMatrix(V, 3, 3), 3, 3);

    printf("==================== 任务 (b) ====================\n");
    FILE *fp  = fopen("./iris.txt", "r");
    if (fp == NULL) {
        printf("无法打开文件\n");
        return 1;
    }

    double **IrisDataT = (double **)malloc(IRIS_COL * sizeof(double *));
    for (int i = 0; i < IRIS_COL; i++){
        IrisDataT[i] = (double *)malloc(IRIS_ROW * sizeof(double));
    }

    char line[100];  // 用于存储读取的每行数据
    int row = 0;
    
    while (fgets(line, sizeof(line), fp) != NULL && row < IRIS_COL) {
        char *token = strtok(line, ",");  // 使用逗号作为分隔符

        int col = 0;
        while (token != NULL && col < IRIS_ROW) {
            IrisDataT[row][col] = atof(token);  // 将字符串转换为浮点数
            token = strtok(NULL, ",");

            col++;
        }

        row++;
    }

    fclose(fp);

    double **DataT = (double **)malloc(IRIS_COL * sizeof(double *));
    for (int i = 0; i < IRIS_COL; i++){
        DataT[i] = (double *)malloc(DATA_ROW * sizeof(double));
        for (int j = 0; j < DATA_ROW; j++){
            DataT[i][j] = IrisDataT[i][j];
        }
    }

    for (int i = 0; i < IRIS_COL; i++){
        double temp = 0;
        for (int j = 0; j < DATA_ROW; j++){
            temp += DataT[i][j];
        }
        temp /= IRIS_ROW;

        for (int j = 0; j < DATA_ROW; j++){
            DataT[i][j] -= temp;
        }
    }

    double **Data = TransMatrix(DataT, IRIS_COL, DATA_ROW);
    double **DDT = Dot_Product(Data, DataT, DATA_ROW, IRIS_ROW, DATA_ROW);

    for (int i = 0; i < DATA_ROW; i++){
        for (int j = 0; j < DATA_ROW; j++){
            DDT[i][j] /= IRIS_COL;
        }
    }

    printf("协方差矩阵：\n");
    PrintMatrix(DDT, DATA_ROW, DATA_ROW);
    double *Eigenvalues = (double *)malloc(DATA_ROW * sizeof(double));
    double **EigenVectors = Jacobi(DDT, DATA_ROW, 1e-6, Eigenvalues);

    double **Shadow = (double **)malloc(IRIS_COL * sizeof(double *));
    for (int i = 0; i < IRIS_COL; i++){
        Shadow[i] = (double *)malloc(3 * sizeof(double));
        for (int k = 0; k < 2; k++){
            Shadow[i][k] = 0;
            for (int j = 0; j < DATA_ROW; j++){
                Shadow[i][k] += EigenVectors[j][k] * Data[j][i];
            }
        }
        Shadow[i][2] = IrisDataT[i][4];
    }

    FILE *outputFile = fopen("projected_data.csv", "w");
    if (outputFile == NULL) {
        printf("无法打开输出文件\n");
        return 1;
    }

    for (int i = 0; i < IRIS_COL; i++) {
        for (int j = 0; j < 3; j++) {
            fprintf(outputFile, "%lf,", Shadow[i][j]);
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);

    return 0;
}

double **GenNormMatrix(int ROW, int COL){
    double **Matrix = (double **)malloc(ROW * sizeof(double*));
    for (int i = 0; i < ROW; i++){
        Matrix[i] = (double *)malloc(COL * sizeof(double));
    }
    srand((unsigned)time(NULL));
    for (int i = 0; i < ROW; i++){
        for (int j = 0; j < COL; j++){
            Matrix[i][j] = (double)rand()/RAND_MAX;
        }
    }
    return Matrix;
}

void PrintMatrix(double** Matrix, int ROW, int COL){
    for (int i = 0; i < ROW; i++){
        for (int j = 0; j < COL; j++){
            printf("%.3lf \t", Matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

double **Dot_Product(double **MatrixA, double **MatrixB, int ROW, int COLA, int COLB){
    double **Result = (double**)malloc(ROW * sizeof(double*));
    for (int i = 0; i < ROW; i++){
        Result[i] = (double*)malloc(COLB * sizeof(double));
        for (int j = 0; j < COLB; j++){
            Result[i][j] = 0;
        }
    }

    for(int i = 0; i < ROW; i++){
        for (int j = 0; j < COLA; j++){
            for (int k = 0; k < COLB; k++){
                Result[i][k] += MatrixA[i][j] * MatrixB[j][k];
            }
        }
    }
    return Result;
}

double **TransMatrix(double **Matrix, int ROW, int COL){
    double **Result = (double **)malloc(COL * sizeof(double *));
    for (int i = 0; i < ROW; i++){
        Result[i] = (double *)malloc(ROW * sizeof(double));
    }

    for (int i = 0; i < ROW; i++){
        for (int j = 0; j < COL; j++){
            Result[j][i] = Matrix[i][j];
        }
    }

    return Result;
}

double **Jacobi(double **Matrix, int Dimension, double epsilon, double *eigenvalues){
    double Copy[Dimension][Dimension], Bmat[Dimension][Dimension], error;
    double **eigenVectors = (double **)malloc(Dimension * sizeof(double *));
    for (int i = 0; i < Dimension; i++){
        eigenVectors[i] = (double *)malloc(Dimension * sizeof(double));
        for (int j = 0; j < Dimension; j++){
            Copy[i][j] = Matrix[i][j];
            Bmat[i][j] = 0;
            eigenVectors[i][j] = (i == j) ? 1 : 0;
        }
    }

    do{
        int p, q; double A = 0; error = 0;
        double **G = (double **)malloc(Dimension * sizeof(double *));
        for (int i = 0; i < Dimension; i++){
            G[i] = (double *)malloc(Dimension * sizeof(double));
            for (int j = 0; j < Dimension; j++){
                if (i != j && fabs(Copy[i][j]) > A){
                    A = fabs(Copy[i][j]);
                    p = i, q = j;
                }
                G[i][j] = (i == j) ? 1 : 0;
            }
        }

        double s = (Copy[q][q] - Copy[p][p]) / (2 * (Copy[p][q]));
        double t = (s == 0) ? 1 : ((fabs(-s + sqrt(s * s + 1)) < fabs(-s - sqrt(s * s + 1))) ? -s + sqrt(s * s + 1) : -s - sqrt(s * s + 1));
        double c = 1 / sqrt(1 + t * t), d = t / sqrt(1 + t * t);
        G[p][p] = c, G[p][q] = d, G[q][p] = -d, G[q][q] = c;
        eigenVectors = Dot_Product(eigenVectors, G, Dimension, Dimension, Dimension);

        Bmat[p][p] = Copy[p][p] - t * Copy[p][q];
        Bmat[q][q] = Copy[q][q] + t * Copy[p][q];
        Bmat[p][q] = 0 , Bmat[q][p] = 0;
        for (int i = 0; i < Dimension; i++){
            free(G[i]);
            if (i != p && i != q){
                Bmat[i][p] = c * Copy[p][i] - d * Copy[q][i];
                Bmat[i][q] = d * Copy[p][i] + c * Copy[q][i];
                Bmat[p][i] = Bmat[i][p], Bmat[q][i] = Bmat[i][q];
                for (int j = 0; j < Dimension; j++){
                    if (j != p && j != q){
                        Bmat[i][j] = Copy[i][j];
                    }
                }
            }
        }
        free(G);

        for (int i = 0; i < Dimension; i++){
            for (int j = 0; j < Dimension; j++){
                Copy[i][j] = Bmat[i][j];
                if (i != j) error += Bmat[i][j]*Bmat[i][j];
            }
        }
    }while (error > epsilon);

    for (int i = 0; i < Dimension; i++){
        eigenvalues[i] = Bmat[i][i];
    }

    for (int i = 0; i < Dimension - 1; i++){
        for (int j = 0; j < Dimension - 1 - i; j++){
            if (eigenvalues[j] < eigenvalues[j + 1]){
                double temp = eigenvalues[j];
                eigenvalues[j] = eigenvalues[j + 1];
                eigenvalues[j + 1] = temp;
                for (int k = 0; k < Dimension; k++){
                    temp = eigenVectors[k][j];
                    eigenVectors[k][j] = eigenVectors[k][j + 1];
                    eigenVectors[k][j + 1] = temp;
                }
            }
        }
    }

    return eigenVectors;
}

void SVD(double **Mat, double **U, double **V, double **S, int ROW, int COL){
    double **MatT = TransMatrix(Mat, ROW, COL);
    double **MMT = Dot_Product(Mat, MatT, ROW, COL, ROW);
    double **MTM = Dot_Product(MatT, Mat, COL, ROW, COL);

    double *eigenValuesMMT = (double *)malloc(ROW * sizeof(double));
    double *eigenValuesMTM = (double *)malloc(COL * sizeof(double));
    double **eigenVectorMatMMT = Jacobi_Gauss(MMT, ROW, 1e-6, eigenValuesMMT);
    double **eigenVectorMatMTM = Jacobi_Gauss(MTM, COL, 1e-6, eigenValuesMTM);
    double **Sigma = (double **)malloc(ROW * sizeof(double *));
    for (int i = 0; i < ROW; i++){
        Sigma[i] = (double *)malloc(COL * sizeof(double));
        for (int j = 0; j < COL; j++){
            Sigma[i][j] = (i == j) ? sqrt(eigenValuesMTM[i]) : 0;
        }
    }
    printf("AA^T 的 jacobi 方法结果：");
    PrintMatrix(&eigenValuesMMT, 1, ROW);
    
    for (int i = 0; i < ROW; i++){
        for (int j = 0; j < ROW; j++){
            U[i][j] = eigenVectorMatMMT[i][j];
        }
    }

    for(int i = 0; i < COL; i++){
        for (int j = 0; j < COL; j++){
            V[i][j] = eigenVectorMatMTM[i][j];
        }
    }

    for (int i = 0; i < ROW; i++){
        for (int j = 0; j < COL; j++){
            S[i][j] = Sigma[i][j];
        }
    }
}

void Swap_Matrix(double **Matrix, double *B, int row_A, int row_B, int Dimension){
    for (int col = 0; col < Dimension; col++){
        double temp = Matrix[row_A][col];
        Matrix[row_A][col] = Matrix[row_B][col];
        Matrix[row_B][col] = temp;
    }
    double temp = B[row_A];
    B[row_A] = B[row_B];
    B[row_B] = temp;
}

void Gauss_elimination(double **S_Matrix, double *solution, int Dimension){
    int index_row, index_col, index_temp;
    double *B = (double *)malloc(Dimension * sizeof(double));
    for (int i = 0; i < Dimension; i++){
        B[i] = 0;
    }

    for (index_col = 0; index_col < Dimension; index_col++){
        double Max_col = fabs(S_Matrix[index_col][index_col]);
        index_temp = index_col;
        
        for (index_row = index_col; index_row < Dimension; index_row++){
            if (fabs(S_Matrix[index_row][index_col]) > Max_col){
                Max_col = fabs(S_Matrix[index_row][index_col]);
                index_temp = index_row;
            }
        }

        if (index_temp != index_col) Swap_Matrix(S_Matrix, B, index_col, index_temp, Dimension);
        
        for (index_row = index_col + 1; index_row < Dimension; index_row++){
            double temp = - (S_Matrix[index_row][index_col]);
            for (int col = index_col; col < Dimension; col++){
                double scale = (S_Matrix[index_col][col] / S_Matrix[index_col][index_col]);
                S_Matrix[index_row][col] += scale * temp;
            }
            B[index_row] += B[index_col]/S_Matrix[index_col][index_col] * temp;
        }
    }

    for (index_row = Dimension - 1; index_row >= 0; index_row--){
        if (index_row == Dimension - 1){
            solution[index_row] = 1.0;
        }else{
            double solution_temp = B[index_row];
            for (int col = index_row + 1; col < Dimension; col++){
                solution_temp -= S_Matrix[index_row][col] * solution[col];
            }
            solution[index_row] = solution_temp / S_Matrix[index_row][index_row];
        }
    }

    free(B);
}

double **Jacobi_Gauss(double **Matrix, int Dimension, double epsilon, double *eigenvalues){
    double Copy[Dimension][Dimension], Bmat[Dimension][Dimension], error;
    double **eigenVectors = (double **)malloc(Dimension * sizeof(double *));
    for (int i = 0; i < Dimension; i++){
        eigenVectors[i] = (double *)malloc(Dimension * sizeof(double));
        for (int j = 0; j < Dimension; j++){
            Copy[i][j] = Matrix[i][j];
            Bmat[i][j] = 0;
            eigenVectors[i][j] = 0;
        }
    }

    do{
        int p, q; double A = 0; error = 0;
        for (int i = 0; i < Dimension; i++){
            for (int j = 0; j < Dimension; j++){
                if (i != j && fabs(Copy[i][j]) > A){
                    A = fabs(Copy[i][j]);
                    p = i, q = j;
                }
            }
        }

        double s = (Copy[q][q] - Copy[p][p]) / (2 * (Copy[p][q]));
        double t = (s == 0) ? 1 : ((-s + sqrt(s * s + 1) > -s - sqrt(s * s + 1)) ? -s + sqrt(s * s + 1) : -s - sqrt(s * s + 1));
        double c = 1 / sqrt(1 + t * t), d = t / sqrt(1 + t * t);

        Bmat[p][p] = Copy[p][p] - t * Copy[p][q];
        Bmat[q][q] = Copy[q][q] + t * Copy[p][q];
        Bmat[p][q] = 0 , Bmat[q][p] = 0;
        for (int i = 0; i < Dimension; i++){
            if (i != p && i != q){
                Bmat[i][p] = c * Copy[p][i] - d * Copy[q][i];
                Bmat[i][q] = d * Copy[p][i] + c * Copy[q][i];
                Bmat[p][i] = Bmat[i][p], Bmat[q][i] = Bmat[i][q];
                for (int j = 0; j < Dimension; j++){
                    if (j != p && j != q){
                        Bmat[i][j] = Copy[i][j];
                    }
                }
            }
        }

        for (int i = 0; i < Dimension; i++){
            for (int j = 0; j < Dimension; j++){
                Copy[i][j] = Bmat[i][j];
                if (i != j) error += Bmat[i][j]*Bmat[i][j];
            }
        }
        printf("非对角元元素平方和：%lf\n", error);
    }while (error > epsilon);

    for (int i = 0; i < Dimension; i++){
        eigenvalues[i] = Bmat[i][i];
    }

    for (int i = 0; i < Dimension - 1; i++){
        for (int j = 0; j < Dimension - 1 - i; j++){
            if (eigenvalues[j] < eigenvalues[j + 1]){
                double temp = eigenvalues[j];
                eigenvalues[j] = eigenvalues[j + 1];
                eigenvalues[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < Dimension; i++){
        double **Smat = (double **)malloc(Dimension * sizeof(double *));
        for (int j = 0; j < Dimension; j++){
            Smat[j] = (double *)malloc(Dimension * sizeof(double));
            for (int k = 0; k < Dimension; k++){
                Smat[j][k] = (j == k) ? Matrix[j][k] - eigenvalues[i] : Matrix[j][k];
            }
        }
        Gauss_elimination(Smat, eigenVectors[i], Dimension);
    }
    eigenVectors = TransMatrix(eigenVectors, Dimension, Dimension);
    for (int j = 0; j < Dimension; j++){
        double temp = 0;
        for (int i = 0; i < Dimension; i++){
            temp += eigenVectors[i][j] * eigenVectors[i][j];
        }
        for (int i = 0; i < Dimension; i++){
            eigenVectors[i][j] /= sqrt(temp);
        }
    }
    
    return eigenVectors;
}