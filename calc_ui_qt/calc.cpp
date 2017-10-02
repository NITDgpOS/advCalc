#include <iostream>
#include <cstdio>
#include "calc.h"
#include <QRegExp>
#include <QDebug>
#include "../calcParser.hpp"

CalcUi::CalcUi(QWidget *parent)
  : QDialog(parent)
{
  setupUi(this);

  buttonConfigure->toggle();

  buttonOF_checked = buttonOtherFunctions->isChecked();

  lineEditInput->setHidden(false);
  keyCheckBox->setHidden(true);
  configureLayoutWidget->setHidden(true);

  textBrowserOutput->setMinimumWidth(500);
  textBrowserOutput->setMinimumHeight(325);
  textBrowserOutput->setMaximumHeight(325);

  layout()->setSizeConstraint(QLayout::SetFixedSize);

  QRegExp regExpIdentifier("[A-Za-z_]{1,20}");
  lineEditConstantName->setValidator(new QRegExpValidator(regExpIdentifier, this));

  lineEditInput->setFocus();
}

void CalcUi::on_buttonCalculate_clicked() {
  QString input = lineEditInput->text();
  std::string in = input.toStdString();
  QString msg = textBrowserOutput->toPlainText() + input + " ";
  try { // Parsing the input
    calcParse<float64_t> parser(in.c_str());
    parser.startParsing();
    char ans[30];
    sprintf(ans, "%lg", parser.Ans());
    qDebug() << input << "=" << parser.Ans();
    msg = msg + "= " + ans;
  } catch (ERROR *e) { // Catch any errors
    if (e->isSet()) {
      QString err = "Error: ";
      qDebug() << input << "Error: " << e->toString();
      msg = msg + err + e->toString();
    }
    delete e;
  }
  msg = msg + "\n";
  textBrowserOutput->setText(msg);
  lineEditInput->clear();
}

void CalcUi::on_lineEditInput_textChanged()
{
  QString input = lineEditInput->text();
  buttonClear->setEnabled(!input.isEmpty());
  buttonDelete->setEnabled(!input.isEmpty());
  buttonCalculate->setEnabled(!input.isEmpty());
  QString msg;
  if (not input.isEmpty()) {
    std::string in = input.toStdString();
    try {
      calcParse<float64_t> parser(in.c_str());
      parser.startParsing();
      char ans[30];
      sprintf(ans, "%lg", parser.Ans());
      msg = ans;
    } catch (ERROR *e) {
      if (e->isSet()) {
        QString err = "Error: ";
        msg = err + e->toString();
      }
      delete e;
    }
  } else {
    msg = "Enter an expression";
  }
  outputDisplayLabel->setText(msg);
}

void CalcUi::on_lineEditConstantName_textChanged()
{
  buttonAddConstant->setEnabled(
                                lineEditConstantName->hasAcceptableInput() &&
                                !lineEditConstantValue->text().isEmpty());
  buttonRemoveConstant->setEnabled(lineEditConstantName->hasAcceptableInput());
}

void CalcUi::on_lineEditConstantValue_textChanged()
{
  buttonAddConstant->setEnabled(
                                lineEditConstantName->hasAcceptableInput() &&
                                !lineEditConstantValue->text().isEmpty());
}

void CalcUi::on_keyCheckBox_toggled(bool checked)
{
  if (checked)
    buttonOtherFunctions->setChecked(not checked);
  else
    buttonOtherFunctions->setChecked(buttonOF_checked);
}

void CalcUi::on_buttonOtherFunctions_clicked()
{
  buttonOF_checked = buttonOtherFunctions->isChecked();
}

void CalcUi::on_comboBox_activated(const QString &arg1)
{
  if (arg1 == "Mouse Only")
    {
      lineEditInput->setReadOnly(true);
      keyCheckBox->setChecked(false);
    }
  else if (arg1 == "Keyboard Only")
    {
      lineEditInput->setReadOnly(false);
      keyCheckBox->setChecked(true);
    }
  else if (arg1 == "Mouse and Keyboard")
    {
      lineEditInput->setReadOnly(false);
      keyCheckBox->setChecked(false);
    }
}

void CalcUi::on_buttonDelete_clicked()
{
  int t = lineEditInput->cursorPosition();
  lineEditInput->setText(lineEditInput->text().left(t ? t - 1 : 0) + lineEditInput->text().right(lineEditInput->text().length() - t));
  lineEditInput->setCursorPosition(t);
}

void CalcUi::on_button1_clicked()
{
  setLineEditInput("1");
}

void CalcUi::on_button2_clicked()
{
  setLineEditInput("2");
}

void CalcUi::on_button3_clicked()
{
  setLineEditInput("3");
}

void CalcUi::on_button4_clicked()
{
  setLineEditInput("4");
}

void CalcUi::on_button5_clicked()
{
  setLineEditInput("5");
}

void CalcUi::on_button6_clicked()
{
  setLineEditInput("6");
}

void CalcUi::on_button7_clicked()
{
  setLineEditInput("7");
}

void CalcUi::on_button8_clicked()
{
  setLineEditInput("8");
}

void CalcUi::on_button9_clicked()
{
  setLineEditInput("9");
}

void CalcUi::on_button0_clicked()
{
  setLineEditInput("0");
}

void CalcUi::on_buttonPlus_clicked()
{
  setLineEditInput("+");
}

void CalcUi::on_buttonMinus_clicked()
{
  setLineEditInput("-");
}

void CalcUi::on_buttonMultiply_clicked()
{
  setLineEditInput("*");
}

void CalcUi::on_buttonDivide_clicked()
{
  setLineEditInput("/");
}

void CalcUi::on_buttonPow_clicked()
{
  setLineEditInput("^");
}

void CalcUi::on_buttonDot_clicked()
{
  setLineEditInput(".");
}

void CalcUi::on_buttonOpenBracket_clicked()
{
  setLineEditInput("(");
}

void CalcUi::on_buttonCloseBracket_clicked()
{
  setLineEditInput(")");
}

void CalcUi::on_buttonSpace_clicked()
{
  setLineEditInput(" ");
}

void CalcUi::on_buttonSin_clicked()
{
  setLineEditInput("sin");
}

void CalcUi::on_buttonCos_clicked()
{
  setLineEditInput("cos");
}

void CalcUi::on_buttonTan_clicked()
{
  setLineEditInput("tan");
}

void CalcUi::on_buttonAsin_clicked()
{
  setLineEditInput("asin");
}

void CalcUi::on_buttonAcos_clicked()
{
  setLineEditInput("acos");
}

void CalcUi::on_buttonAtan_clicked()
{
  setLineEditInput("atan");
}

void CalcUi::on_buttonLog_clicked()
{
  setLineEditInput("log");
}

void CalcUi::on_buttonLn_clicked()
{
  setLineEditInput("ln");
}

void CalcUi::on_buttonLog10_clicked()
{
  setLineEditInput("logten");
}

void CalcUi::on_buttonFact_clicked()
{
  setLineEditInput("!");
}

void CalcUi::on_buttonP_clicked()
{
  setLineEditInput("P");
}

void CalcUi::on_buttonC_clicked()
{
  setLineEditInput("C");
}

void CalcUi::on_buttonSinh_clicked()
{
  setLineEditInput("sinh");
}

void CalcUi::on_buttonCosh_clicked()
{
  setLineEditInput("cosh");
}

void CalcUi::on_buttonTanh_clicked()
{
  setLineEditInput("tanh");
}

void CalcUi::on_buttonAmp_clicked()
{
  setLineEditInput("&");
}

void CalcUi::on_buttonPipe_clicked()
{
  setLineEditInput("|");
}
