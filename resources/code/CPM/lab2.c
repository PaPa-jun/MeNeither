# include <stdio.h>
# include <math.h>

#define Demention 99

void Write_Matrix(double Matrix[][Demention], double epsilon, double h);
void Gauss_elimination(double S_Matrix[][Demention], double solution[], double B[]);
void Print_Matrix(double Matrix[][Demention], double B[]);
void Swap_Matrix(double Matrix[][Demention], double B[], int row_A, int row_B);
void Exact_solution(double y[], double x[], double a, double epsilon);
void Gauss_Seidel(double S_Matrix[][Demention], double B[], double initial_X[], double Max_error, double Result[]);
void Vector_Minus(double V[], double U[], double Result[]);
double Infinite_Norm(double V[]);
double ComputeRMSE(double numerical_solution[], double exact_solution[], int num_points);

int main(){
    double S_Matrix[Demention][Demention], B[Demention], x[Demention], y[Demention], epsilon[4] = {1.0, 0.1, 0.01, 0.0001}, Solution_Elimination[Demention];
    double  initial_X[Demention], Solution_Ite[Demention];
    int n = 100;
    double a = 1.0/2.0, h = 1.0/n;
    for (int i = 0 ; i < Demention; i++){
        x[i] = (i+1) * h;
        B[i] = a * h * h;
        initial_X[i] = 0;
    }

    for (int i = 0; i < 4; i++){
        B[Demention - 1] = a * h * h - (epsilon[i] + h);
        Exact_solution(y, x, a, epsilon[i]);
        Write_Matrix(S_Matrix, epsilon[i], h);
        printf("epsilon = %lf\n", epsilon[i]);

        printf("Exact Solution: \n");
        for (int j = 0; j < Demention; j++){
            printf("%.3f \t", y[j]);
            if ((j + 1)%9 == 0) printf("\n");
        }

        Gauss_elimination(S_Matrix, Solution_Elimination, B);
        double Elimination_rmse = ComputeRMSE(Solution_Elimination, y, Demention);

        Gauss_Seidel(S_Matrix, B, initial_X, 1e-4, Solution_Ite);
        double Iteration_rmse = ComputeRMSE(Solution_Ite, y, Demention);

        printf("Elimination Solution: \n");
        for (int j = 0; j < Demention; j++){
            printf("%.3f \t", Solution_Elimination[j]);
            if ((j + 1)%9 == 0) printf("\n");
        }
        printf("Elimination error: %.5f\n", Elimination_rmse);

        printf("Iteration Solution: \n");
        for (int j = 0; j < Demention; j++){
            printf("%.3f \t", Solution_Ite[j]);
            if ((j + 1)%9 == 0) printf("\n");
        }
        printf("Iteration error: %.5f\n\n", Iteration_rmse);
    }
    return 0;
}

void Exact_solution(double y[], double x[], double a, double epsilon){
    for (int i = 0 ; i < Demention; i++){
        y[i] = (1 - a) / (1 - exp(-(1 / epsilon))) * (1 - exp(-(x[i] / epsilon))) + a * x[i];
    }
}

void Write_Matrix(double Matrix[][Demention], double epsilon, double h){
    for (int i = 0; i < Demention; i++){
        for (int j = 0; j < Demention; j++){
            Matrix[i][j] = 0.0;
            if (i == j){
                Matrix[i][j] = -(2 * epsilon + h);
            }if (i == j + 1){
                Matrix[i][j] = epsilon;
            }if (i == j - 1){
                Matrix[i][j] = epsilon + h;
            }
        }
    }
}

void Print_Matrix(double Matrix[][Demention], double B[]){
    for (int i = 0; i < Demention; i++){
        for (int j = 0; j < Demention; j++){
            printf("%lf\t", Matrix[i][j]);
        }
        printf("%lf\n", B[i]);
    }
    printf("\n");
}

void Swap_Matrix(double Matrix[][Demention], double B[], int row_A, int row_B){
    for (int col = 0; col < Demention; col++){
        double temp = Matrix[row_A][col];
        Matrix[row_A][col] = Matrix[row_B][col];
        Matrix[row_B][col] = temp;
    }
    double temp = B[row_A];
    B[row_A] = B[row_B];
    B[row_B] = temp;
}

void Gauss_elimination(double S_Matrix[][Demention], double solution[], double B[]){
    int index_row, index_col, index_temp;

    for (index_col = 0; index_col < Demention; index_col++){
        double Max_col = fabs(S_Matrix[index_col][index_col]);
        index_temp = index_col;
        
        for (index_row = index_col; index_row < Demention; index_row++){
            if (fabs(S_Matrix[index_row][index_col]) > Max_col){
                Max_col = fabs(S_Matrix[index_row][index_col]);
                index_temp = index_row;
            }
        }

        if (index_temp != index_col) Swap_Matrix(S_Matrix, B, index_col, index_temp);
        
        for (index_row = index_col + 1; index_row < Demention; index_row++){
            double temp = - (S_Matrix[index_row][index_col]);
            for (int col = index_col; col < Demention; col++){
                double scale = (S_Matrix[index_col][col] / S_Matrix[index_col][index_col]);
                S_Matrix[index_row][col] += scale * temp;
            }
            B[index_row] += B[index_col]/S_Matrix[index_col][index_col] * temp;
        }
    }

    for (index_row = Demention - 1; index_row >= 0; index_row--){
        if (index_row == Demention - 1){
            solution[index_row] = B[index_row] / S_Matrix[index_row][index_row];
        }else{
            double solution_temp = B[index_row];
            for (int col = index_row + 1; col < Demention; col++){
                solution_temp -= S_Matrix[index_row][col] * solution[col];
            }
            solution[index_row] = solution_temp / S_Matrix[index_row][index_row];
        }
    }
}

void Gauss_Seidel(double S_Matrix[][Demention], double B[], double initial_X[], double Max_error, double Result_X[]){
    double Difference_X[Demention], temp;

    for (int index = 0; index < Demention; index++){
        Result_X[index] = initial_X[index];
    }

    do{
        for (int index_row = 0; index_row < Demention; index_row++){
            temp = B[index_row];
            for (int index_col = 0; index_col < Demention; index_col++){
                if (index_row != index_col){
                    temp -= S_Matrix[index_row][index_col] * Result_X[index_col];
                }
            }
            Result_X[index_row] = temp / S_Matrix[index_row][index_row];
        }

        Vector_Minus(Result_X, initial_X, Difference_X);

        for (int index = 0; index < Demention; index++){
            initial_X[index] = Result_X[index];
        }
    } while (Infinite_Norm(Difference_X) > Max_error);
}

void Vector_Minus(double V[], double U[], double Result[]){
    for (int i = 0; i < Demention; i++){
        Result[i] = V[i] - U[i];
    }
}

double Infinite_Norm(double V[]){
    double Max = fabs(V[0]);

    for (int i = 1; i < Demention; i++){
        Max = (fabs(V[i] > Max)) ? fabs(V[i]) : Max;
    }

    return Max;
}

double ComputeRMSE(double numerical_solution[], double exact_solution[], int num_points){
    double sum_squared_error = 0.0;

    for (int i = 0; i < num_points; ++i) {
        double error = numerical_solution[i] - exact_solution[i];
        sum_squared_error += error * error;
    }

    double rmse = sqrt(sum_squared_error / num_points);

    return rmse;
}
