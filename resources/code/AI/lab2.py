import sys
from utils import UtilGobang
import numpy as np
import tkinter as tk
import copy
import random
import math
from tqdm import tqdm
import pickle
from scipy.signal import convolve2d
from typing import *


########################################################################################
# This is the second assignment.
# In the class UtilGobang, we have already helped to implement other functions that may be
# used later, and if necessary, you can directly use some or all of these functions.
# Gobang inherits the UtilGobang Class, and all you need to do is to complete the remaining
# key functions. Please refer to the comments before each question for a better understanding.
# Here are some useful methods from external libraries (which have already been imported in UtilGobang)
# for your reference.
# y = copy.deepcopy(x) | Copies the value of object x and assigns it to y.
# x = random.choice(l) | Samples an element x from a list l with equal probability.
# np.count_nonzero(s) | Returns the number of zeros in the array s.
########################################################################################

########################################################################################
# Some important explanations regarding the variables:
# self.board:   An N x N two-dimensional np.array stored inside the Gobang class (also in UtilGobang).
#               The value of self.board[i][j] should be:
#               0, if the current position [i][j] has not been occupied by any piece.
#               1, if a black chess piece is located.
#               2, if a white chess piece is located.
#
# self.Q:       A dictionary {state: {action: q}} that stores the estimated Q* values.
#               For a given state-action pair (s, a), you can access q(s, a) by reading self.Q[s][a].
#               It is essential to ensure that the state has already been hashed.
#
# color:        1 represents black, 2 represents white.
#
# action:       A triplet (1, x, y) for black pieces.
#               For example, action = (1, 0, 0) indicates that the next step is to set self.board[0][0] = 1.
#
# self.action_space:
#               A list [(int, int)] that stores the available positions on the current chessboard where
#               players can place their pieces.
#
# noise:        A triplet (2, x, y) used to indicate that the opponent (white) will place a piece
#               at (x, y), resulting in self.board[x][y] = 2.
#
# Additionally, the usage of several key functions:
#
# max_black, max_white = self.count_max_connections(state):
#               This function is used to determine the maximum number of consecutive black and
#               white chess pieces in a specific state, returning max_black and max_white.
#
# state = self.array_to_hashable(state_):
#               Converts the np.array type state into a hashable value, enabling it to be
#               stored as a key in a dictionary.
########################################################################################


