INTRODUCTION
--------------------------------------------------------------------------------
This program determines the digit that is missing from a
METU student-id.
If no digit is missing, it determines whether the student-id is valid or not.
A student-id will be read from the standard input as ######-#
where each # is a decimal digit or a ‘?’ (question mark).
There will be at most one question mark.

EXAMPLE RUNS
--------------------------------------------------------------------------------
>>> 167912-5
VALID
>>> 167912-?
167912-5
>>> 1679?2-5
167912-5
>>> 13?503-7
139503-7

ABOUT METU STUDENT-ID
--------------------------------------------------------------------------------
The Algorithm is as follows:
• sum ← 0
• For each digit in the student number that has an odd position (first, third,
fifth) take the digit and add it to the sum.
• For each digit in the student number that has an even position (second, forth,
sixth) take twice the digit. If this result of doubling is a two digit number
then add each of these digits to the sum, otherwise add the result of the
doubling (which is a single digit number itself) to the sum.
• The one digit number, which when added to the sum results in a multiple of 10,
is the check digit
