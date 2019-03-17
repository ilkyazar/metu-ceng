invalidInput = "INVALID"
validInput = "VALID"

def main():
    inp = raw_input();
    if isInputInExpectedFormat(inp):
        if thereExistQM(inp):
            if thereExistQM(inp[0:6:2]):
                print inp.replace("?", str(valueOfQMforOdds(inp)));

            elif thereExistQM(inp[1:6:2]):
                print inp.replace("?", str(valueOfQMforEvens(inp)));

            else:
                print inp.replace("?",str(checkDigit(inp[:6])));

        else:
            validateCheckDigit(inp);
    else:
        print invalidInput;

def isInputInExpectedFormat(s):
    if len(s) != 8:
        return False;
    elif s[6] != "-":
        return False;
    elif s.count('?') == 1:
        newStr = s.replace('?', "").replace('-', "");
        if not newStr.isdigit():
            return False;
    elif s.count('?') > 1:
        return False;
    elif s.count('?') == 0:
        newStr = s.replace('-', "");
        if not newStr.isdigit():
            return False;
    return True;

def thereExistQM(s):
    return '?' in s;

def calculateSumOfOdds(s):
    odds = oddDigits(s);
    oddsList = list(odds);
    result = 0;
    for x in oddsList:
        result += int(x);
    return result;

def calculateSumOfEvens(s):
    evens = evenDigits(s);
    evensList = list(evens);

    result = 0;
    for x in evensList:
        y = 2 * int(x);
        result += sumOfDigits(list(str(y)));
    return result;

def sumOfDigits(s):
    result = 0;
    for x in s:
        result += int(x);
    return result;

def sumOfAll(s):
    total = calculateSumOfOdds(s) + calculateSumOfEvens(s);
    return total;

def closestMultTen(num):
    result = 10 - (int(num) % 10);
    return result % 10;

def checkDigit(s):
    return closestMultTen(sumOfAll(s));

def validateCheckDigit(s):
    if checkDigit(s[:6]) == int(s[7]):
        print validInput;
    else:
        print invalidInput;

def isOdd(s):
    return s % 2 == 1;

def oddDigits(s):
    count = 1;
    result = "";
    for x in s:
        if isOdd(count):
            result += x;
        count += 1;
    return result;

def isEven(s):
    return s % 2 == 0;

def evenDigits(s):
    count = 1;
    result = "";
    for x in s:
        if isEven(count):
            result += x;
        count += 1;
    return result;

def valueOfQMforOdds(s):
    newInput = s.replace("?",str(0));
    return closestMultTen(sumOfAll(newInput[0:6]) + int(s[7]));

def valueOfQMforEvens(s):
    digitOfQM = s.find("?")
    newInput = s.replace("?",str(0));
    if isEven(closestMultTen(sumOfAll(newInput[0:6]) + int(s[7]))):
        return (closestMultTen(sumOfAll(newInput[0:6]) + int(s[7]))) / 2;
    else:
        return ((closestMultTen(sumOfAll(newInput[0:6]) + int(s[7]))) + 9) / 2;
    s = newInput.replace(digitOfQM,"?")

main();
