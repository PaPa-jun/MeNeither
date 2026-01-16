#include <stdio.h>
#include <math.h>

#define MAX_ITER 8
#define TOL 1e-6

// 加速度函数
double a_x(double t) {
    return sin(t) / (sqrt(t) + 1);
}

double a_y(double t) {
    return log(t + 1) / (t + 1);
}

// 插值函数
double interpolate(double x, double *xp, double *yp, int n) {
    if (x <= xp[0]) return yp[0];
    if (x >= xp[n-1]) return yp[n-1];
    for (int i = 0; i < n - 1; ++i) {
        if (x < xp[i + 1]) {
            double t = (x - xp[i]) / (xp[i + 1] - xp[i]);
            return yp[i] * (1 - t) + yp[i + 1] * t;
        }
    }
    return yp[n-1];
}

// Romberg积分实现
double romberg(double (*f)(double, void*), double a, double b, double tol, int max_iter, void* data) {
    double R[MAX_ITER][MAX_ITER] = {0};
    double h = b - a;
    R[0][0] = 0.5 * h * (f(a, data) + f(b, data));

    for (int i = 1; i < max_iter; ++i) {
        h /= 2;
        double sum_f = 0.0;
        for (int k = 1; k <= (1 << (i - 1)); ++k) {
            sum_f += f(a + (2 * k - 1) * h, data);
        }
        R[i][0] = 0.5 * R[i - 1][0] + sum_f * h;

        for (int j = 1; j <= i; ++j) {
            R[i][j] = R[i][j - 1] + (R[i][j - 1] - R[i - 1][j - 1]) / (pow(4, j) - 1);
        }

        if (fabs(R[i][i] - R[i - 1][i - 1]) < tol) {
            return R[i][i];
        }
    }
    return R[max_iter - 1][max_iter - 1];
}

typedef struct {
    double *times;
    double *values;
    int n;
} InterpolationData;

double interpolate_wrapper(double tau, void* data) {
    InterpolationData *interp_data = (InterpolationData*)data;
    return interpolate(tau, interp_data->times, interp_data->values, interp_data->n);
}

double ax_wrapper(double t, void* data) {
    return a_x(t);
}

double ay_wrapper(double t, void* data) {
    return a_y(t);
}

// 计算速度和位移
int main() {
    int n = 100;
    double times[n];
    for (int i = 0; i < n; ++i) {
        times[i] = 0.1 * (i + 1);
    }

    double vx[n], vy[n];
    for (int i = 0; i < n; ++i) {
        vx[i] = romberg(ax_wrapper, 0, times[i], TOL, MAX_ITER, NULL);
        vy[i] = romberg(ay_wrapper, 0, times[i], TOL, MAX_ITER, NULL);
    }

    printf("t\tv_x(t)\tv_y(t)\n");
    for (int i = 0; i < n; ++i) {
        printf("%.1f\t%.6f\t%.6f\n", times[i], vx[i], vy[i]);
    }

    double x[n], y[n];
    InterpolationData vx_data = {times, vx, n};
    InterpolationData vy_data = {times, vy, n};

    for (int i = 0; i < n; ++i) {
        x[i] = romberg(interpolate_wrapper, 0, times[i], TOL, MAX_ITER, &vx_data);
        y[i] = romberg(interpolate_wrapper, 0, times[i], TOL, MAX_ITER, &vy_data);
    }

    printf("t\tx(t)\ty(t)\n");
    for (int i = 0; i < n; ++i) {
        printf("%.1f\t%.6f\t%.6f\n", times[i], x[i], y[i]);
    }

    return 0;
}
