#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QFont fontText("Verdana", 14);
    QFont fontHeaders("Verdana", 12, QFont::Bold);
    QFont fontTable("Verdana", 12);
    QFont fontCode("Courier New", 10);

    this->setWindowTitle("Holsted Measures");
    this->setWindowIcon(QIcon("..\\..\\..\\images\\icon.ico"));
    this->setFixedSize(1540, 750);


    ui->buttonOpenFile->setFont(fontText);
    ui->buttonRefresh->setFont(fontText);
    ui->buttonSave->setFont(fontText);

    ui->textFile->setReadOnly(false);
    ui->textFile->setFont(fontCode);


    ui->tableOperators->setColumnCount(3);
    ui->tableOperators->setHorizontalHeaderLabels({"j", "Operator", "f_1j"});
    ui->tableOperators->horizontalHeader()->setStretchLastSection(false);
    ui->tableOperators->verticalHeader()->setVisible(false);
    ui->tableOperators->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    ui->tableOperators->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableOperators->setFont(fontTable);
    ui->tableOperators->horizontalHeader()->setFont(fontHeaders);


    connect(ui->buttonOpenFile, &QPushButton::clicked, this,
            &MainWindow::OpenFileSlot);
    connect(ui->buttonRefresh, &QPushButton::clicked, this,
            &MainWindow::RefreshSlot);
    connect(ui->buttonSave, &QPushButton::clicked, this,
            &MainWindow::SaveTablesSlot);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SetText() {
    if (_fileName.isEmpty()) {
        QMessageBox::warning(this, "File Problem", "No opened File!");
        return;
    }

    QFile file(_fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        ui->textFile->setPlainText(in.readAll());
        file.close();
    } else {
        QMessageBox::warning(this, "File Problem", "Can't Open File!");
    }
}

void MainWindow::SetHolsted() {
    QMap<QString, int> operatorsCount;

    this->SetOperatorMap(&operatorsCount);

    this->SetTableOperators(operatorsCount);
}

void MainWindow::SetTableOperators(const QMap<QString, int>& operators) {
    ui->tableOperators->clear();
    ui->tableOperators->setRowCount(operators.size() + 1);
    ui->tableOperators->setColumnCount(3);

    QStringList headers = {"j", "Operator", "f_1j"};
    ui->tableOperators->setHorizontalHeaderLabels(headers);
    ui->tableOperators->horizontalHeader()->setStretchLastSection(true);

    QTableWidgetItem* temp;
    int row = 0;
    int count = 0;
    foreach (const QString& op, operators.keys()) {
        temp = new QTableWidgetItem(QString::number(row + 1));
        temp->setTextAlignment(Qt::AlignCenter);
        ui->tableOperators->setItem(row, 0, temp);

        temp = new QTableWidgetItem(op);
        temp->setTextAlignment(Qt::AlignCenter);
        ui->tableOperators->setItem(row, 1, temp);

        temp = new QTableWidgetItem(QString::number(operators.value(op)));
        temp->setTextAlignment(Qt::AlignCenter);
        ui->tableOperators->setItem(row, 2, temp);


        if (op != "case") {
            count += operators.value(op);
        }
        ++row;
    }

    ui->tableOperators->setItem(
        row, 0, new QTableWidgetItem("n_1 = " + (QString::number(row))));
    _n_1 = row;
    ui->tableOperators->setItem(
        row, 2, new QTableWidgetItem("N_1 = " + QString::number(count)));
    _N_1 = count;

    ui->tableOperators->resizeColumnsToContents();
    ui->tableOperators->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
}


void MainWindow::SaveTable(QTableWidget* table, const QString& filePath) {


    if (!table) {
        QMessageBox::warning(this, "Save Error", "Table is Empty!");
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Save Error", "Can't open file!");
        return;
    }

    QTextStream out(&file);

    QVector<int> colWidths(table->columnCount(), 0);
    for (int col = 0; col < table->columnCount(); ++col) {
        int maxLen = table->horizontalHeaderItem(col)->text().length();
        for (int row = 0; row < table->rowCount(); ++row) {
            QTableWidgetItem* item = table->item(row, col);
            if (item)
                maxLen = qMax(maxLen, item->text().length());
        }
        colWidths[col] = maxLen + 2;
    }

    auto writeLine = [&]() {
        out << "+";


        for (int w : colWidths) {
            out << QString(w, '-') << "+";
        }
        out << "\n";
    };

    auto writeRow = [&](const QStringList& rowData) {
        out << "|";
        for (int i = 0; i < rowData.size(); ++i) {
            QString s = " " +
                        rowData[i]
                            .left(colWidths[i] - 2)
                            .leftJustified(colWidths[i] - 2) +
                        " ";
            out << s << "|";
        }
        out << "\n";
    };

    writeLine();

    QStringList headers;
    for (int col = 0; col < table->columnCount(); ++col)
        headers << table->horizontalHeaderItem(col)->text();
    writeRow(headers);

    writeLine();

    for (int row = 0; row < table->rowCount(); ++row) {
        QStringList rowData;
        for (int col = 0; col < table->columnCount(); ++col) {
            QTableWidgetItem* item = table->item(row, col);
            rowData << (item ? item->text() : "");
        }
        writeRow(rowData);
    }

    writeLine();

    file.close();

    QMessageBox::information(this, "Saved", "Table Saved To File:");
}

void MainWindow::OpenFileSlot() {
    QString initDir = "..\\..\\..\\analyz";
    QStringList filters;
    filters << "Scala (*.scala *.sc)"
            << "All files (*.*)";

    QString fileName = QFileDialog::getOpenFileName(
        this, "Choose File", initDir, filters.join(";;"));

    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "File Problem", "No such file!");
        return;
    }

    _fileName = fileName;

    this->SetText();
    this->SetHolsted();
}

void MainWindow::RefreshSlot() {

    if (_fileName.isEmpty()) {
        QMessageBox::warning(this, "File Problem", "No opened File!");
        return;
    }


    std::ofstream myFile(_fileName.toStdString());
    if (myFile.is_open()) {
        myFile << ui->textFile->toPlainText().toStdString();
        myFile.close();
    }


    this->SetHolsted();
}

void MainWindow::SaveTablesSlot() {
    this->SaveTable(ui->tableOperators, "..\\..\\..\\res\\operators.txt");
    // this->SaveTable(ui->tableOperands, "..\\..\\..\\res\\operands.txt");
}

void MainWindow::SetOperatorMap(QMap<QString, int>* operatorsCount) {
    std::string code = proceedFile(this->_fileName.toStdString());
    auto oper = get_operators(code);


    for (auto [op, count] : oper) {
        operatorsCount->insert(QString::fromStdString(op), count);
    }


    auto num = count_branches(oper);
    this->SetAbsolute(QString::number(num));


    auto maxDeph = count_max_nesting_depth(code);
    this->SetMaximum(QString::number(maxDeph));


    int result = count_operators(oper);

    auto relative = static_cast<double>(num) / static_cast<double>(result);
    this->SetRelative(QString::number(relative));
}

void MainWindow::SetAbsolute(QString num) {
    ui->absolute->setFontWeight(80);
    ui->absolute->setText("Абсолютная :" + num);
}


void MainWindow::SetRelative(QString num) {
    ui->relative->setText("Относительная :" + num);
};
void MainWindow::SetMaximum(QString num) {
    ui->maximum->setFontWeight(80);
    ui->maximum->setText("Максимальнй уровень вложенности :" + num);
};
