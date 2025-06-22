#include <QApplication>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <string>
#include <strings.h>

// Placeholders
const int NUM_BOATS = 10;
const int NUM_RACES = 5;
const int NUM_RACES_TO_DROP = 5;
const int NUM_TOTAL_COLS = 1;
const int NUM_BOAT_INFO_COLS = 1;

const int ROW_SIZE = NUM_BOATS;
const int COL_SIZE = NUM_RACES + NUM_TOTAL_COLS + NUM_BOAT_INFO_COLS;

QStringList scoringAbbreviations = {"DNC", "DNS", "OCS", "ZFP", "UFD", "BFD",
                                    "SCP", "NSC", "DNF", "NSC", "DNF", "RET",
                                    "DSQ", "DNE", "DGM", "RDG", "DPI"};

// TODO: Add ability to specify multiple drops after race targets
int number_of_drops(int num_races_to_drop, int num_races) {
  return (int)(num_races >= num_races_to_drop);
}

// TODO: Implement
int scoringAbbreviationToScore(QTableWidget *tableWidget,
                               QString abbreviation) {
  return 1;
}

void updateTableTotals(QTableWidget *tableWidget) {
  for (int row = 0; row < ROW_SIZE; ++row) {
    // one less because last col is totals, TODO: make sure to change if layout
    // changes
    int rowSum = 0;
    for (int column = NUM_BOAT_INFO_COLS; column < COL_SIZE - 1; ++column) {
      auto item = tableWidget->item(row, column);
      if (scoringAbbreviations.contains(item->text()))
        rowSum += scoringAbbreviationToScore(tableWidget, item->text());
      else if (item->text() == "")
        rowSum += 0;
      else
        rowSum += item->text().toInt();
    }
    std::string s = std::to_string(rowSum);
    char const *pchar = s.c_str();
    tableWidget->item(row, COL_SIZE - 1)->setText(pchar);
  }
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Create a QTableWidget
  QTableWidget tableWidget;
  tableWidget.setRowCount(ROW_SIZE);    // 1 row per boat plus info  TODO: add
                                        // number of rows needed for info
  tableWidget.setColumnCount(COL_SIZE); // 1 column per race plus info  TODO:
                                        // add number of columns needed for info
  QStringList headers;
  // add boat number header
  headers << QString("Boat Number");
  // add race headers
  for (int i = 1; i < NUM_RACES + 1; i++) {
    headers << QString("Race %1").arg(i);
  }
  // add totals column
  headers << QString("Total");
  tableWidget.setHorizontalHeaderLabels(headers);

  // Make sure Totals data is not editable
  // also make sure boat data input is separately edited (not edited here)
  for (int row = 0; row < ROW_SIZE; ++row) {
    for (int column = 0; column < COL_SIZE; ++column) {
      QTableWidgetItem *item = new QTableWidgetItem();
      if (column == COL_SIZE - 1 || column < NUM_BOAT_INFO_COLS)
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
      tableWidget.setItem(row, column, item);
    }
  }

  // make it so that only proper text input may occur
  QObject::connect(&tableWidget, &QTableWidget::itemChanged,
                   [&tableWidget](QTableWidgetItem *item) {
                     QString text = item->text();
                     bool ok = true;
                     item->setText(item->text().toUpper());
                     text.toInt(&ok);
                     if (!ok && !scoringAbbreviations.contains(item->text())) {

                       item->setText("");
                     }
                     // update totals &  TODO: Update the results ordering
                     updateTableTotals(&tableWidget);
                   });

  // Show the table widget
  tableWidget.show();

  return app.exec();
}
