#include <iostream>
#include <stdlib.h>

#include "scanner.h"
#include "parser.h"

using namespace std;

int main ()
{

  char *filename;

  filename = "T5.txt";

  // Declare a Scanner object and a Parser object.
  Scanner s(filename);
  Parser p(&s);

  if (p.parse_program()){
    cout << "Parse successful." << endl;
  } else {
    cout << "Parse failed."    << endl;
  }

}
