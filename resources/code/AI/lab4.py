from utils import *
import numpy as np
import torch
import torch.nn as nn
from typing import *
import sys
import argparse

parser = argparse.ArgumentParser(description='args')
parser.add_argument('--num_episodes', type=int, help='number of episodes')
parser.add_argument('--checkpoint', type=int, help='the interval of saving models')
args = parser.parse_args()
num_episodes = args.num_episodes
checkpoint = args.checkpoint


class Residual(nn.Module):
    """
    残差块
    """
    def __init__(self, in_channels, out_channels, use_one_d:bool, stride) -> None:
        super().__init__()
        self.convd_layer_a = nn.Conv2d(in_channels=in_channels, out_channels=out_channels, kernel_size=3, padding=1, stride=stride)
        self.convd_layer_b = nn.Conv2d(in_channels=out_channels, out_channels=out_channels, kernel_size=3, padding=1)
        self.convd_layer_c = nn.Conv2d(in_channels=in_channels, out_channels=out_channels, kernel_size=1, stride=stride) if use_one_d else None
        self.batchnorm_layer_a = nn.BatchNorm2d(out_channels)
        self.batchnorm_layer_b = nn.BatchNorm2d(out_channels)

    def forward(self, input_tensor):
        out_tensor = nn.functional.relu(self.batchnorm_layer_a(self.convd_layer_a(input_tensor)))
        out_tensor = self.batchnorm_layer_b(self.convd_layer_b(out_tensor))
        input_tensor = self.convd_layer_c(input_tensor) if self.convd_layer_c else input_tensor
        out_tensor += input_tensor
        return nn.functional.relu(out_tensor)

class Actor(nn.Module):
    """
    The actor is responsible for generating dependable policies to maximize the cumulative reward as much as possible.
    It takes a batch of arrays shaped either (B, 1, N, N) or (N, N) as input, and outputs a tensor shaped (B, N ** 2)
    as the generated policy.
    """

    def __init__(self, board_size: int, lr=1e-4):
        super().__init__()
        self.board_size = board_size
        """
        # Define your NN structures here. Torch modules have to be registered during the initialization process.
        # For example, you can define CNN structures as follows:

        # self.conv_blocks = nn.Sequential(
        #     nn.Conv2d(in_channels=1, out_channels=channels, kernel_size=kernel_size, padding=padding),
        #     nn.MaxPool2d(kernel_size=kernel_size, padding=padding, stride=stride),
        #     nn.ReLU(),
        # )

        # Here, channels, kernel_size, padding, and stride are what we would call "Hyperparameters" in deep learning.

        # After convolution, you can flatten (nn.Flatten()) the hidden 2d-representation to obtain the corresponding
        # 1d-representation. Then, fully connected layers can be used to obtain a representation of n**2 dimensions,
        # with each digit indicating the "raw number of policy" (which has to be further constrained and modified
        # in the next step).

        # self.linear_blocks = nn.Sequential(
        #     nn.Linear(in_features=features, out_features=board_size ** 2),
        # )

        # After obtaining a representation of n**2 dimensions, you STILL NEED TO PERFORM ADDITIONAL PROCESSING,
        # including:
        # i) ensuring that all digits corresponding to illegal actions are set to 0 (!!!!!THE MOST IMPORTANT!!!!!);
        # ii) ensuring that the remaining digits satisfy the normalization condition (i.e., the sum of them is equal
        #     to 1).
        # In-place operations are strongly discouraged because they can lead to gradient calculation failures.
        # As an intelligent alternative, consider approaches that can avoid in-place modifications to achieve the goal.

        # You are also encouraged to explore other powerful models and experiment with different techniques,
        # such as using attention modules, different activation functions, or simply adjusting hyperparameter settings.
        """

        # BEGIN YOUR CODE
        self.residual_net = nn.Sequential(
            nn.Conv2d(in_channels=1, out_channels=64, kernel_size=7, padding=3, stride=1),
            nn.BatchNorm2d(64),
            nn.MaxPool2d(kernel_size=3, padding=1, stride=1),
            Residual(in_channels=64, out_channels=64, use_one_d=False, stride=1),
            Residual(in_channels=64, out_channels=64, use_one_d=False, stride=1),
            Residual(in_channels=64, out_channels=128, use_one_d=True, stride=2),
            Residual(in_channels=128, out_channels=128, use_one_d=False, stride=1),
            Residual(in_channels=128, out_channels=256, use_one_d=True, stride=2),
            Residual(in_channels=256, out_channels=256, use_one_d=False, stride=1),
            nn.AdaptiveAvgPool2d((1,1)),
            nn.Flatten(),
            nn.Linear(in_features=256, out_features=self.board_size**2)
        )
        # END YOUR CODE

        # Define your optimizer here, which is responsible for calculating the gradients and performing optimizations.
        # The learning rate (lr) is another hyperparameter that needs to be determined in advance.
        self.optimizer = torch.optim.Adam(params=self.parameters(), lr=lr)

    def forward(self, x: np.ndarray):
        if len(x.shape) == 2:
            output = torch.tensor(x).to(device).to(torch.float32).unsqueeze(0).unsqueeze(0)
        else:
            output = torch.tensor(x).to(device).to(torch.float32)

        # Further process and transform the data here. Ensure that the output is shaped (B, n ** 2).
        # We have already ensured that the shape of the raw input is unified to be (B, 1, N, N),
        # where B >= 1 represents the number of data in this batch, and N = n is exactly the size of the board.

        # You can continue processing the data here using the modules that were previously registered during the
        # initialization process. For example:

        # output = self.conv_blocks(output)
        # output = nn.Flatten()(output)
        # output = self.linear_blocks(output)

        # And the reminder AGAIN:

        # ****************************************
        # After obtaining a representation of n**2 dimensions, you STILL NEED TO PERFORM ADDITIONAL DATA PROCESSING,
        # including:
        # i) ensuring that all digits corresponding to illegal actions are set to 0 (!!!!!THE MOST IMPORTANT!!!!!);
        # ii) ensuring that the remaining digits satisfy the normalization condition (i.e., the sum of them is equal
        #     to 1).
        # In-place operations are strongly discouraged because they can lead to gradient calculation failures.
        # ****************************************

        # BEGIN YOUR CODE
        mask = (nn.Flatten()(output) == 0)

        output = self.residual_net(output)
        output = torch.softmax(output, dim=(-1)) + 1e-10
        
        output = output * mask
        output = output / torch.sum(output, dim=(-1), keepdim=True)
        # END YOUR CODE
        return output


