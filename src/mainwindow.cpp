#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "cpu_tester.h"

#include <thread>
#include <chrono>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setWindowTitle("Drainage");

    // Tooltips
    ui->timeText->setToolTip("Sets how long the test will last (maximum of two hours).");
    ui->loadText->setToolTip("Sets the amount of load that will be placed on the CPU during the test.");

    // Make sure load and thread selection is greyed out by default
    ui->loadSelect->setEnabled(false);
    ui->threadSelect->setEnabled(false);

    // Set threadsSpinBox maximum to the number of hardware threads
    unsigned int maxThreads = std::thread::hardware_concurrency();
    ui->threadSelect->setMaximum(maxThreads);

    // Connect Begin button click to the slot
    connect(ui->beginTest, &QPushButton::clicked, this, &MainWindow::onBeginButtonClicked);

    // Grey out load and thread selection when custom load isn't selected
    connect(ui->loadCustom, &QRadioButton::toggled, this, [=](bool checked) {
        ui->loadSelect->setEnabled(checked);
        ui->threadSelect->setEnabled(checked);
    });
}

void MainWindow::onBeginButtonClicked() {
    int loadPercentage = ui->loadSelect->value();
    int duration = ui->timeSelect->value();
    int numThreads = ui->threadSelect->value();

    if (ui->loadLow->isChecked()) {
        testCPUWithLoadAndThreads(25, std::chrono::seconds(duration), 1);
    }
    else if (ui->loadMedium->isChecked()) {
        testCPUWithLoadAndThreads(50, std::chrono::seconds(duration), std::thread::hardware_concurrency() / 2);
    }
    else if (ui->loadHigh->isChecked()) {
        testCPUWithLoadAndThreads(100, std::chrono::seconds(duration), std::thread::hardware_concurrency() - 2);
    }
    else if (ui->loadCustom->isChecked()) {
        testCPUWithLoadAndThreads(loadPercentage, std::chrono::seconds(duration), numThreads);
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
