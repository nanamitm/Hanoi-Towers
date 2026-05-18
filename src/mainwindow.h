#pragma once

#include "hanoimodel.h"
#include "hanoiview.h"

#include <QMainWindow>
#include <QTimer>

class QCheckBox;
class QComboBox;
class QLabel;
class QToolButton;
class QPushButton;
class QSlider;
class QSpinBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void buildUi();
    void connectSignals();
    void regenerate();
    void updateUi();
    void setPlaying(bool playing);
    QString formatDuration(qint64 seconds) const;

    HanoiModel m_model;
    QTimer m_timer;
    HanoiView *m_view = nullptr;
    QSpinBox *m_diskSpin = nullptr;
    QSlider *m_speedSlider = nullptr;
    QLabel *m_speedLabel = nullptr;
    QLabel *m_stepLabel = nullptr;
    QLabel *m_durationLabel = nullptr;
    QComboBox *m_themeCombo = nullptr;
    QToolButton *m_settingsButton = nullptr;
    QPushButton *m_firstButton = nullptr;
    QPushButton *m_previousButton = nullptr;
    QPushButton *m_playButton = nullptr;
    QPushButton *m_nextButton = nullptr;
    QPushButton *m_lastButton = nullptr;
    QCheckBox *m_loopCheck = nullptr;
    QCheckBox *m_topMostCheck = nullptr;
};