class Critic(nn.Module):
    """
    The critic is responsible for generating dependable Q-values to fit the solution of Bellman Equations. It takes
    a batch of arrays (shaped either (B, 1, N, N) or (N, N)) and a batch of actions (shaped (B, 2)) as input, and
    outputs a tensor shaped (B, ) as the Q-values on the specified (s, a) pairs.

    For example, actions can be:
    [[0, 1],
     [2, 3],
     [5, 6]]
    which means that there are three actions leading the model to place the pieces on the coordinates (0, 1), (2, 3),
    and (5, 6), respectively. These actions correspond one-to-one with indices 0 * 12 + 1 = 1, 2 * 12 + 3 = 27,
    and 5 * 12 + 6 = 66, assuming n to be 12. You can easily transform a single action to the corresponding digit by
    using _position_to_index, or using _index_to_position vice versa.

    The main idea is that we first obtain a tensor shaped (B, N ** 2) as the Q-values for all possible actions given
    the unified state tensor shaped (B, 1, N, N), and then extract the Q-values corresponding to each action (i, j)
    from the entire Q-value tensor. (_position_to_index should be fully utilized to get the corresponding action indices).
    Finally, it returns a tensor of shape (B,) containing these Q-values.
    """

    def __init__(self, board_size: int, lr=1e-4):
        super().__init__()
        self.board_size = board_size
        # Define your NN structures here as the same. Torch modules have to be registered during the initialization
        # process.

        # BEGIN YOUR CODE
        self.residual_layer = nn.Sequential(
            nn.Conv2d(in_channels=1, out_channels=64, kernel_size=7, padding=3, stride=1),
            nn.BatchNorm2d(64),
            nn.MaxPool2d(kernel_size=3, padding=1, stride=1),
            Residual(in_channels=64, out_channels=64, use_one_d=False, stride=1),
            Residual(in_channels=64, out_channels=64, use_one_d=False, stride=1),
            Residual(in_channels=64, out_channels=128, use_one_d=True, stride=2),
            Residual(in_channels=128, out_channels=128, use_one_d=False, stride=1),
            Residual(in_channels=128, out_channels=256, use_one_d=True, stride=1),
            Residual(in_channels=256, out_channels=256, use_one_d=False, stride=1),
            nn.AdaptiveAvgPool2d((1,1)),
            nn.Flatten(),
            nn.Linear(in_features=256, out_features=self.board_size**2),
        )

        self.dense_layer_a = nn.Sequential(
            nn.Linear(in_features=1, out_features=8),
            nn.ReLU(),
            nn.Linear(in_features=8, out_features=16),
            nn.ReLU(),
            nn.Linear(in_features=16, out_features=32)
        )

        self.dense_layer_b = nn.Sequential(
            nn.Linear(in_features=self.board_size**2 + 32, out_features=256),
            nn.ReLU(),
            nn.Linear(in_features=256, out_features=128),
            nn.ReLU(),
            nn.Linear(in_features=128, out_features=64),
            nn.ReLU(),
            nn.Linear(in_features=64, out_features=1)
        )
        # END YOUR CODE

        # Define your optimizer here, which is responsible for calculating the gradients and performing optimizations.
        # The learning rate (lr) is another hyperparameter that needs to be determined in advance.
        self.optimizer = torch.optim.Adam(params=self.parameters(), lr=lr)

    def forward(self, x: np.ndarray, action: np.ndarray):
        indices = torch.tensor([_position_to_index(self.board_size, x, y) for x, y in action]).to(device)
        if len(x.shape) == 2:
            output = torch.tensor(x).to(device).to(torch.float32).unsqueeze(0).unsqueeze(0)
        else:
            output = torch.tensor(x).to(device).to(torch.float32)

        # BEGIN YOUR CODE
        output = self.residual_layer(output)
        indices = self.dense_layer_a(indices.reshape([indices.shape[0], 1]).float())
        output = torch.cat((output, indices), dim=(-1))
        output = self.dense_layer_b(output).squeeze(-1)
        # END YOUR CODE

        return output


