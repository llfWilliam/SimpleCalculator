#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <QDoubleValidator>
#include <QValidator>
#include <QMessageBox>
#include <QStack>
#include <QStringList>
#include <QVariant>
#include <QString>
#include <cmath>
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
    connect(ui->Button10, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->ButtonAdd, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->ButtonMinus, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->ButtonMultiple, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->ButtonDivide, &QPushButton::clicked, this, &MainWindow::onButtonPressed);
    connect(ui->ButtonClear, &QPushButton::clicked, this, &MainWindow::onButtonClearPressed);
    connect(ui->ButtonEqual, &QPushButton::clicked, this, &MainWindow::onButtonEqualsPressed);
    connect(ui->ButtonBackSpace, &QPushButton::clicked, this, &MainWindow::onButtonBackspacePressed);
    connect(ui->ButtonClearAll, &QPushButton::clicked, this, &MainWindow::onButtonClearAllPressed);
//----------------------------------------------------------------------------------------将ui下的按扭的信号和mainwindow的函数进行链接
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
    QVector<double> finalNumbers = numbers;      //---------------------------------------最终的数字数组
    QVector<char> finalOperators = operators;    //---------------------------------------最终的运算符数组
                                                 //---------------------------------------处理乘除运算：先处理乘除，再处理加减
    for (int i = 0; i < finalOperators.size(); ++i) {
        if (finalOperators[i] == '*' || finalOperators[i] == '/') {
                                                 //---------------------------------------对于乘除操作，执行运算并将结果替换
            if (finalOperators[i] == '*') {
                finalNumbers[i] = finalNumbers[i] * finalNumbers[i + 1];//----------------将乘号左右的数字都算出结果并替换左边的数字
            } else if (finalOperators[i] == '/') {
                if (finalNumbers[i + 1] == 0) {
                    return -10086;  //----------------------------------------------------除零错误
                }
                finalNumbers[i] = finalNumbers[i] / finalNumbers[i + 1];
            }
            finalNumbers.remove(i + 1);//-------------------------------------------------删除当前乘除符号后面的数字，因为它已经被计算了
            finalOperators.remove(i);//---------------------------------------------------删除符号
            i--;  //----------------------------------------------------------------------重新检查当前位置的运算符
        }
    }

    // 处理加减运算
    double result = finalNumbers[0];
    for (int i = 1; i < finalNumbers.size(); ++i) {
        switch (finalOperators[i - 1]) {
            case '+':
                result += finalNumbers[i];
                break;
            case '-':
                result -= finalNumbers[i];
                break;
            default:
                return -10086;  // 无效操作
        }
    }

    return result;
}

void MainWindow::onButtonEqualsPressed()
{
    QString input = ui->resultDisplay->text(); // 获取输入
    QVector<double> numbers;                    // 存储数字
    QVector<char> operators;                    // 存储运算符
    QString currentNumberStr;                   // 存储暂存数字

    for (int i = 0; i < input.length(); ++i) {
        QChar ch = input[i];
        // 如果是数字或小数点，直接拼接到当前数字字符串中
        if (ch.isDigit() || ch == '.') {
            currentNumberStr += ch;
        }
        // 如果是运算符
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            // 检测连续运算符：如果当前运算符后面还有字符且下一个字符也是运算符，则弹出警告
            if (i + 1 < input.length()) {
                QChar nextChar = input[i + 1];
                if (nextChar == '+' || nextChar == '-' || nextChar == '*' || nextChar == '/') {
                    QMessageBox::critical(this, "警告", "连续运算符不合法！");
                    return;
                }
            }
            // 如果当前数字字符串不为空，则先将数字转换为 double 添加到 numbers 中
            if (!currentNumberStr.isEmpty()) {
                numbers.append(currentNumberStr.toDouble());
                currentNumberStr.clear();
            }
            // 添加运算符到 operators 数组
            operators.append(ch.toLatin1());
        }
        // 非法字符，直接弹出警告
        else {
            QMessageBox::critical(this, "警告", "你写了一堆什么东西");
            return;
        }
    }

    // 如果最后还有剩余的数字字符串，转换并添加到 numbers 中
    if (!currentNumberStr.isEmpty()) {
        numbers.append(currentNumberStr.toDouble());
    }

    // 调用 calculate 计算结果
    // **修复崩溃问题**，防止 `numbers.size() == 1 && operators.isEmpty()` 进入计算
    if (numbers.size() == 1 && operators.isEmpty()) {
        return;  // 避免重复计算已得出的结果
    }
    double result = calculate(numbers, operators);
    if (result != -10086) {
        ui->resultDisplay->setText(QString::number(result)); // 显示结果
    } else {
        QMessageBox::critical(this, "警告", "除数为零或非法输入");
    }
}

void MainWindow::onButtonBackspacePressed()
{
    QString text = ui->resultDisplay->text();
    if (!text.isEmpty())
    {
        text.chop(1);//------------------------------------------------------------------删除一个字符
        ui->resultDisplay->setText(text);//----------------------------------------------把删掉以后的文本放上去
    }
}

void MainWindow::onButtonClearAllPressed()
{
    ui->resultDisplay->clear();//--------------------------------------------------------清空文本
}
