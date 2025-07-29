#ifndef QUESTIONUI_H
#define QUESTIONUI_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class QuestionUI : public QWidget
{
    Q_OBJECT

public:
    explicit QuestionUI(QWidget *parent = nullptr);
    ~QuestionUI();

signals:
    void adminSelected();
    void userSelected();

private:
    QLabel *titleLabel;
    QPushButton *adminButton;
    QPushButton *userButton;
};

#endif // QUESTIONUI_H
