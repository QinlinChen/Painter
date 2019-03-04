#include "qsizedialog.h"

#include <QtWidgets>

QSizeDialog::QSizeDialog(const QSize &limit, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    QSize normLimit = normalizeSize(limit);
    widthLineEdit->setValidator(
                new QIntValidator(0, normLimit.width(), this));
    heightLineEdit->setValidator(
                new QIntValidator(0, normLimit.height(), this));

    connect(widthLineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableOkButton()));
    connect(heightLineEdit, SIGNAL(textChanged(const QString &)),
            this, SLOT(enableOkButton()));
}

void QSizeDialog::enableOkButton()
{
    bool enabled = widthLineEdit->hasAcceptableInput()
            && heightLineEdit->hasAcceptableInput();
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(enabled);
}

QSize QSizeDialog::normalizeSize(const QSize &size)
{
    return QSize(0, 0).expandedTo(size);
}

QSize QSizeDialog::selectedSize() const
{
    int width = widthLineEdit->text().toInt();
    int height = heightLineEdit->text().toInt();
    return QSize(width, height);
}

void QSizeDialog::setCurrentSize(const QSize &size)
{
    QSize normSize = normalizeSize(size);
    widthLineEdit->setText(QString::number(normSize.width()));
    heightLineEdit->setText(QString::number(normSize.height()));
}

QSize QSizeDialog::getSize(const QSize &limit, const QSize &initSize,
                           QWidget *parent, const QString &title)
{
    QSizeDialog dialog(limit, parent);
    dialog.setCurrentSize(initSize);
    dialog.setWindowTitle(title);

    if (dialog.exec()) {
        return dialog.selectedSize();
    }
    return QSize(0, 0);
}
