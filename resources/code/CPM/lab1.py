import numpy as np

def function_A(x_P, y_P, x_Q, y_Q, x_T, y_T):
    return 2 * (x_P * x_Q + y_P * y_Q) * x_T* y_T + 2 * (x_Q**2 + y_Q**2) * y_T**2 - y_T - 1 + x_T

def function_B(x_P, y_P, x_Q, y_Q, x_T, y_T):
    return (x_P**2 + y_P**2) * x_T**2 + 2 * (x_P * x_Q + y_P * y_Q) * x_T * y_T + (x_Q**2 + y_Q**2) * y_T**2 - 1

def d_function_A_dx(x_P, y_P, x_Q, y_Q, x_T, y_T):
    return 2 * (x_P * x_Q + y_P * y_Q) * y_T + 1

def d_function_A_dy(x_P, y_P, x_Q, y_Q, x_T, y_T):
    return 2 * (x_P * x_Q + y_P * y_Q) * x_T + 4 * (x_Q**2 + y_Q**2) * y_T - 1

def d_function_B_dx(x_P, y_P, x_Q, y_Q, x_T, y_T):
    return 2 * (x_P**2 + y_P**2) * x_T + 2 * (x_P * x_Q + y_P * y_Q) * y_T

def d_function_B_dy(x_P, y_P, x_Q, y_Q, x_T, y_T):
    return 2 * (x_Q**2 + y_Q**2) * y_T + 2 * (x_P * x_Q + y_P * y_Q) * x_T

def Newton_iteration(x_P, y_P, x_Q, y_Q, x_T, y_T):
    x = x_T
    y = y_T
    delta_x = 1
    delta_y = 1

    while abs(delta_x) >= 1e-12 and abs(delta_y) >= 1e-12:
        jacobian = np.array([[d_function_A_dx(x_P, y_P, x_Q, y_Q, x, y), d_function_A_dy(x_P, y_P, x_Q, y_Q, x, y)], [d_function_B_dx(x_P, y_P, x_Q, y_Q, x, y), d_function_B_dy(x_P, y_P, x_Q, y_Q, x, y)]])
        jacobian_inv = np.linalg.inv(jacobian)

        delta_x = -function_A(x_P, y_P, x_Q, y_Q, x, y) * jacobian_inv[0, 0] - function_B(x_P, y_P, x_Q, y_Q,x, y) * jacobian_inv[0, 1]
        delta_y = -function_A(x_P, y_P, x_Q, y_Q, x, y) * jacobian_inv[1, 0] - function_B(x_P, y_P, x_Q, y_Q, x, y) * jacobian_inv[1, 1]

        x += delta_x
        y += delta_y

    return [x * x_P + y * x_Q, x * y_P + y * y_Q]

def CalculateR(x_Q, y_Q, x_T, y_T):
    a = -1 / (y_T / x_T)
    b = y_T - a * x_T
    x_R = x_Q - (2 * a * (a * x_Q - y_Q + b))/(a**2 + 1)
    y_R = y_Q - (2 * (-1.0) * (a * x_Q - y_Q + b))/(a**2 + 1)
    return [x_R, y_R]

P = [-2, 0]
Q = [-1, 1]
T = Newton_iteration(P[0], P[1], Q[0], Q[1], 0.5, 0.5)
R = CalculateR(Q[0], Q[1], T[0], T[1])
print('T: ', T, 'R: ', R)

P = [-10, 0]
Q = [-2, 1]
T = Newton_iteration(P[0], P[1], Q[0], Q[1], 0.5, 0.5)
R = CalculateR(Q[0], Q[1], T[0], T[1])
print('T: ', T, 'R: ', R)

P = [-1.000001, 0]
Q = [-2, 2]
T = Newton_iteration(P[0], P[1], Q[0], Q[1], 0.5, 0.5)
R = CalculateR(Q[0], Q[1], T[0], T[1])
print('T: ', T, 'R: ', R)

P = [-2, 0]
Q = [-1, 0.000001]
T = Newton_iteration(P[0], P[1], Q[0], Q[1], 0.5, 0.5)
R = CalculateR(Q[0], Q[1], T[0], T[1])
print('T: ', T, 'R: ', R)

P = [-2.33, 0]
Q = [-3, 1]
T = Newton_iteration(P[0], P[1], Q[0], Q[1], 0.5, 0.5)
R = CalculateR(Q[0], Q[1], T[0], T[1])
print('T: ', T, 'R: ', R)

P = [-3, 0]
Q = [-1, 0.5]
T = Newton_iteration(P[0], P[1], Q[0], Q[1], 0.5, 0.5)
R = CalculateR(Q[0], Q[1], T[0], T[1])
print('T: ', T, 'R: ', R)

P = [-3, 0]
Q = [-3, 10]
T = Newton_iteration(P[0], P[1], Q[0], Q[1], 0.5, 0.5)
R = CalculateR(Q[0], Q[1], T[0], T[1])
print('T: ', T, 'R: ', R)

P = [-3, 0]
Q = [-3, 1]
T = Newton_iteration(P[0], P[1], Q[0], Q[1], 0.5, 0.5)
R = CalculateR(Q[0], Q[1], T[0], T[1])
print('T: ', T, 'R: ', R)

P = [-10, 0]
Q = [-2, 1]
T = Newton_iteration(P[0], P[1], Q[0], Q[1], 0.5, 0.5)
R = CalculateR(Q[0], Q[1], T[0], T[1])
print('T: ', T, 'R: ', R)

P = [-1024, 0]
Q = [-8, 4]
T = Newton_iteration(P[0], P[1], Q[0], Q[1], 0.5, 0.5)
R = CalculateR(Q[0], Q[1], T[0], T[1])
print('T: ', T, 'R: ', R)