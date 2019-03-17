module Hw2 where

import Data.List -- YOU MAY USE THIS MODULE FOR SORTING THE AGENTS

data Level = Newbie | Intermediate | Expert deriving (Enum, Eq, Ord, Show, Read)
data Hunter = Hunter {hID::Int, hlevel::Level, hEnergy::Int, hNumberOfCatches::Int, hActions::[Direction]} deriving (Eq, Show, Read)
data Prey = Prey {pID::Int, pEnergy::Int, pActions::[Direction]} deriving (Eq, Show, Read)
data Cell = O | X | H Hunter | P Prey | T deriving (Eq, Show, Read)
data Direction = N | S | E | W deriving (Eq, Show, Read)
type Coordinate = (Int, Int)
-- DO NOT CHANGE THE DEFINITIONS ABOVE. --


-- INSTANCES OF Ord FOR SORTING, UNCOMMENT AND COMPLETE THE IMPLEMENTATIONS --
instance Ord Hunter where
   compare (Hunter id1 lev1 en1 catch1 dir1) (Hunter id2 lev2 en2 catch2 dir2) = do
     if lev1 /= lev2
       then compare lev1 lev2
       else if en1 /= en2
         then compare en1 en2
         else if catch1 /= catch2
           then compare catch1 catch2
           else if id1 < id2 then GT
             else if id1 == id2 then EQ
               else LT

instance Ord Prey where
   compare (Prey id1 en1 dir1) (Prey id2 en2 dir2) = do
     if en1 /= en2
       then compare en1 en2
       else if id1 < id2 then GT
         else if id1 == id2 then EQ
           else LT

-- WRITE THE REST OF YOUR CODE HERE --
{-

8. The hunter with the highest priority will begin to catch the prey that is the lowest in the ordering,
if multiple hunters and preys reside in the same cell.
9. A hunter will be able to catch at most 2 preys in a time step. There is no limit for a hunter’s
number of catches during whole simulation.

-}


simulate :: [[Cell]] -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
simulate cells = do
  let preysCoordList = findPrey cells
  let huntersCoordList = findHunter cells


  let infoOfPreysList = if preysCoordList /= []
                          then infoOf cells preysCoordList
                          else []

  let infoOfHuntersList = infoOf cells huntersCoordList

  let hTuple = hunterCoordinate infoOfHuntersList huntersCoordList
  let pTuple = preyCoordinate infoOfPreysList preysCoordList

  let newPTuple = pGoesTo cells pTuple
  let newHTuple = hGoesTo cells hTuple

  let hpList = checkDestroyP (sort newPTuple) (reverse (sort newHTuple))

  if infoOfPreysList == []
    then
     (reverse(sort hTuple), [])
     else if snd(hpList) == [] || pActions (fst((snd(hpList))!!0)) == []
       then hpList
       else mySimulate cells hpList


mySimulate :: [[Cell]] -> ([(Hunter, Coordinate)], [(Prey, Coordinate)]) -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
mySimulate cells (hTuple, pTuple) = do
  let newPTuple = pGoesTo cells pTuple
  let newHTuple = hGoesTo cells hTuple

  let hpList = checkDestroyP (sort newPTuple) (reverse (sort newHTuple))

  if newPTuple == []
    then
     (reverse (sort hTuple), [])
     else if snd(hpList) == [] || pActions (fst((snd(hpList))!!0)) == []
       then hpList
       else mySimulate cells hpList



checkDestroyP :: [(Prey, Coordinate)] -> [(Hunter, Coordinate)] -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
checkDestroyP (a:as) (b:bs) = do
  let ps = convertP (a:as)
  let hs = convertH (b:bs)

  let interForP = intersect ps hs
  let interForH = intersect hs ps

  if interForP == [] then ((b:bs), (a:as))
    else destroyP interForP interForH ((b:bs), (a:as))

