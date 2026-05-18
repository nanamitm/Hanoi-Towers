#include "mainwindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QStyle>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWidgetAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    buildUi();
    connectSignals();
    m_model.reset(10);
    m_view->setModel(&m_model);
    updateUi();
    setWindowIcon(QIcon(":/icons/app.ico"));
    resize(760, minimumSizeHint().height());
}

void MainWindow::buildUi()
{
    auto *central = new QWidget(this);
    auto *root = new QVBoxLayout(central);
    root->setContentsMargins(14, 12, 14, 12);
    root->setSpacing(10);

    auto *settings = new QGridLayout();
    settings->setHorizontalSpacing(12);
    settings->setVerticalSpacing(8);

    m_diskSpin = new QSpinBox(central);
    m_diskSpin->setRange(1, 16);
    m_diskSpin->setValue(10);
    m_diskSpin->setSuffix(" disks");

    m_speedSlider = new QSlider(Qt::Horizontal, central);
    m_speedSlider->setRange(50, 5000);
    m_speedSlider->setSingleStep(50);
    m_speedSlider->setPageStep(250);
    m_speedSlider->setValue(1000);

    m_speedLabel = new QLabel(central);
    m_speedLabel->setMinimumWidth(88);

    m_loopCheck = new QCheckBox("Loop", central);
    m_loopCheck->setChecked(true);

    m_topMostCheck = new QCheckBox("Always on top", central);

    m_themeCombo = new QComboBox(central);
    m_themeCombo->addItems({"System", "Light", "Dark"});

    auto *settingsMenu = new QMenu(this);
    auto *settingsPanel = new QWidget(settingsMenu);
    auto *settingsPanelLayout = new QGridLayout(settingsPanel);
    settingsPanelLayout->setContentsMargins(12, 10, 12, 10);
    settingsPanelLayout->setHorizontalSpacing(10);
    settingsPanelLayout->setVerticalSpacing(8);
    settingsPanelLayout->addWidget(new QLabel("Theme", settingsPanel), 0, 0);
    settingsPanelLayout->addWidget(m_themeCombo, 0, 1);
    settingsPanelLayout->addWidget(m_loopCheck, 1, 0, 1, 2);
    settingsPanelLayout->addWidget(m_topMostCheck, 2, 0, 1, 2);

    auto *settingsAction = new QWidgetAction(settingsMenu);
    settingsAction->setDefaultWidget(settingsPanel);
    settingsMenu->addAction(settingsAction);

    m_settingsButton = new QToolButton(central);
    m_settingsButton->setText(QString(QChar(0x2699)));
    m_settingsButton->setToolTip("Settings");
    m_settingsButton->setPopupMode(QToolButton::InstantPopup);
    m_settingsButton->setMenu(settingsMenu);
    m_settingsButton->setFixedSize(34, 30);

    settings->addWidget(new QLabel("Disks", central), 0, 0);
    settings->addWidget(m_diskSpin, 0, 1);
    settings->addWidget(new QLabel("Speed", central), 0, 2);
    settings->addWidget(m_speedSlider, 0, 3);
    settings->addWidget(m_speedLabel, 0, 4);
    settings->addWidget(m_settingsButton, 0, 5);
    settings->setColumnStretch(3, 1);
    root->addLayout(settings);

    m_view = new HanoiView(central);
    root->addWidget(m_view, 1);

    auto *controls = new QHBoxLayout();
    controls->setSpacing(8);

    m_firstButton = new QPushButton(style()->standardIcon(QStyle::SP_MediaSkipBackward), "", central);
    m_previousButton = new QPushButton(style()->standardIcon(QStyle::SP_MediaSeekBackward), "", central);
    m_playButton = new QPushButton(style()->standardIcon(QStyle::SP_MediaPlay), "", central);
    m_nextButton = new QPushButton(style()->standardIcon(QStyle::SP_MediaSeekForward), "", central);
    m_lastButton = new QPushButton(style()->standardIcon(QStyle::SP_MediaSkipForward), "", central);

    const QList<QPushButton *> buttons = {m_firstButton, m_previousButton, m_playButton, m_nextButton, m_lastButton};
    for (auto *button : buttons) {
        button->setFixedSize(42, 34);
        controls->addWidget(button);
    }

    controls->addStretch(1);
    m_stepLabel = new QLabel(central);
    m_durationLabel = new QLabel(central);
    m_stepLabel->setMinimumWidth(130);
    m_durationLabel->setMinimumWidth(170);
    controls->addWidget(m_stepLabel);
    controls->addWidget(m_durationLabel);
    root->addLayout(controls);

    setCentralWidget(central);
    setWindowTitle("Hanoi Towers Slideshow");
}

