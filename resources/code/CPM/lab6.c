# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <complex.h>

# define PI 3.14159265358979

double functions(double t, int index);
void gen_values(int index, int n, const char* filename);
void fft(complex double *f, complex double *g, int n);
void implenment_fft(const char *input_filename, const char *output_filename, int n);
void ifft(complex double *f, complex double *g, int n);
void implenment_ifft(const char *input_filename, const char *output_filename, int n);

int main(){
    gen_values(1, 16, "f1_16_distribute.txt");
    gen_values(1, 128, "f1_128_distribute.txt");
    gen_values(2, 128, "f2_128_distribute.txt");

    implenment_fft("f1_16_distribute.txt", "g1_16.txt", 16);
    implenment_fft("f1_128_distribute.txt", "g1_128.txt", 128);
    implenment_fft("f2_128_distribute.txt", "g2_128.txt", 128);

    implenment_ifft("g1_16.txt", "f1_16_result.txt", 16);
    implenment_ifft("g1_128.txt", "f1_128_result.txt", 128);
    implenment_ifft("g2_128.txt", "f2_128_result.txt", 128);

    FILE *fp = fopen("g2_128.txt", "r");
    complex double *quarter = (complex double *)malloc((128 / 4) * sizeof(complex double));
    for (int i = 0; i < 128 / 4; i++){
        double real, imag;
        fscanf(fp, "%lf %lf\n", &real, &imag);
        quarter[i] = real + I * imag;
    }
    fclose(fp);

    complex double *quarter_result = (complex double *)malloc((128 / 4) * sizeof(complex double));
    ifft(quarter, quarter_result, 128 / 4);

    fp = fopen("f2_128_quarter.txt", "w");
    for(int i = 0; i < 128 / 4; i++){
        fprintf(fp, "%lf %lf\n", creal(quarter_result[i]), cimag(quarter_result[i]));
    }
    fclose(fp);

    free(quarter);
    free(quarter_result);
    return 0;
}

double functions(double t, int index){
    double result;
    switch(index){
    case 1:
        result = 0.7 * sin(2 * PI * 2 * t) + sin(2 * PI * 5 * t);
        break;
    case 2:
        result = 0.7 * sin(2 * PI * 2 * t) + sin(2 * PI * 5 * t) + 0.3 * ((double) rand() / RAND_MAX);
        break;
    default:
        printf("Wrong input!!!");
        break;
    }

    return result;
}

void gen_values(int index, int n, const char* filename){
    double *results = (double *)malloc(n * sizeof(double));
    double *t_values = (double *)malloc(n * sizeof(double));
    
    FILE *fp = fopen(filename, "w");
    for(int i = 0; i < n; i++){
        t_values[i] = (double) i / n;
        results[i] = functions(t_values[i], index);
        fprintf(fp, "%lf %lf\n", t_values[i], results[i]);
    }
    fclose(fp);

    free(t_values);
    free(results);
}

void fft(complex double *f, complex double *g, int n){
    if(n == 1){
        g[0] = f[0];
        return;
    }

    complex double *f_even = (complex double*)malloc((n / 2) * sizeof(complex double));
    complex double *f_odd = (complex double *)malloc((n / 2) * sizeof(complex double));
    complex double *g_even = (complex double *)malloc((n / 2) * sizeof(complex double));
    complex double *g_odd = (complex double *)malloc((n / 2) * sizeof(complex double));
    for (int i = 0; i < n / 2; i++){
        f_even[i] = f[2 * i];
        f_odd[i] = f[2 * i + 1];
    }

    fft(f_even, g_even, n / 2);
    fft(f_odd, g_odd, n / 2);

    complex double omega_n = cexp(- I * 2 * PI / n);
    complex double omega = 1;

    for(int k = 0; k < n / 2; k++){
        g[k] = (g_even[k] + omega * g_odd[k]) / 2;
        g[k + (n / 2)] = (g_even[k] - omega * g_odd[k]) / 2;
        omega = omega * omega_n;
    }

    free(f_even);
    free(f_odd);
    free(g_even);
    free(g_odd);
}

void implenment_fft(const char *input_filename, const char *output_filename, int n){
    complex double *f = (complex double *)malloc(n * sizeof(complex double));
    complex double *g = (complex double *)malloc(n * sizeof(complex double));

    FILE *fp = fopen(input_filename, "r");
    double t, result;
    for (int i = 0; i < n; i++){
        fscanf(fp, "%lf %lf\n", &t, &result);
        f[i] = result + 0 * I;
    }
    fclose(fp);

    fft(f, g, n);

    fp = fopen(output_filename, "w");
    for(int i = 0; i< n; i++){
        fprintf(fp, "%lf %lf\n", creal(g[i]), cimag(g[i]));
    }
    fclose(fp);

    free(f);
    free(g);
}

void ifft(complex double *f, complex double *g, int n){
    if(n == 1){
        g[0] = f[0];
        return;
    }

    complex double *f_even = (complex double *)malloc((n / 2) * sizeof(complex double));
    complex double *f_odd = (complex double *)malloc((n / 2) * sizeof(complex double));
    complex double *g_even = (complex double *)malloc((n / 2) * sizeof(complex double));
    complex double *g_odd = (complex double *)malloc((n / 2) * sizeof(complex double));

    for (int i = 0; i < n / 2; i++){
        f_even[i] = f[2 * i];
        f_odd[i] = f[2 * i + 1];
    }

    ifft(f_even, g_even, n / 2);
    ifft(f_odd, g_odd, n / 2);

    complex double omega_n = cexp(I * 2 * PI / n);
    complex double omega = 1;

    for(int k = 0; k < n / 2; k++){
        g[k] = (g_even[k] + omega * g_odd[k]);
        g[k + (n / 2)] = (g_even[k] - omega * g_odd[k]);
        omega = omega * omega_n;
    }

    free(f_even);
    free(f_odd);
    free(g_even);
    free(g_odd);
}

void implenment_ifft(const char *input_filename, const char *output_filename, int n){
    complex double *f = (complex double *)malloc(n * sizeof(complex double));
    complex double *g = (complex double *)malloc(n * sizeof(complex double));

    FILE *fp = fopen(input_filename, "r");
    double real, imag;
    for (int i = 0; i < n; i++){
        fscanf(fp, "%lf %lf\n", &real, &imag);
        f[i] = real + imag * I;
    }
    fclose(fp);

    ifft(f, g, n);

    fp = fopen(output_filename, "w");
    for(int i = 0; i< n; i++){
        fprintf(fp, "%lf %lf\n", (double) i / n, creal(g[i]));
    }
    fclose(fp);

    free(f);
    free(g);
}