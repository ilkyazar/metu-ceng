def hyphenate(word):
    looping = True;
    pivot = 0;
    lastPivot = 0;
    hyphens = [];

    if not word:
        return hyphens;

    while looping:
        pivot, letter = changePivotBy(0, pivot, word);
        if isVowel(letter):                                                     # if a
            if isEnd(pivot, word):
                hyphens.append(word[lastPivot:pivot+1]);
                break;
            pivot, letter = changePivotBy(1, pivot, word);
            if isVowel(letter):                                                 # if c
                hyphens.append(word[lastPivot:pivot]);
                if not isEnd(pivot, word):
                    lastPivot = pivot;
                    continue;
                else:
                    lastPivot = pivot;
                    hyphens.append(word[lastPivot:pivot+1]);
                    break;
            else:                                                               # else c
                while True:
                    if isEnd(pivot, word):                                      # if d
                        hyphens.append(word[lastPivot:(pivot+1)]);
                        looping = False;                                        # finish outer while
                        break;                                                  # finish inner while
                    else:                                                       # else d
                        pivot, letter = changePivotBy(1, pivot, word);
                        if isVowel(letter):                                     # if e
                            hyphens.append(word[lastPivot:(pivot-1)]);
                            pivot, letter = changePivotBy(-1, pivot, word);
                            lastPivot = pivot;
                            break;                                              # continue outer while
                        else:                                                   # else e
                            continue;
        else:                                                                   # else a
            if isEnd(pivot, word):                                              # if b
                hyphens.append(word[lastPivot:pivot+1])
                break;
            else:                                                               #else b
                pivot, letter = changePivotBy(1, pivot, word);
                continue;
    return hyphens;

def isVowel(letter):
    vowels = ['a', 'e', 'I', 'i', 'o', 'O', 'u', 'U'];
    return letter in vowels;

def isEnd(pivot, word):
    return pivot == len(word)-1;

def changePivotBy(amount, pivot, word):
    newPivot = pivot + amount;
    letter = word[newPivot];
    return newPivot, letter;

def execute():
    firstLine = raw_input();
    n = int(firstLine.split()[0]);
    m = int(firstLine.split()[1]);
    hyphenTree = {};
    beforeLineEndsWithPeriod = False;
    beforeChar = " ";
    word = "";
    while True:
        text = raw_input();
        for char in text:
            if char != " " and char != "." and char != "=" and char != "\n":
                word += char;
            elif char == "=":
                hyphens = hyphenate(word);
                hyphens.insert(0,beforeChar);
                addHyphensIntoTree(hyphens, hyphenTree);
            else:
                hyphens = hyphenate(word);
                hyphens.append(char);
                hyphens.insert(0,beforeChar);
                addHyphensIntoTree(hyphens, hyphenTree);
                beforeChar = char;
                word = "";
        if "=" in text:
            break;
    firstHyphen = getRandomAfterHyphen(" ", hyphenTree)[" "];
    print outputText(firstHyphen, hyphenTree, n);

def outputText(firstHyphen, hyphenTree, n):
    outputText = "";
    newHyphen = getRandomAfterHyphen(firstHyphen, hyphenTree)[firstHyphen];
    countOfWords = 0;
    while True:
        outputText += newHyphen;
        if newHyphen == " " or newHyphen == ".":
            countOfWords += 1;
        if countOfWords >= n:
            break;
        newHyphen = getRandomAfterHyphen(newHyphen, hyphenTree)[newHyphen];
    return outputText;

def addHyphensIntoTree(hyphens, hyphenTree):
    index = 0;
    for hyphen in hyphens:
        # create a hyphen dictionary in hyphen tree
        if hyphen not in hyphenTree:
            hyphenTree[hyphen] = {};
        # add hyphen statistics into hyphenBefore dictionary
        if index != 0: #if is 0 : there is no hyphenBefore
            hyphenBefore = hyphens[index-1];
            hyphenBeforeDict = hyphenTree[hyphenBefore];
            insertHyphenIntoDictionary(hyphen, hyphenBeforeDict);
        index += 1;

def insertHyphenIntoDictionary(hyphen, hyphenDictionary):
    if hyphen not in hyphenDictionary:
        hyphenDictionary[hyphen] = 1;
    else:
        hyphenDictionary[hyphen] += 1;

def getRandomAfterHyphen(hyphen, hyphenTree):
    hyphenAndFollower = {};
    followerHyphens = [];
    followerHyphens.append(hyphenTree[hyphen]);
    listOfCount =[];
    for k in followerHyphens[0]:
        hyphen, nextHyphen, count = hyphen, k, followerHyphens[0][k];
        listOfCount.append(nextHyphen);
        listOfCount.append(count);
    first = listOfCount[1::2][0];
    hyphenFirst = listOfCount[0::2][0];
    second = listOfCount [1::2][0];
    hyphenSecond = listOfCount[0::2][0];
    for i in listOfCount[1::2]:
        if i < first and i < second and first == second:
            second = i;
        elif i < first and i > second:
            second = i;
        elif i > first:
            second = first;
            first = i;
        elif i == first:
            second = i;
    myDict = {"f":followerHyphens[0].keys()[followerHyphens[0].values().index(first)], "s":followerHyphens[0].keys()[followerHyphens[0].values().index(second)]};
    import random;
    hyphenAndFollower.update({hyphen:myDict[random.choice(myDict.keys())]});
    return hyphenAndFollower;
