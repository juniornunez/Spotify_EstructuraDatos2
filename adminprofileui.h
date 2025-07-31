#ifndef ADMINPROFILEUI_H
#define ADMINPROFILEUI_H

#include <QWidget>
#include <QString>

class AdminProfileUI : public QWidget
{
    Q_OBJECT
public:
    explicit AdminProfileUI(const QString &profilePicPath, const QString &artistName, const QString &realName, QWidget *parent = nullptr);
    ~AdminProfileUI();

signals:
    void logoutClicked();
};

#endif // ADMINPROFILEUI_H
