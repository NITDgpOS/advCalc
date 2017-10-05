#ifndef CALC_UI_H
#define CALC_UI_H

#include <QDialog>

#include "ui_calc.h"

class CalcUi : public QDialog, public Ui::CalcUi
{
  Q_OBJECT

private:
  // Don't use lineEditInput->setText to insert text
  // Use this function instead
  void setLineEditInput(QString s)
  {
    int t = lineEditInput->cursorPosition();
    lineEditInput->
      setText(lineEditInput->text().left(t) + s +
              lineEditInput->text().right(lineEditInput->text().length() - t));
    lineEditInput->setCursorPosition(t + s.length());
  }

public:
  explicit CalcUi(QWidget *parent = 0);

private slots:
  void on_lineEditInput_textChanged(QString &);
  void on_lineEditConstantName_textChanged();
  void on_lineEditConstantValue_textChanged();
  void on_keyCheckBox_toggled(bool);
  void on_buttonOtherFunctions_toggled(bool);
  void on_buttonDelete_clicked();
  void on_comboBox_activated(const QString &arg1);
  void on_button1_clicked();
  void on_button2_clicked();
  void on_button3_clicked();
  void on_button4_clicked();
  void on_button5_clicked();
  void on_button6_clicked();
  void on_button7_clicked();
  void on_button8_clicked();
  void on_button9_clicked();
  void on_button0_clicked();
  void on_buttonPlus_clicked();
  void on_buttonMinus_clicked();
  void on_buttonMultiply_clicked();
  void on_buttonDivide_clicked();
  void on_buttonPow_clicked();
  void on_buttonDot_clicked();
  void on_buttonOpenBracket_clicked();
  void on_buttonCloseBracket_clicked();
  void on_buttonAmp_clicked();
  void on_buttonPipe_clicked();
  void on_buttonSpace_clicked();
  void on_buttonSin_clicked();
  void on_buttonCos_clicked();
  void on_buttonTan_clicked();
  void on_buttonAsin_clicked();
  void on_buttonAcos_clicked();
  void on_buttonAtan_clicked();
  void on_buttonLog_clicked();
  void on_buttonLn_clicked();
  void on_buttonLog10_clicked();
  void on_buttonFact_clicked();
  void on_buttonP_clicked();
  void on_buttonC_clicked();
  void on_buttonSinh_clicked();
  void on_buttonCosh_clicked();
  void on_buttonTanh_clicked();
  void on_buttonCalculate_clicked();
  void on_outputTable_itemPressed(QTableWidgetItem *);
};

#endif
