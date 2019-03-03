#include "resetcanvasdialog.h"

#include <QtWidgets>

ResetCanvasDialog::ResetCanvasDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    widthLineEdit->setValidator(new QIntValidator(1, 4096, this));
    heightLineEdit->setValidator(new QIntValidator(1, 2160, this));

    connect(widthLineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableOkButton()));
    connect(heightLineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableOkButton()));
}

void ResetCanvasDialog::enableOkButton()
{
    bool enabled = widthLineEdit->hasAcceptableInput()
            && heightLineEdit->hasAcceptableInput();
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(enabled);
}

QSize ResetCanvasDialog::selectedCanvasSize() const
{
    int width = widthLineEdit->text().toInt();
    int height = heightLineEdit->text().toInt();
    return QSize(width, height);
}

void ResetCanvasDialog::setCurrentCanvasSize(const QSize &size)
{
    widthLineEdit->setText(QString::number(size.width()));
    heightLineEdit->setText(QString::number(size.height()));
}