destroyP :: [Coordinate] -> [Coordinate] -> ([(Hunter, Coordinate)], [(Prey, Coordinate)]) -> ([(Hunter, Coordinate)], [(Prey, Coordinate)])
destroyP (i:is) (j:js) ((b:bs),(a:as)) = do
  let huntersLength = length (j:js)
  let preysLength = length (i:is)

  let deletingPs = if (2*huntersLength) <= preysLength
                    then take (2*huntersLength) (i:is)
                    else (i:is)

  let upgradingHs = if (2*huntersLength) <= preysLength
                      then (j:js)
                      else if preysLength == 1 then [j]
                      else take (round ((fromIntegral preysLength) / (fromIntegral 2))) (j:js)

  let bools = boolList (i:is) (j:js)
  let pTuple = deleteP deletingPs (a:as)
  let hTuple = upgradeH upgradingHs bools (b:bs)

  (hTuple, pTuple)


boolList :: [Coordinate] -> [Coordinate] -> [Bool]
boolList coord (j:js) = do
  let pLeft = filter (/=j) coord
  let pSame = filter (==j) coord

  let plist = take 2 pSame
  let otherlist = drop 2 pSame

  if length plist == 2 && js /= [] then [True] ++ boolList (otherlist ++ pLeft) js
    else if length plist == 2 && js == [] then [True]
      else if (otherlist ++ pLeft) /= [] && js /= [] then [False] ++ boolList (otherlist ++ pLeft) js
        else if (otherlist ++ pLeft) /= [] && js == [] then [False]
          else if (otherlist ++ pLeft) == [] && js /= [] then [False] ++ addFalse js
            else [False]

addFalse :: [Coordinate] -> [Bool]
addFalse (b:bs) = do
  if bs == [] then [False]
    else [False] ++ addFalse bs

deleteP :: [Coordinate] -> [(Prey, Coordinate)] -> [(Prey, Coordinate)]
deleteP (i:is) preys = do
    let pLeft = filter ((/=i).snd) preys
    if is == [] then pLeft
      else deleteP is pLeft


upgradeH :: [Coordinate] -> [Bool] -> [(Hunter, Coordinate)] -> [(Hunter, Coordinate)]
upgradeH (i:is) bools hclist = do
  let hunters = reverse (sort hclist)
  let hLeft = filter ((==i).snd) hunters
  let hOther = filter ((/=i).snd) hunters

  let b = hLeft!!0

  let ((h, tuple)) = b

  let bs = if length hLeft > 0
           then drop 1 hLeft
           else []

  let oldEnergy = hEnergy h
  let newEnergy = if bools!!0 == False then oldEnergy + 20
                  else oldEnergy + 40

  let updateEnH x = if newEnergy > 100
                    then x {hEnergy = 100}
                    else x {hEnergy = newEnergy}

  let oldCatch = hNumberOfCatches h
  let newCatch = if bools!!0 == False then oldCatch + 1
                 else oldCatch + 2

  let updateCatch x = x {hNumberOfCatches = newCatch}

  if bs == [] && is == [] then [((updateCatch(updateEnH h)), tuple)] ++ hOther
    else if bs /= [] && is == [] then [((updateCatch(updateEnH h)), tuple)] ++ bs ++ hOther
      else if hOther /= []
        then [((updateCatch(updateEnH h)), tuple)] ++ upgradeH is (drop 1 bools) hOther
        else if i == is!!0
          then [((updateCatch(updateEnH h)), tuple)] ++ upgradeH is (drop 1 bools) (drop 1 hLeft)
          else [((updateCatch(updateEnH h)), tuple)]



convertP :: [(Prey, Coordinate)] -> [Coordinate]
convertP (a:as) = do
  if as /= [] then [snd(a)] ++ convertP as
    else [snd(a)]

convertH :: [(Hunter, Coordinate)] -> [Coordinate]
convertH (b:bs) = do
  if bs /= [] then [snd(b)] ++ convertH bs
    else [snd(b)]


hunterCoordinate :: [Cell] -> [(Int, Int)] -> [(Hunter, Coordinate)]
hunterCoordinate (x:xs) (y:ys)
  | xs == [] = [(eliminateH x, y)]
  | otherwise = [(eliminateH x, y)] ++ hunterCoordinate xs ys

