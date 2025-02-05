#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <QRegExpValidator>
#include <QRegExp>
#include <QDoubleValidator>
#include <QValidator>
#include <QMessageBox>
#include <QStack>
#include <QStringList>
#include <QVariant>
#include <QString>
#include <cmath>
#include <limits>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->Button0, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->Button1, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->Button2, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->Button3, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->Button4, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->Button5, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->Button6, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->Button7, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->Button8, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->Button9, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    // 连接运算符按钮
    connect(ui->ButtonAdd, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->ButtonMinus, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->ButtonMultiple, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->ButtonDivide, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    // 连接其他按钮
    connect(ui->ButtonClear, &QPushButton::clicked, this, &MainWindow::onButtonClearPressed);
    connect(ui->ButtonEqual, &QPushButton::clicked, this, &MainWindow::onButtonEqualsPressed);
    connect(ui->ButtonBackSpace, &QPushButton::clicked, this, &MainWindow::onButtonBackspacePressed);
    connect(ui->ButtonClearAll, &QPushButton::clicked, this, &MainWindow::onButtonClearAllPressed);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButtonPressed()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QString text = ui->resultDisplay->text();
        text += button->text();
        ui->resultDisplay->setText(text);
    }
}

void MainWindow::onButtonClearPressed()
{
    ui->resultDisplay->clear();
}
double calculate(const QVector<double>& numbers, const QVector<char>& operators) {
    double result = numbers[0];
    for (size_t i = 1; i < numbers.size(); ++i) {
        switch (operators[i - 1]) {
            case '+':
                result += numbers[i];
                break;
            case '-':
                result -= numbers[i];
                break;
            case '*':
                result *= numbers[i];
                break;
            case '/':
                if (numbers[i] == 0) {
                    return -1; // Indicate error
                }
                result /= numbers[i];
                break;
            default:
                return -1; // Indicate error
        }
    }
    return result;
}

void MainWindow::onButtonEqualsPressed()
{
    QString input = ui->resultDisplay->text(); // Get input from line edit
    QVector<double> numbers;//-----------------------------------------------------------创建动态数组number。Qvector是动态数组，double是类型，
    QVector<char> operators;
    QString currentNumberStr; //---------------------------------------------------------作为暂存的数字字符串

    for (int i = 0; i < input.length(); ++i) {
        QChar ch = input[i];
        if (ch.isDigit()) { //-----------------------------------------------------------如果是数字的话
            currentNumberStr += ch; //---------------------------------------------------直接把现在的字符串加到数字字符串后面
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            if (!currentNumberStr.isEmpty()) {
                numbers.append(currentNumberStr.toDouble()); //--------------------------如果碰到了运算符，并且当前暂存数组不为空，那么先进行强制类型转换，并且将他添加到number容器的末尾
                currentNumberStr.clear(); //---------------------------------------------清空当前暂存数组为下一个数字做准备
            }
            operators.append(ch.toLatin1()); //------------------------------------------把运算符强制类型转换为Latin并加到运算符数组最后
        } else {
            QMessageBox::critical(this, "Error", "Invalid character in expression!");//--无效输入弹窗
            return;
        }
    }
    if (!currentNumberStr.isEmpty()) {
        numbers.append(currentNumberStr.toDouble());//-----------------------------------把最后的数字加入到数组里
    }

    double result = calculate(numbers, operators);
    if (result != -1) {
        ui->resultDisplay->setText(QString::number(result)); // Display the result
    } else {
        QMessageBox::critical(this, "Error", "Division by zero or unknown operator!");
    }
}


void MainWindow::onButtonBackspacePressed()
{
    QString text = ui->resultDisplay->text();
    if (!text.isEmpty())
    {
        text.chop(1);  // 删除最后一个字符
        ui->resultDisplay->setText(text);
    }
}

void MainWindow::onButtonClearAllPressed()
{
    ui->resultDisplay->clear();
}



