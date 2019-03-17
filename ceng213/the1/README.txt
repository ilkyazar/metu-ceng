INTRODUCTION
--------------------------------------------------------------------------------
This is an implementation of a playlist editor. Editing the playlist is possible
by inserting/deleting an entry, moving an entry to left or right,
reversing the list, sorting the list or shuffle the list.
Also merging two playlist entries should also be possible.
The playlist editor has also a limited UNDO capability which will be able
to undo only insert, delete and reverse operations.


Testing MyStack
--------------------------------------------------------------------------------
g++ main_MyStack.cpp -o MyStack

./MyStack
./MyStack > main_MyStack.out
valgrind ./MyStack

Testing LinkedList
--------------------------------------------------------------------------------
g++ main_LinkedList.cpp -o LinkedList
./LinkedList
./LinkedList > main_LinkedList.out
valgrind ./LinkedList

Testing Playlist
--------------------------------------------------------------------------------
g++ main_Playlist.cpp Playlist.cpp Entry.cpp HistoryRecord.cpp -o Playlist
./Playlist
./Playlist > main_Playlist.out
valgrind ./Playlist
