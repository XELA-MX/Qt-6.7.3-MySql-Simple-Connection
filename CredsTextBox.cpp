#include "CredsTextBox.h"

CredsTB::CredsTB(QWidget *parent) : QLineEdit(parent)
{
    // Implementación del constructor
}


void CredsTB::enterEvent(QEnterEvent *e)
{
    QLineEdit::enterEvent(e);
    setStyleSheet("background-color: lightblue");
}

void CredsTB::leaveEvent(QEvent *e)
{
    QLineEdit::leaveEvent(e);

    setStyleSheet("");
}
