INTRODUCTION
--------------------------------------------------------------------------------
This program simulates one of the classical Artificial Intelligence problems:
the hunter-prey problem. In this problem, the agent called as hunter pursues
and the other agent called as prey, and the prey tries to avoid encounters
with the hunter.
Although the problem itself contains autonomous entities, it is assumed that
the initial status of the agents in the environment are known and the actions
which the agents are going to take during the simulation are given in first-hand.
The simulation environment will be represented by a 2D grid (NxM) where
each cell is either an empty cell, the cell containing an obstacle or
the cell contains an agent.

PROBLEM DEFINITON
--------------------------------------------------------------------------------
• The cells with letters H and P contain the hunter and the prey respectively.
• The coordinate of the top left corner is (0,0).
• O will show the empty cells.
• X will show the cells with an obstacle.
• H will show the cell that contains the hunter.
• P will show the cell that contains the prey.
• N for North
• E for East
• S for South
• W for West

Each agent can move in the given four direction (North, East, South, West)
if the border is not reached and there is no obstacle in that direction.
If an action that is in the direction of border or an obstacle is given,
the agent will stay in the same cell for the next time step as well.
It is assumed that no collision will occur, if the agents move towards each other.

EXAMPLE RUNS
--------------------------------------------------------------------------------
ghci Hw1.hs
∗Hw1> simulate [ [ P , O , X , O ] , [ O , O , H , O ] , [ O , O , X , O ] ] [ ( W , E ) , ( N , S ) ]
>>>Fail
∗Hw1> simulate [ [ P , O , O ] , [ O , O , O ] , [ O , O , H ] ] [ ( W , E ) , ( N , S ) ]
>>>Caught ( 1 , 1 )
∗Hw1> simulate [ [ P , O , O ] , [ O , O , O ] , [ O , O , H ] ] [ ( W , E ) , ( N , E ) ]
>>>Fail
∗Hw1> simulate [ [ O , O , X ] , [ O , O , X ] , [ O , H , O ] , [ O , X , O ] , [ O , X , P ] ] [ ( E , N ) , ( S , S ) , ( S , W ) , ( E , N ) ]
>>>Caught ( 2 , 4 )