preyCoordinate :: [Cell] -> [(Int, Int)] -> [(Prey, Coordinate)]
preyCoordinate (x:xs) (y:ys)
  | xs == [] = [(eliminateP x, y)]
  | otherwise = [(eliminateP x, y)] ++ preyCoordinate xs ys


enumerate = zip [0..]

findPrey :: [[Cell]] -> [(Int, Int)]
findPrey cells =
        [(x, y)
          | (y, row) <- enumerate cells
          , (x, P Prey {}) <- enumerate row]

findHunter :: [[Cell]] -> [(Int, Int)]
findHunter cells =
        [(x, y)
          | (y, row) <- enumerate cells
          , (x, H Hunter {}) <- enumerate row]

infoOf :: [[Cell]] -> [(Int, Int)] -> [Cell]
infoOf cells ((a, b) : xs)
  | xs == [] = [(cells!!b!!a)]
  | otherwise = [(cells!!b!!a)] ++ infoOf cells xs


pGoesTo :: [[Cell]] -> [(Prey, Coordinate)] -> [(Prey, Coordinate)]
pGoesTo cells (a:as) = do

    let (p, (x, y)) = a

    let pDirections = pActions p
    let dir = pDirections!!0

    let tuple = if pEnergy p < 10
                  then (x, y)
                  else goesTo cells dir (x, y)


    let oldEnergy = pEnergy p
    let newEnergy = if pEnergy p < 10
                    then oldEnergy + 1
                    else oldEnergy - 1

    let newActions = drop 1 pDirections

    let updateEnP x = if newEnergy < 0 then x {pEnergy = 0}
                      else x {pEnergy = newEnergy}

    let updateTrap x = if newEnergy - 10 < 0 then x {pEnergy = 0}
                      else x {pEnergy = newEnergy - 10}

    let updateActP x = x {pActions = newActions}

    if as /= []
      then
        if cells!!(snd(tuple))!!(fst(tuple)) /= T
          then [((updateActP (updateEnP p)), tuple)] ++ pGoesTo cells as
          else [((updateActP (updateTrap p)), tuple)] ++ pGoesTo cells as
      else
        if cells!!(snd(tuple))!!(fst(tuple)) /= T
          then [((updateActP (updateEnP p)), tuple)]
          else [((updateActP (updateTrap p)), tuple)]


hGoesTo :: [[Cell]] -> [(Hunter, Coordinate)] -> [(Hunter, Coordinate)]
hGoesTo cells (b:bs) = do

  let (h, (x, y)) = b

  let hDirections = hActions h
  let dir = hDirections!!0


  let tuple = if hEnergy h < 10
                then (x, y)
                else goesTo cells dir (x, y)


  let oldEnergy = hEnergy h
  let newEnergy = if hEnergy h < 10
                  then oldEnergy + 1
                  else oldEnergy - 1

  let newActions = drop 1 hDirections

  let updateEnH x = if newEnergy < 0 then x{hEnergy = 0}
                    else x {hEnergy = newEnergy}
  let updateActH x = x {hActions = newActions}

  if bs /= []
    then [((updateActH (updateEnH h)), tuple)] ++ hGoesTo cells bs
    else [((updateActH (updateEnH h)), tuple)]


goesTo :: [[Cell]] -> Direction -> (Int, Int) -> (Int, Int)
goesTo cells dir (x, y)
  | dir == N = check cells (x, y) (x, y - 1)
  | dir == S = check cells (x, y) (x, y + 1)
  | dir == E = check cells (x, y) (x + 1, y)
  | dir == W = check cells (x, y) (x - 1, y)


check :: [[Cell]] -> (Int, Int) -> (Int, Int) -> (Int, Int)
check cells (x, y) (x', y')
    | y' < 0 = (x, y)
    | x' < 0 = (x, y)
    | y' >= length cells = (x, y)
    | x' >= length (cells!!0) = (x, y)
    | cells!!y'!!x' == X = (x, y)
    | otherwise = (x', y')

isP (P Prey{}) = True
isP _ = False

eliminateP (P x) = x
eliminateH (H x) = x