void MainWindow::connectSignals()
{
    connect(m_diskSpin, &QSpinBox::valueChanged, this, &MainWindow::regenerate);
    connect(m_speedSlider, &QSlider::valueChanged, this, [this](int value) {
        m_timer.setInterval(value);
        updateUi();
    });
    connect(m_themeCombo, &QComboBox::currentIndexChanged, this, [this](int index) {
        const auto theme = index == 1 ? HanoiTheme::Light : (index == 2 ? HanoiTheme::Dark : HanoiTheme::System);
        m_view->setTheme(theme);
    });
    connect(m_firstButton, &QPushButton::clicked, this, [this] {
        m_model.goToStep(0);
        setPlaying(false);
        updateUi();
    });
    connect(m_previousButton, &QPushButton::clicked, this, [this] {
        m_model.previous();
        updateUi();
    });
    connect(m_playButton, &QPushButton::clicked, this, [this] {
        setPlaying(!m_timer.isActive());
    });
    connect(m_nextButton, &QPushButton::clicked, this, [this] {
        if (!m_model.next() && m_loopCheck->isChecked()) {
            m_model.goToStep(0);
        }
        updateUi();
    });
    connect(m_lastButton, &QPushButton::clicked, this, [this] {
        m_model.goToStep(m_model.totalSteps());
        setPlaying(false);
        updateUi();
    });
    connect(&m_timer, &QTimer::timeout, this, [this] {
        if (!m_model.next()) {
            if (m_loopCheck->isChecked()) {
                m_model.goToStep(0);
            } else {
                setPlaying(false);
            }
        }
        updateUi();
    });
    connect(m_topMostCheck, &QCheckBox::toggled, this, [this](bool checked) {
        const Qt::WindowFlags baseFlags = windowFlags() & ~Qt::WindowStaysOnTopHint;
        setWindowFlags(checked ? baseFlags | Qt::WindowStaysOnTopHint : baseFlags);
        show();
    });
}

void MainWindow::regenerate()
{
    const bool wasPlaying = m_timer.isActive();
    setPlaying(false);
    m_model.reset(m_diskSpin->value());
    if (wasPlaying) {
        setPlaying(true);
    }
    updateUi();
}

void MainWindow::updateUi()
{
    const int interval = m_speedSlider->value();
    m_speedLabel->setText(QString::number(interval / 1000.0, 'f', 2) + " s/move");

    m_stepLabel->setText(QString("%1 / %2").arg(m_model.currentStep()).arg(m_model.totalSteps()));

    const qint64 remainingMoves = m_model.totalSteps() - m_model.currentStep();
    const qint64 remainingSeconds = remainingMoves * interval / 1000;
    m_durationLabel->setText("Remaining " + formatDuration(remainingSeconds));
    m_durationLabel->setVisible(m_timer.isActive());

    m_firstButton->setEnabled(m_model.currentStep() > 0);
    m_previousButton->setEnabled(m_model.currentStep() > 0);
    m_nextButton->setEnabled(m_model.currentStep() < m_model.totalSteps() || m_loopCheck->isChecked());
    m_lastButton->setEnabled(m_model.currentStep() < m_model.totalSteps());
    m_playButton->setIcon(style()->standardIcon(m_timer.isActive() ? QStyle::SP_MediaPause : QStyle::SP_MediaPlay));

    m_view->update();
}

void MainWindow::setPlaying(bool playing)
{
    if (playing) {
        if (m_model.currentStep() >= m_model.totalSteps()) {
            m_model.goToStep(0);
        }
        m_timer.start(m_speedSlider->value());
    } else {
        m_timer.stop();
    }
    updateUi();
}

QString MainWindow::formatDuration(qint64 seconds) const
{
    const qint64 hours = seconds / 3600;
    seconds %= 3600;
    const qint64 minutes = seconds / 60;
    seconds %= 60;

    if (hours > 0) {
        return QString("%1h %2m %3s").arg(hours).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
    }
    if (minutes > 0) {
        return QString("%1m %2s").arg(minutes).arg(seconds, 2, 10, QChar('0'));
    }
    return QString("%1s").arg(seconds);
}
