INTRODUCTION
--------------------------------------------------------------------------------
The program simulates the Hunter-Prey problem, in a multi-agent approach.
There will be more than one hunter and one prey in the environment.
Each agent will be able to act independently with their given action sets.
Their initial positions will be always different.
However, two or more than agents with same type may reside in the same cell
in a time step. It is assumed that there will not be any collision in such cases.
Agents will have some features so that they can be compared and sorted
to build up a hierarchy in order to decide which hunter will catch the prey
when they all end up in the same cell.
The simulation environment will be represented by a 2D grid (NxM)
where each cell is either an empty cell, a cell with a trap, the cell containing
an obstacle or the cell contains an agent.

PROBLEM DEFINITION
--------------------------------------------------------------------------------
Each agent can move in the given four direction (North, East, South, West),
if it has enough energy and the border is not reached and there is no obstacle
in that direction.
If it does not have enough energy, it will rest in the same cell and gain
some energy.
If it has enough energy but an action that is in the direction of border or
an obstacle is given, the agent will stay in the same cell for the next time step
as well.
It is assumed that no collision will occur if the agents move towards each other
or reside in the same cell. When the prey pass to a cell with a trap,
it will lose some energy.

SPECIFICATIONS
--------------------------------------------------------------------------------
• O will show the empty cells.
• X will show the cells with an obstacle.
• H Hunter will show the cell that contains a hunter.
• P Prey will show the cell that contains a prey.
• T will show the cells with a trap.
• The cells with letters H and P contain the hunter and the prey respectively.
• The coordinate of the top left corner is (0,0).

Each prey will have;
• a unique integer as id,
• an integer between 0 and 100, representing the energy it has,
• and a list of Direction corresponding the actions it will take during the simulation.

data Prey = Prey { pID : : Int , pEnergy : : Int , pActions : : [ Direction ] } deriving ( Eq , Show
, Read )

Each hunter will have;
• a unique integer as id,
• a Level type corresponding its experience in hunting,
• an integer between 0 and 100, representing the energy it has,
• an integer for the number of catches during the simulation,
• and a list of Direction corresponding the actions it will take during the simulation.

data Hunter = Hunter { hID : : Int , hlevel : : Level , hEnergy : : Int , hNumberOfCatches : : Int ,
hActions : : [ Direction ] } deriving ( Eq , Show , Read )

Definition of Level type:
data Level = Newbie | Intermediate | Expert deriving ( Enum , Eq , Ord , Show , Read )

Definition of Direction type:
data Direction = N | S | E | W deriving ( Eq , Show , Read )

The comparison of two preys will be as follows:
• The prey with more energy will be prior than the other prey.
• If their energies are equal, then the prey with smaller id will be prior.

The comparison of two hunters will be as follows:
• The hunter with upper level will be prior then the hunter with lower level.
• If levels are equal, then the one with more energy will be prior.
• If energies are equal, then the hunter which caught more prey will be prior.
• If the number of catches are also equal, then the hunter with smaller id will
be prior.

RULES OF SIMULATION
--------------------------------------------------------------------------------
1. The number of actions in given sequences will be equal for all of the agents.
2. Each step decreases the energy of an agent by 1. This will be applied even if
the agent cannot pass another cell due to an obstacle or the border.
3. An agent with less energy than 10 cannot move and misses its current action.
It rests in the same cell for a timestep and this will increase its energy by 1.
4. When a prey ends up in a cell with trap its energy will be reduced by 10.
5. A trap in the environment (if there exists) is always static. That is,
the trap will not be removed after a prey falls into.
6. When a hunter catches a prey, its energy will increase by 20 if its total
energy does not exceed 100.
If such a case occur the energy of the hunter will be 100.
7. The hunter with the highest priority will begin to catch the prey that is
the lowest in the ordering, if multiple hunters and preys reside in the same cell.
8. A hunter will be able to catch at most 2 preys in a time step.
There is no limit for a hunter’s number of catches during whole simulation.

EXAMPLE RUNS
--------------------------------------------------------------------------------
∗Hw2> sort [ Prey 1 100 [ S , E ] , Prey 2 90 [ S , N ] , Prey 3 100 [ E , N ] ]
[ Prey { pID = 2 , pEnergy = 9 0 , pActions = [ S , N ] } , Prey { pID = 3 , pEnergy = 1 0 0 ,
pActions = [ E , N ] } , Prey { pID = 1 , pEnergy = 1 0 0 , pActions = [ S , E ] } ]
∗Hw2> sort [ Hunter 1 Expert 100 10 [ N , W ] , Hunter 2 Expert 80 5 [ E , S ] , Hunter 3
Newbie 80 1 [ N , N ] , Hunter 4 Expert 100 10 [ S , W ] ]
[ Hunter { hID = 3 , hlevel = Newbie , hEnergy = 8 0 , hNumberOfCatches = 1 , hActions = [
N , N ] } , Hunter { hID = 2 , hlevel = Expert , hEnergy = 8 0 , hNumberOfCatches = 5 ,
hActions = [ E , S ] } , Hunter { hID = 4 , hlevel = Expert , hEnergy = 1 0 0 ,
hNumberOfCatches = 1 0 , hActions = [ S , W ] } , Hunter { hID = 1 , hlevel = Expert ,
hEnergy = 1 0 0 , hNumberOfCatches = 1 0 , hActions = [ N , W ] } ]
∗Hw2> simulate [ [ P ( Prey 1 70 [ S , E , W , S , N , E ] ) , O , X , P ( Prey 2 100 [ N , N , W , S , E , E ] ) ] ,
[ T , O , H ( Hunter 1 Expert 90 2 [ E , S , N , E , S , W ] ) , O ] , [ H ( Hunter 2 Newbie 70 0 [ E , N ,
S , W , N , N ]) , O , X , O ]]
( [ ( Hunter { hID = 1 , hlevel = Expert , hEnergy = 1 0 0 , hNumberOfCatches = 3 , hActions
= [ S , W ] } , ( 3 , 1 ) ) , ( Hunter { hID = 2 , hlevel = Newbie , hEnergy = 8 6 ,
hNumberOfCatches = 1 , hActions = [ N , N ] } , ( 0 , 2 ) ) ] , [ ] )
∗Hw2> simulate [ [ P ( Prey 1 70 [ E , W , E , W , E , W ] ) , O , X , P ( Prey 2 100 [ N , N , W , S , E , E ] ) ] ,
[ T , O , H ( Hunter 1 Expert 90 2 [ E , S , N , E , S , W ] ) , O ] , [ H ( Hunter 2 Newbie 70 0 [ E , N ,
S , W , E , N ]) , O , X , O ]]
( [ ( Hunter { hID = 1 , hlevel = Expert , hEnergy = 9 8 , hNumberOfCatches = 3 , hActions =
[ ] } , ( 3 , 2 ) ) , ( Hunter { hID = 2 , hlevel = Newbie , hEnergy = 6 4 , hNumberOfCatches =
0 , hActions = [ ] } , ( 1 , 1 ) ) ] , [ ( Prey { pID = 1 , pEnergy = 6 4 , pActions = [ ] } , ( 0 , 0 ) ) ] )
