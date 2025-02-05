#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H
#include <QPushButton>

class CustomButton : public QPushButton {
    Q_OBJECT
public:
    explicit CustomButton(const QString& text, const QString& buttonType, QWidget* parent = nullptr);

    QString getButtonType() const; // 获取按钮类型
    void setButtonType(const QString& buttonType); // 设置按钮类型

private:
    QString buttonType; // 按钮类型
};

#endif // CUSTOMBUTTON_H
