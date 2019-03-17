module Hw1 where

data Cell = H | P | O | X deriving (Read,Show,Eq)
data Result = Fail | Caught (Int,Int) deriving (Read,Show,Eq)
data Direction = N | S | E | W deriving (Read,Show,Eq)

simulate :: [[Cell]] -> [(Direction, Direction)] -> Result
-- DO NOT CHANGE ABOVE THIS LINE, WRITE YOUR CODE BELOW --
simulate cells ((a1, a2) : xs) = do

  let huntersPosition = findHunter cells
  let preysPosition = findPrey cells

  let huntersDirection = a1
  let preysDirection = a2

  let huntersNewPos = goesTo cells huntersPosition huntersDirection
  let preysNewPos = goesTo cells preysPosition preysDirection

  if (huntersNewPos == preysNewPos)
    then Caught huntersNewPos
    else if xs /= []
      then mySimulate cells huntersNewPos preysNewPos xs
      else Fail

mySimulate :: [[Cell]] -> (Int, Int) -> (Int, Int) -> [(Direction, Direction)] -> Result
mySimulate cells (h1, h2) (p1, p2) ((a1, a2) : xs) = do
  let huntersDirection = a1
  let preysDirection = a2

  let huntersNewPos = goesTo cells (h1, h2) huntersDirection
  let preysNewPos = goesTo cells (p1, p2) preysDirection

  if (huntersNewPos == preysNewPos)
    then Caught huntersNewPos
    else if xs /= []
      then mySimulate cells huntersNewPos preysNewPos xs
      else Fail


enumerate = zip [0..]
findHunter :: [[Cell]] -> (Int, Int)
findHunter cells = do
        makeTuple [(x, y)
          | (y, row) <- enumerate cells
          , (x, cell) <- enumerate row
          , cell == H]

findPrey :: [[Cell]] -> (Int, Int)
findPrey cells = do
        makeTuple [(x, y)
          | (y, row) <- enumerate cells
          , (x, cell) <- enumerate row
          , cell == P]

makeTuple :: [(Int, Int)] -> (Int, Int)
makeTuple [(x, y)] = (x, y)

goesTo :: [[Cell]] -> (Int, Int) -> Direction -> (Int, Int)
goesTo cells (x, y) direction
    | direction == N = check cells (x, y) (x, y - 1)
    | direction == S = check cells (x, y) (x, y + 1)
    | direction == E = check cells (x, y) (x + 1, y)
    | direction == W = check cells (x, y) (x - 1, y)

check :: [[Cell]] -> (Int, Int) -> (Int, Int) -> (Int, Int)
check cells (x, y) (x', y')
    | y' < 0 = (x, y)
    | x' < 0 = (x, y)
    | y' >= length cells = (x, y)
    | x' >= length (cells!!0) = (x, y)
    | cells!!y'!!x' == X = (x, y)
    | otherwise = (x', y')
