# include <stdio.h>
# include <math.h>

# define D 2

typedef struct Vector{
    double cor[D];
    double mol;
}vector;

typedef struct Circle{
    double center[D];
    double radius;
}circle;

circle initCircle(double x, double y, double r);
vector initVector(double cor_x, double cor_y);
double CalculateMol(vector V);
double InnerProduct(vector A, vector B);
double function_A(vector P, vector Q, vector T);
double function_B(vector P, vector Q, vector T);
vector d_function_A(vector P, vector Q, vector T);
vector d_function_B(vector P, vector Q, vector T);
vector Newton_iteration(vector T_initial, vector P, vector Q, double epsilon);
void matrixInverse(double A[][D], double inverse[][D]);
void MatrixInv(double Matrix[][D], double result[][D]);
vector CalculateR(vector Q, vector T);

int main(){
    circle C = initCircle(0, 0, 1);
    vector T_initial = initVector(-0.5, 0.5);

    vector P = initVector(-2, 0);
    vector Q = initVector(-1, 1);
    vector T = Newton_iteration(T_initial, P, Q, 1e-12);
    vector R = CalculateR(Q, T);
    printf("T: (%lf, %lf)\tR: (%lf, %lf)\n", T.cor[0], T.cor[1], R.cor[0], R.cor[1]);

    P = initVector(-10, 0);
    Q = initVector(-2, 1);
    T = Newton_iteration(T_initial, P, Q, 1e-12);
    R = CalculateR(Q, T);
    printf("T: (%lf, %lf)\tR: (%lf, %lf)\n", T.cor[0], T.cor[1], R.cor[0], R.cor[1]);

    P = initVector(-1.000001, 0);
    Q = initVector(-2, 2);
    T = Newton_iteration(T_initial, P, Q, 1e-12);
    R = CalculateR(Q, T);
    printf("T: (%lf, %lf)\tR: (%lf, %lf)\n", T.cor[0], T.cor[1], R.cor[0], R.cor[1]);

    P = initVector(-2, 0);
    Q = initVector(-1, 0.000001);
    T = Newton_iteration(T_initial, P, Q, 1e-12);
    R = CalculateR(Q, T);
    printf("T: (%lf, %lf)\tR: (%lf, %lf)\n", T.cor[0], T.cor[1], R.cor[0], R.cor[1]);
    
    P = initVector(-2.33, 0);
    Q = initVector(-3, 1);
    T = Newton_iteration(T_initial, P, Q, 1e-12);
    R = CalculateR(Q, T);
    printf("T: (%lf, %lf)\tR: (%lf, %lf)\n", T.cor[0], T.cor[1], R.cor[0], R.cor[1]);

    P = initVector(-3, 0);
    Q = initVector(-1, 0.5);
    T = Newton_iteration(T_initial, P, Q, 1e-12);
    R = CalculateR(Q, T);
    printf("T: (%lf, %lf)\tR: (%lf, %lf)\n", T.cor[0], T.cor[1], R.cor[0], R.cor[1]);

    P = initVector(-3, 0);
    Q = initVector(-2, 10);
    T = Newton_iteration(T_initial, P, Q, 1e-12);
    R = CalculateR(Q, T);
    printf("T: (%lf, %lf)\tR: (%lf, %lf)\n", T.cor[0], T.cor[1], R.cor[0], R.cor[1]);

    P = initVector(-3, 0);
    Q = initVector(-3, 1);
    T = Newton_iteration(T_initial, P, Q, 1e-12);
    R = CalculateR(Q, T);
    printf("T: (%lf, %lf)\tR: (%lf, %lf)\n", T.cor[0], T.cor[1], R.cor[0], R.cor[1]);

    P = initVector(-10, 0);
    Q = initVector(-2, 1);
    T = Newton_iteration(T_initial, P, Q, 1e-12);
    R = CalculateR(Q, T);
    printf("T: (%lf, %lf)\tR: (%lf, %lf)\n", T.cor[0], T.cor[1], R.cor[0], R.cor[1]);

    P = initVector(-1024, 0);
    Q = initVector(-8, 4);
    T = Newton_iteration(T_initial, P, Q, 1e-12);
    R = CalculateR(Q, T);
    printf("T: (%lf, %lf)\tR: (%lf, %lf)\n", T.cor[0], T.cor[1], R.cor[0], R.cor[1]);
}

double function_A(vector P, vector Q, vector T){
    return 2 * InnerProduct(P, Q) * T.cor[0] * T.cor[1] + 2 * pow(Q.mol, 2) * pow(T.cor[1], 2) - T.cor[1] - 1 + T.cor[0];
}

double function_B(vector P, vector Q, vector T){
    return pow(P.mol, 2) * pow(T.cor[0], 2) + 2 * InnerProduct(Q, P) * T.cor[0] * T.cor[1] + pow(Q.mol, 2) * pow(T.cor[1], 2) - 1;
}

vector d_function_A(vector P, vector Q, vector T){
    vector df;
    df.cor[0] = 2 * InnerProduct(Q, P) * T.cor[1] + 1;
    df.cor[1] = 2 * InnerProduct(Q, P) * 4 * pow(Q.mol, 2) * T.cor[1] - 1;
    return df;
}

vector d_function_B(vector P, vector Q, vector T){
    vector df;
    df.cor[0] = 2 * pow(P.mol, 2) * T.cor[0] + 2 * InnerProduct(P, Q) * T.cor[1];
    df.cor[1] = 2 * InnerProduct(Q, P) * T.cor[0] + 2 * pow(Q.mol, 2) * T.cor[1];
    return df;
}