class Gobang(UtilGobang):
    ########################################################################################
    # Problem 1: Modelling MDP.
    ########################################################################################

    # You do not have to modify the value of self.board during this step. #

    def get_next_state(self, action: Tuple[int, int, int], noise: Tuple[int, int, int]) -> np.array:
        """
        The function get_next_state takes two parameters, “action” and “noise”, and calculates
        the next state based on the current state stored in self.board.

        NOTE: The presence of “Noise=None” is a specific requirement for other functions in UtilGobang,
        indicating that when this condition is met, the placement of white chess pieces is not taken into account.

        There is no need to alter the value of self.board during this process.
        """

        # BEGIN_YOUR_CODE (our solution is 3 line of code, but don't worry if you deviate from this)
        black, x_black, y_black = action
        next_state = copy.deepcopy(self.board)
        next_state[x_black][y_black] = black
        # END_YOUR_CODE

        if noise is not None:
            white, x_white, y_white = noise
            next_state[x_white][y_white] = white
        return next_state

    def sample_noise(self) -> Union[Tuple[int, int, int], None]:
        """
        The function sample_noise returns a noise tuple (2, x, y), where the position (x, y) is randomly sampled from
        self.action_space. Additionally, it is necessary to remove the selected position (x, y) from self.action_space,
        as it is no longer available for placing pieces.
        """
        if self.action_space:
            # BEGIN_YOUR_CODE (our solution is 2 line of code, but don't worry if you deviate from this)
            x, y = random.choice(self.action_space)
            self.action_space.remove((x, y))
            # END_YOUR_CODE
            return 2, x, y
        else:
            return None

    def get_connection_and_reward(self, action: Tuple[int, int, int],
                                  noise: Tuple[int, int, int]) -> Tuple[int, int, int, int, float]:
        """
        The function get_connection_and_reward takes two parameters, “action” and “noise”, and calculates the reward
        based on the predefined criteria outlined in our experimental documentation. You are encouraged to use the
        existing function self.count_max_connections for ease of computation.

        NOTE:
            "black_1" and "white_1" denote the maximum number of connections in the current state represented by
            self.board. "black_2" and "white_2" denote the maximum number of connections in the subsequent state
            (next_state).
        """

        # BEGIN_YOUR_CODE (our solution is 4 line of code, but don't worry if you deviate from this)
        black_1, white_1 = self.count_max_connections(self.board)
        next_state = self.get_next_state(action, noise)
        black_2, white_2 = self.count_max_connections(next_state)
        reward = (black_2**2 - white_2**2) - (black_1**2 - white_1**2)
        # END_YOUR_CODE

        return black_1, white_1, black_2, white_2, reward

    ########################################################################################
    # Problem 2: Implement Q learning algorithms.
    ########################################################################################

    def sample_action_and_noise(self, eps: float) -> Tuple[Tuple[int, int, int], Tuple[int, int, int]]:
        """
        The function returns the action and noise based on the current state represented by self.board.

        During the learning process, the action is chosen following the epsilon-greedy algorithm, which entails the
        following:

        The action is randomly selected with a probability of epsilon (eps), and with a probability of 1-eps, the
        action corresponding to the current maximum estimated q-values q[s][a] is chosen. If the state-action pair
        (s, a) has not been recorded in self.Q, a random action should also be returned. The action must be selected
        from self.action_space to prevent overlapping chess pieces.

        *** IMPORTANT: Remember to remove the selected position (x, y) from self.action_space once the action (1, x, y)
            is chosen. ***

        It is worth noting that the action space in our scenario may change over time. This dynamic action space
        approach is based on the assumption that the optimal policy learned in a dynamic action space with limited
        rewards and constraints on piece placement should be equivalent to the one learned in a fixed action space with
        penalties (e.g., negative infinity rewards) for overlapping pieces. You are encouraged to contemplate whether
        this equivalence truly holds.

        Additionally, we implement dynamic epsilons to enhance the learning outcome, which are pre-computed before
        being passed to the function sample_action_and_noise.
        """

        # BEGIN_YOUR_CODE (our solution is 8 line of code, but don't worry if you deviate from this)
        state = self.array_to_hashable(self.board)
        q_values = self.Q.get(state, {})
        if random.random() < eps or not q_values:
            act = random.choice(self.action_space)
            action = (1, act[0], act[1])
        else:
            action = max(q_values, key=q_values.get)
        self.action_space.remove((action[1], action[2]))
        # END_YOUR_CODE
        return action, self.sample_noise()

    def q_learning_update(self, s0_: np.array, action: Tuple[int, int, int], s1_: np.array, reward: float,
                          alpha_0: float = 1):
        """
        The function q_learning_update takes 4 parameters: s0_, action, s1_, and reward. It updates the estimations for
        Q* values stored in self.Q.

        The function does not return any values.

        Alpha represents the dynamic learning rate that ensures convergence in uncertain environments.

        NOTE: Prior to updating, you need to convert the raw states into hashable states to enable them to be stored as
        keys in the dictionary.
        """

        s0, s1 = self.array_to_hashable(s0_), self.array_to_hashable(s1_)
        self.s_a_visited[(s0, action)] = 1 if (s0, action) not in self.s_a_visited else \
            self.s_a_visited[(s0, action)] + 1
        alpha = alpha_0 / self.s_a_visited[(s0, action)]

        # BEGIN_YOUR_CODE (our solution is 18 line of code, but don't worry if you deviate from this)
        q_value = self.Q.get(s0, {}).get(action, 0.0)
        new_q_value = q_value + alpha * (reward + self.gamma * max(self.Q.get(s1, {}).values(), default=0.0) - q_value)
        if s0 not in self.Q:
            self.Q[s0] = {}
        self.Q[s0][action] = new_q_value
       # END_YOUR_CODE