class GobangModel(nn.Module):
    """
    The GobangModel class integrates the Actor and Critic classes for computation and training. Given state tensors "x"
    and action tensors "action", it directly outputs self.actor(x) and self.critic(x, action) as the policy and Q-values
    respectively.
    """

    def __init__(self, board_size: int, bound: int):
        super().__init__()
        self.bound = bound
        self.board_size = board_size

        """
        Register the actor and critic modules here. You do not need to further design the structures at this step.
        Feel free to add extra parameters in the __init__ method of either the Actor class or the Critic class for your 
        convenience, if necessary.
        """

        # BEGIN YOUR CODE
        self.actor = Actor(board_size=board_size)
        self.critic = Critic(board_size=board_size)
        # END YOUR CODE

        self.to(device)

    def forward(self, x, action):
        """
        Return the policy vector π(s) and Q-values Q(s, a) given state "x" and action "action".
        """
        return self.actor(x), self.critic(x, action)

    def optimize(self, policy, qs, actions, rewards, next_qs, gamma, eps=1e-6):
        """
        This function calculates the loss for both the actor and critic.
        Using the obtained loss, we can apply optimization algorithms through actor.optimizer and critic.optimizer
        to either maximize the actor's actual objective or minimize the critic's loss.

        There are 3 bugs in the function "optimize" that prevent the model from executing optimizations correctly.
        Identify and debug all errors.
        """

        targets = rewards + gamma * next_qs
        critic_loss = nn.MSELoss()(targets, qs)
        indices = torch.tensor([_position_to_index(self.board_size, x, y) for x, y in actions]).to(device)
        aimed_policy = policy[torch.arange(len(indices)), indices]
        actor_loss = -torch.mean(torch.log(aimed_policy + eps) * qs.clone().detach())

        self.actor.optimizer.zero_grad()
        actor_loss.backward()
        self.actor.optimizer.step()

        self.critic.optimizer.zero_grad()
        critic_loss.backward()
        self.critic.optimizer.step()
        return actor_loss, critic_loss


if __name__ == "__main__":
    agent = GobangModel(board_size=12, bound=5).to(device)
    train_model(agent, num_episodes=num_episodes, checkpoint=checkpoint)