double CalculateMol(vector V){
    return sqrt(pow(V.cor[0], 2) + pow(V.cor[1], 2));
}

circle initCircle(double x, double y, double r){
    circle C;
    C.center[0] = x;
    C.center[1] = y;
    C.radius = r;
    return C;
}

vector initVector(double cor_x, double cor_y){
    vector V;
    V.cor[0] = cor_x;
    V.cor[1] = cor_y;
    V.mol = CalculateMol(V);
    return V;
}

double InnerProduct(vector A, vector B){
    return A.cor[0] * B.cor[0] + A.cor[1] * B.cor[1];
}

vector Newton_iteration(vector T_initial, vector P, vector Q, double epsilon){
    vector T = T_initial;
    double delta_x;
    double delta_y;

    do{
        double jacobian[D][D];
        double jacobian_inv[D][D];
        jacobian[0][0] = d_function_A(P, Q, T).cor[0];
        jacobian[0][1] = d_function_A(P, Q, T).cor[1];
        jacobian[1][0] = d_function_B(P, Q, T).cor[0];
        jacobian[1][1] = d_function_B(P, Q, T).cor[1];

        matrixInverse(jacobian, jacobian_inv);
        delta_x = jacobian_inv[0][0] * (-function_A(P, Q, T)) + jacobian_inv[0][1] * (-function_B(P, Q, T));
        delta_y = jacobian_inv[1][0] * (-function_A(P, Q, T)) + jacobian_inv[1][1] * (-function_B(P, Q, T));

        T.cor[0] += delta_x;
        T.cor[1] += delta_y;
    }while (abs(delta_x) >= epsilon || abs(delta_y) >= epsilon);

    T.cor[0] = T.cor[0] * P.cor[0] + T.cor[1] * Q.cor[0];
    T.cor[1] = T.cor[0] * P.cor[1] + T.cor[1] * Q.cor[1];
    
    return T;
}

void matrixLUDecomposition(double A[][D], double L[][D], double U[][D]){
    // Initialize L as an identity matrix and U as a copy of A
    for (int i = 0; i < D; i++) {
        for (int j = 0; j < D; j++) {
            if (i == j)
                L[i][j] = 1.0;
            else
                L[i][j] = 0.0;
            U[i][j] = A[i][j];
        }
    }

    // Perform LU decomposition
    for (int k = 0; k < D; k++) {
        for (int i = k + 1; i < D; i++) {
            double factor = U[i][k] / U[k][k];
            L[i][k] = factor;
            for (int j = k; j < D; j++) {
                U[i][j] -= factor * U[k][j];
            }
        }
    }
}

void matrixInverse(double A[][D], double inverse[][D]){
    double L[D][D], U[D][D];

    // Perform LU decomposition
    matrixLUDecomposition(A, L, U);

    // Solve Ly = b for y (Forward substitution)
    double y[D][D];
    for (int i = 0; i < D; i++) {
        for (int j = 0; j < D; j++) {
            double sum = 0.0;
            for (int k = 0; k < i; k++) {
                sum += L[i][k] * y[k][j];
            }
            y[i][j] = (i == j) ? 1.0 - sum : -sum;
        }
    }

    // Solve Ux = y for x (Backward substitution)
    double x[D][D];
    for (int i = D - 1; i >= 0; i--) {
        for (int j = 0; j < D; j++) {
            double sum = 0.0;
            for (int k = i + 1; k < D; k++) {
                sum += U[i][k] * x[k][j];
            }
            x[i][j] = (y[i][j] - sum) / U[i][i];
        }
    }

    // Copy x to inverse matrix
    for (int i = 0; i < D; i++) {
        for (int j = 0; j < D; j++) {
            inverse[i][j] = x[i][j];
        }
    }
}

void MatrixInv(double Matrix[][D], double result[][D]){
    result[0][0] = 1;
    result[0][1] = 0;
    result[1][0] = 0;
    result[1][1] = 1;

    double temp = Matrix[1][0];
    Matrix[1][0] += (Matrix[0][0]/Matrix[0][0]) * (-temp);
    Matrix[1][1] += (Matrix[0][1]/Matrix[0][0]) * (-temp);
    result[1][0] += (result[0][0]/Matrix[0][0]) * (-temp);
    result[1][1] += (result[0][1]/Matrix[0][0]) * (-temp);

    temp = Matrix[0][1];
    Matrix[0][0] += (Matrix[1][0]/Matrix[1][1]) * (-temp);
    Matrix[0][1] += (Matrix[1][1]/Matrix[1][1]) * (-temp);
    result[0][0] += (result[1][0]/Matrix[1][1]) * (-temp);
    result[0][1] += (result[1][1]/Matrix[1][1]) * (-temp);

    result[0][0] /= Matrix[0][0];
    result[0][1] /= Matrix[0][0];
    result[1][0] /= Matrix[1][1];
    result[1][1] /= Matrix[1][1];
}

vector CalculateR(vector Q, vector T){
    double a = -1.0 / (T.cor[1]/T.cor[0]);
    double b = T.cor[1] - a * T.cor[0];
    double x = Q.cor[0] - (2 * a * (a * Q.cor[0] - Q.cor[1] + b))/(pow(a, 2) + 1);
    double y = Q.cor[1] - (2 * (-1.0) * (a * Q.cor[0] - Q.cor[1] + b))/(pow(a, 2) + 1);
    vector result;
    result.cor[0] = x;
    result.cor[1] = y;
    result.mol = CalculateMol(result);
    return result;
}