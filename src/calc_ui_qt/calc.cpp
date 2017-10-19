#include <iostream>
#include <cstdio>
#include "calc.h"
#include <QRegExp>
#include <QDebug>
#include "calcParser.hpp"

CalcUi::CalcUi(QWidget *parent)
  : QDialog(parent)
{
  setupUi(this);

  buttonConfigure->toggle();

  lineEditInput->setHidden(false);
  keyCheckBox->setHidden(true);
  configureLayoutWidget->setHidden(true);

  outputTable->setMinimumWidth(500);
  outputTable->setMinimumHeight(325);
  outputTable->setMaximumHeight(325);

  layout()->setSizeConstraint(QLayout::SetFixedSize);

  QRegExp regExpIdentifier("[A-Za-z_]{1,20}");
  lineEditConstantName->setValidator(new QRegExpValidator(regExpIdentifier, this));

  lineEditInput->setFocus();
}

void CalcUi::on_buttonCalculate_clicked() {
  QString expression = lineEditInput->text().simplified();
  try { // Parsing the expression
    std::string in = expression.toStdString();
    QString answer;
    calcParse<float64_t> parser(in.c_str());
    parser.startParsing();
    char ans[30];
    sprintf(ans, "%lg", parser.Ans());
    qDebug() << expression << "=" << parser.Ans();
    answer = ans;
    int currentRow = outputTable->rowCount();
    QTableWidgetItem *expressionItem = new QTableWidgetItem(expression);
    QTableWidgetItem *answerItem = new QTableWidgetItem(answer);
    expressionItem->setFlags(Qt::ItemIsEnabled);
    answerItem->setFlags(Qt::ItemIsEnabled);
    outputTable->insertRow(currentRow);
    outputTable->setItem(currentRow, 0, expressionItem);
    outputTable->setItem(currentRow, 1, answerItem);
    lineEditInput->clear();
  } catch (ERROR *e) { // Catch any errors
    if (e->isSet()) {
      qDebug() << expression << "Error: " << e->toString();
      delete e;
    }
  }
}

void CalcUi::on_lineEditInput_textChanged(QString expression)
{
  expression = expression.simplified();
  buttonClear->setEnabled(!expression.isEmpty());
  buttonDelete->setEnabled(!expression.isEmpty());
  QString msg;
  if (not expression.isEmpty()) {
    std::string in = expression.toStdString();
    try {
      calcParse<float64_t> parser(in.c_str());
      parser.storeAnswers = false;
      parser.startParsing();
      char ans[30];
      sprintf(ans, "%lg", parser.Ans());
      msg = ans;
      buttonCalculate->setEnabled(true);
    } catch (ERROR *e) {
      if (e->isSet()) {
        QString err = "Error: ";
        msg = err + e->toString();
      }
      buttonCalculate->setEnabled(false);
      delete e;
    }
  } else {
    msg = "Enter an expression";
    buttonCalculate->setEnabled(false);
  }
  outputDisplayLabel->setText(msg);
}

void CalcUi::on_lineEditConstantName_textChanged()
{
  buttonAddConstant->setEnabled(lineEditConstantName->hasAcceptableInput() &&
                                !lineEditConstantValue->text().isEmpty());
  buttonRemoveConstant->setEnabled(lineEditConstantName->hasAcceptableInput());
}

void CalcUi::on_lineEditConstantValue_textChanged()
{
  buttonAddConstant->setEnabled(lineEditConstantName->hasAcceptableInput() &&
                                !lineEditConstantValue->text().isEmpty());
}

void CalcUi::on_keyCheckBox_toggled(bool checked)
{
  buttonOtherFunctions->setChecked(not checked);
  buttonOtherFunctions->setHidden(checked);
  button1->setHidden(checked);
  button2->setHidden(checked);
  button3->setHidden(checked);
  button4->setHidden(checked);
  button5->setHidden(checked);
  button6->setHidden(checked);
  button7->setHidden(checked);
  button8->setHidden(checked);
  button9->setHidden(checked);
  button0->setHidden(checked);
  buttonCloseBracket->setHidden(checked);
  buttonOpenBracket->setHidden(checked);
  buttonPlus->setHidden(checked);
  buttonMinus->setHidden(checked);
  buttonMultiply->setHidden(checked);
  buttonDivide->setHidden(checked);
  buttonPow->setHidden(checked);
  buttonDot->setHidden(checked);
  buttonLn->setHidden(checked);
  buttonFact->setHidden(checked);
  buttonSpace->setHidden(checked);
  button3->setHidden(checked);
  buttonClear->setHidden(checked);
  buttonDelete->setHidden(checked);
  buttonCalculate->setHidden(checked);
  keyboardLine->setHidden(checked);
}

