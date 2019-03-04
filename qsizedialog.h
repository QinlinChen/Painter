#ifndef RESETCANVASDIALOG_H
#define RESETCANVASDIALOG_H

#include "ui_qsizedialog.h"

#include <QDialog>

class QSizeDialog : public QDialog, public Ui::QSizeDialog
{
    Q_OBJECT

public:
    explicit QSizeDialog(const QSize &limit, QWidget *parent = nullptr);

    QSize selectedSize() const;
    void setCurrentSize(const QSize &size);

    static QSize getSize(const QSize &limit,
                         const QSize &initSize = QSize(0, 0),
                         QWidget *parent = nullptr,
                         const QString &title = QString());
private slots:
    void enableOkButton();

private:
    static QSize normalizeSize(const QSize &size);
};

#endif // RESETCANVASDIALOG_H
