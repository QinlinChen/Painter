#ifndef RESETCANVASDIALOG_H
#define RESETCANVASDIALOG_H

#include "ui_resetcanvasdialog.h"

#include <QDialog>

class ResetCanvasDialog : public QDialog, public Ui::ResetCanvasDialog
{
    Q_OBJECT

public:
    explicit ResetCanvasDialog(QWidget *parent = nullptr);

    QSize selectedCanvasSize() const;
    void setCurrentCanvasSize(const QSize &size);

private slots:
    void enableOkButton();
};

#endif // RESETCANVASDIALOG_H
