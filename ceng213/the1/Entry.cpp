#include <cstdlib>
#include <iostream>
#include <string>
#include "Entry.hpp"

using namespace std;

std::ostream &operator<<(std::ostream &out, const Entry& t){
    out <<"{"<< t.title<<","<<t.genre<<","<<t.year<<"}";
    return out;
};


/* TO-DO: method implementations below */

Entry::Entry() {
  std::string title = "";
  std::string genre = "";
  std::string year = "";
}

      /* This constructor is the only way to set private members of the class*/
Entry::Entry(std::string _title, std::string _genre, std::string _year) {
    title = _title;
    genre = _genre;
    year = _year;
}

      /*return Title of the entry*/
std::string Entry::getTitle() const {
    return title;
}

      /*return Genre of the entry*/
std::string Entry::getGenre() const {
    return genre;
}

      /*return year of the entry*/
std::string Entry::getYear() const{
    return year;
}

      /* You are expected to overload the “==” operator.
       * If the “title”s of two entries are same, this method returns true*/
bool Entry::operator==(const Entry & rhs) const {
    return title == rhs.getTitle();
}
