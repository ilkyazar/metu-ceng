INTRODUCTION
--------------------------------------------------------------------------------
This is an introductory level Random Natural Language Generator. Domain
language is Turkish.
The program breaks down each word into its syllables. Then the
statistics (counts) of which syllable follows which syllable is collected over
all the words.

EXAMPLE RUN
--------------------------------------------------------------------------------
python the3exec.py < test.txt

RESTRICTIONS
--------------------------------------------------------------------------------
• The first input line contains two integers n and m. n is the expected count
of words to be formed in the output. m is the maximal count of characters
of the output.
• All the following lines before the last line are the input text.
• The last line consists of a single character: An equal sign: =
• The input text is lowercase only (the capitalisation rules of Turkish writing
does not apply). Upper case letters always represent Turkish special characters.
• Input text does not contain word breaks over line endings.
• The only punctuation that will appear is the sentence-ending dot.
• Words are separated exactly by one blank.
• Line starts and endings do not contain blanks.
• The last line ends with a period.
• A word is any sequence of letters terminated by a space or period.
• A period is not followed by a space.
• The first syllable of the input text shall be assumed to be following a blank
(so it shall contribute to the statistics of the syllables that follow a blank).
This blank will not be present in the input.
