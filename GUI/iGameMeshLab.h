#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_iGameMeshLab.h"

class iGameMeshLab : public QMainWindow
{
    Q_OBJECT


public:
    iGameMeshLab(QWidget *parent = nullptr);
    ~iGameMeshLab();

private:
    Ui::iGameMeshLabClass ui;
};
