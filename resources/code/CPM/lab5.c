# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# define ROW 21
# define COL 2

double** loadPoints(const char* filename);
double *calculateH(double **points);
double *calculateLambda(double *h);
double *calculateD(double **points, double *h);
double *calculateMu(double *lambda);
double **createMatrix(double *lambda, double *mu);
double *gaussElimination(double **mat, double *bias);
double **calculateS(double *M, double *h, double **points);

int main(){
    double **points = loadPoints("points.txt"); // 0 - 20

    double *h = calculateH(points);  // 0 - 19
    double *d = calculateD(points, h);  // 1 - 19
    double *lambda = calculateLambda(h);  // 1 - 19
    double *mu = calculateMu(lambda);  // 1 - 19
    double **mat = createMatrix(lambda, mu);  // 19 * 19
    double *M = gaussElimination(mat, d);  // 0 - 20

    double **S = calculateS(M, h, points);
    for (int i = 0; i < ROW - 1; i++){
        printf("S_i(x) = (%.3lf)x^3 + (%.3lf)x^2 + (%.3lf)x + (%.3lf),\t x in [%.3lf, %.3lf]\n", S[i][3], S[i][2], S[i][1], S[i][0], points[i][0], points[i + 1][0]);
    }

    return 0;
}

double** loadPoints(const char* filename){
    double** points = (double**)malloc(ROW * sizeof(double*));
    for (int i = 0; i < ROW; i++){
        points[i] = (double*)malloc(COL * sizeof(double));
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL){
        fprintf(stderr, "无法打开文件 %s\n", filename);
        exit(1);
    }

    int row = 0;
    while (row < ROW && fscanf(file, "%lf %lf", &points[row][0], &points[row][1]) == 2){
        row++;
    }

    fclose(file);

    return points;
}

double *calculateH(double **points){
    double *h = (double *)malloc((ROW - 1) * sizeof(double));

    for (int i = 0; i < ROW - 1; i++){
        h[i] = points[i + 1][0] - points[i][0];
    }

    return h;
}

double *calculateD(double **points, double *h){
    double *d = (double *)malloc((ROW - 1) * sizeof(double));

    for (int i = 1; i < ROW - 1; i++){
        d[i] = (6 / (h[i] + h[i - 1])) * (((points[i + 1][1] - points[i][1]) / h[i]) - ((points[i][1] - points[i - 1][1]) / h[i - 1]));
    }

    return d;
}

double *calculateLambda(double *h){
    double *lambda = (double *)malloc((ROW - 1) * sizeof(double));

    for (int i = 1; i < ROW - 1; i++){
        lambda[i] = h[i] / (h[i] + h[i - 1]);
    }

    return lambda;
}

double *calculateMu(double *lambda){
    double *mu = (double *)malloc((ROW - 1) * sizeof(double));

    for (int i = 1; i < ROW - 1; i++){
        mu[i] = 1 - lambda[i];
    }

    return mu;
}

double **createMatrix(double *lambda, double *mu){
    double **mat = (double **)malloc((ROW - 1) * sizeof(double *));
    for (int i = 1; i < ROW - 1; i++){
        mat[i] = (double *)malloc((ROW - 1) * sizeof(double));
        for (int j = 1; j < ROW - 1; j++){
            if (i == j){
                mat[i][j] = 2;
            }else if (i == j + 1){
                mat[i][j] = mu[i];
            }else if (i == j - 1){
                mat[i][j] = lambda[i];
            }else{
                mat[i][j] = 0;
            }
        }
    }

    return mat;
}

double *gaussElimination(double **mat, double *bias){
    double *Result = (double *)malloc((ROW) * sizeof(double));

    for (int i = 1; i < ROW - 1; i++){
        for (int k = i + 1; k < ROW - 1; k++){
            double temp = mat[k][i];
            for (int j = i; j < ROW - 1; j++){
                mat[k][j] -= (mat[i][j] / mat[i][i]) * temp;
            }
            bias[k] -= (bias[i] / mat[i][i]) * temp;
        }
    }

    for (int i = ROW - 2; i >= 1; i--){
        if (i == ROW - 2){
            Result[i] = bias[i] / mat[i][i];
        }else{
            for (int j = i + 1; j < ROW - 1; j++){
                bias[i] -= mat[i][j] * Result[j];
            }
            Result[i] = bias[i] / mat[i][i];
        }
    }

    Result[0] = 0, Result[ROW - 1] = 0;
    return Result;
}

double **calculateS(double *M, double *h, double **points){
    double **Result = (double **)malloc((ROW - 1) * sizeof(double *));
    for (int i = 0; i < ROW - 1; i++){
        Result[i] = (double *)malloc(4 * sizeof(double));

        double x_i = points[i][0];
        double x_ip1 = points[i + 1][0];
        double y_i = points[i][1];
        double y_ip1 = points[i + 1][1];
        double h_i = h[i];

        double A = M[i] / (6 * h_i);
        double B = M[i + 1] / (6 * h_i);
        double C = (y_i / h_i) - (M[i] * h_i) / 6;
        double D = (y_ip1 / h_i) - (M[i + 1] * h_i) / 6;

        Result[i][3] = B - A;
        Result[i][2] = 3 * x_ip1 * A - 3 * x_i * B;
        Result[i][1] = 3 * x_i * x_i * B - 3 * x_ip1 * x_ip1 * A - C + D;
        Result[i][0] = x_ip1 * x_ip1 * x_ip1 * A - x_i * x_i * x_i * B + x_ip1 * C - D * x_i;
    }

    return Result;
}