void CalcUi::on_buttonOtherFunctions_toggled(bool checked)
{
  buttonSin->setVisible(checked);
  buttonCos->setVisible(checked);
  buttonTan->setVisible(checked);
  buttonAsin->setVisible(checked);
  buttonAcos->setVisible(checked);
  buttonAtan->setVisible(checked);
  buttonAmp->setVisible(checked);
  buttonLog->setVisible(checked);
  buttonLog10->setVisible(checked);
  buttonPipe->setVisible(checked);
  buttonP->setVisible(checked);
  buttonC->setVisible(checked);
  buttonSinh->setVisible(checked);
  buttonCosh->setVisible(checked);
  buttonTanh->setVisible(checked);
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
  lineEditInput->setFocus();
}

void CalcUi::on_buttonDelete_clicked()
{
  int t = lineEditInput->cursorPosition();
  lineEditInput->setText(lineEditInput->text().left(t ? t - 1 : 0) +
                         lineEditInput->text().
                         right(lineEditInput->text().length() - t));
  lineEditInput->setCursorPosition(t);
  lineEditInput->setFocus();
}

void CalcUi::on_button1_clicked()
{
  setLineEditInput("1");
  lineEditInput->setFocus();
}

void CalcUi::on_button2_clicked()
{
  setLineEditInput("2");
  lineEditInput->setFocus();
}

void CalcUi::on_button3_clicked()
{
  setLineEditInput("3");
  lineEditInput->setFocus();
}

void CalcUi::on_button4_clicked()
{
  setLineEditInput("4");
  lineEditInput->setFocus();
}

void CalcUi::on_button5_clicked()
{
  setLineEditInput("5");
  lineEditInput->setFocus();
}

void CalcUi::on_button6_clicked()
{
  setLineEditInput("6");
  lineEditInput->setFocus();
}

void CalcUi::on_button7_clicked()
{
  setLineEditInput("7");
  lineEditInput->setFocus();
}

void CalcUi::on_button8_clicked()
{
  setLineEditInput("8");
  lineEditInput->setFocus();
}

void CalcUi::on_button9_clicked()
{
  setLineEditInput("9");
  lineEditInput->setFocus();
}

void CalcUi::on_button0_clicked()
{
  setLineEditInput("0");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonPlus_clicked()
{
  setLineEditInput("+");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonMinus_clicked()
{
  setLineEditInput("-");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonMultiply_clicked()
{
  setLineEditInput("*");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonDivide_clicked()
{
  setLineEditInput("/");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonPow_clicked()
{
  setLineEditInput("^");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonDot_clicked()
{
  setLineEditInput(".");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonOpenBracket_clicked()
{
  setLineEditInput("(");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonCloseBracket_clicked()
{
  setLineEditInput(")");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonSpace_clicked()
{
  setLineEditInput(" ");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonSin_clicked()
{
  setLineEditInput("sin");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonCos_clicked()
{
  setLineEditInput("cos");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonTan_clicked()
{
  setLineEditInput("tan");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonAsin_clicked()
{
  setLineEditInput("asin");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonAcos_clicked()
{
  setLineEditInput("acos");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonAtan_clicked()
{
  setLineEditInput("atan");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonLog_clicked()
{
  setLineEditInput("log");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonLn_clicked()
{
  setLineEditInput("ln");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonLog10_clicked()
{
  setLineEditInput("logten");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonFact_clicked()
{
  setLineEditInput("!");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonP_clicked()
{
  setLineEditInput("P");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonC_clicked()
{
  setLineEditInput("C");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonSinh_clicked()
{
  setLineEditInput("sinh");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonCosh_clicked()
{
  setLineEditInput("cosh");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonTanh_clicked()
{
  setLineEditInput("tanh");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonAmp_clicked()
{
  setLineEditInput("&");
  lineEditInput->setFocus();
}

void CalcUi::on_buttonPipe_clicked()
{
  setLineEditInput("|");
  lineEditInput->setFocus();
}

void CalcUi::on_outputTable_itemPressed(QTableWidgetItem *item)
{
  setLineEditInput(item->text());
  lineEditInput->setFocus();
}
