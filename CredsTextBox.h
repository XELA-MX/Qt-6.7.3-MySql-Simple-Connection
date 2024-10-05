#ifndef CREDSTEXTBOX_H
#define CREDSTEXTBOX_H

#include <QLineEdit>

class CredsTB : public QLineEdit
{
    Q_OBJECT

public:
    explicit CredsTB(QWidget *parent = nullptr);

protected:
    void enterEvent(QEnterEvent *e) override;
    void leaveEvent(QEvent *e) override;
};

#endif // CREDSTEXTBOX_H
