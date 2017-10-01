#include <QApplication>
#include "calc.h"
#include "../calcOptr.hpp"

int main(int argc, char **argv)
{
  QApplication a(argc, argv);

  makeOperatorHashes();

  CalcUi *calculatorWindow = new CalcUi;

  calculatorWindow->show();

  return a.exec();
}
