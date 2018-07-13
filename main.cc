#include <iostream>
#include <sstream>
#include "Polynomial.h"
using namespace std;

/////////////////////////////////////////////
// This file was given as a starting point //
/////////////////////////////////////////////

// a class to store an unlimited
// number of Polynomials
struct PolynomialTable {
  int size;
  int capacity;
  Polynomial **table; // array of Polynomial pointers

  PolynomialTable() : size{0}, capacity{0}, table{nullptr} {}

  ~PolynomialTable() {
    if (size) {
      for (int i=0; i<size; ++i) delete table[i];
      delete[] table;
    }
  }

  //create a new Polynomial at the next possible position
  int add(int c = 0, int e = 0) {
    if (size==capacity) realloc();
    table[size] = new Polynomial{c,e};
    return size++;
  }

  //create a new Polynomial using a string at next possible position.
  int add(const string &s) {
    if (size==capacity) realloc();
    table[size] = new Polynomial{s};
    return size++;
  }

  //create a new Polynomial at the next possible position
  //as a copy at position x
  int addCopy(int x) {
    if (size==capacity) realloc();
    table[size] = new Polynomial {*table[x]};
    return size++;
  }
  //create a new Polynomial at the next possible position
  //as a move of another Polynomial.
  int addMove(Polynomial &p) {
    if (size==capacity) realloc();
    table[size] = new Polynomial {std::move(p)};
    return size++;
  }
  //assign Polynomial at pos j to pos i
  void assign(int i, int j) {
    *table[i] = *table[j];
  }

  //increase the capacity of the table
  void realloc() {
    if (capacity==0) {
      table = new Polynomial*[5];
      capacity=5;
      return;
    }
    capacity=capacity*2;
    Polynomial** temp = new Polynomial*[capacity];
    for (int i=0; i<size; ++i) temp[i]=table[i];
    delete[] table;
    table=temp;
  }
};


// Test harness for Polynomial methods.
// Please read it carefully!
int main(){
  bool done = false;
  PolynomialTable t;
  while (!done){
    char command;
    int lhs, rhs;

    cin >> command;
    if (cin.eof()) break;
    switch(command){
    case 'n':{
      // Usage:
      // n create a new zero polynomial
      // n <number> create a polynomial valued <number>
      // n <coefficient> <exponent> create a polynomial term with
      // the corresponding coefficient and exponent.
      string rest;
      getline(cin,rest);
      istringstream iss{rest};
      lhs = rhs = 0;
      iss >> lhs;
      iss >> rhs;
      cerr << "Polynomial Created at: " << t.add(lhs,rhs) << endl;
    }
      break;
    case 's':{
      // Usage:
      // s <string>
      // creates a polynomial using the string format
      string rest;
      getline (cin,rest);
      cerr << "Polynomial Created at: " << t.add(rest) << endl;
    }
      break;
    case 'i': {
      // i <index> <string>
      // Use the input operator to read into existing polynomial
      // input operator reads string till end of line
      cin >> lhs;
      cin >> *t.table[lhs];
    }
      break;
    case 'p':{
      // p <index> print the polynomial at index.
      cin >> lhs;
      cout << *t.table[lhs] << endl;
    } break;
    case 'c':{
      // c <index> invokes copy constructor, creates a copy of polynomial at
      // index.
      cin >> lhs;
      cerr << "Polynomial Created at: " << t.addCopy(lhs) << endl;
    } break;
    case 'm':{
      // m <string>
      // reads a polynomial by string
      // creates a temporary object and move-construct it
      string rest;
      getline (cin,rest);
      Polynomial p{rest};
      cerr << "Polynomial Created at: " << t.addMove(p) << endl;
    } break;
    case '=':{
      // = <index1> <index2> assigns polynomial in index2 to index1
      cin >> lhs >> rhs;
      t.assign(lhs,rhs);
    } break;
    case 'v':{
      // v <index1> <index2> creates a copy of polynomial in index2,
      // and move-assign it to index1.
      cin >> lhs >> rhs;
      Polynomial p{*t.table[rhs]};
      *t.table[lhs] = std::move(p);
    } break;
    case 'e':{
      // e <index1> <index2> prints true if two polynomials at index1
      // and index2 are equal, false otherwise.
      cin >> lhs >> rhs;
      cout << boolalpha << (*t.table[lhs] == *t.table[rhs]) << noboolalpha << endl;
    } break;
    case '+':{
      // + <index1> <index2> print the sum of two polynomials at index1 and index2
      cin >> lhs >> rhs;
      cout << (*t.table[lhs] + *t.table[rhs])<< endl;
    } break;
    case '-':{
      // - <index1> <index2> print the difference of two polynomials at index1 and index2
      cin >> lhs >> rhs;
      cout << (*t.table[lhs] - *t.table[rhs])<< endl;
    } break;
    case '*':{
      // * <index1> <index2> print the product of polynomials at index1 and index2
      cin >> lhs >> rhs;
      cout << (*t.table[lhs] * *t.table[rhs])<< endl;
    } break;
    case 'u':{
      // u <value> <index> substitutes value into the polynomial at index
      // and print the result.
      cin >> lhs >> rhs;
      cout << ((*t.table[rhs])(lhs)) << endl;
    } break;
    case 'g':{
      // g <exponent> <index> get the coefficient of the exponent of the polynomial at index
      // and print the result.
      cin >> lhs >> rhs;
      cout << ((*t.table[rhs])[lhs]) << endl;
    } break;

    case 'q':
      done = true;
    }
  }
}
