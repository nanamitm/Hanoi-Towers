#pragma once

#include "hanoimodel.h"

#include <QWidget>

enum class HanoiTheme {
    System,
    Light,
    Dark
};

class HanoiView : public QWidget
{
    Q_OBJECT

public:
    explicit HanoiView(QWidget *parent = nullptr);

    void setModel(const HanoiModel *model);
    void setTheme(HanoiTheme theme);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize minimumSizeHint() const override;

private:
    void drawScene(QPainter &painter, const QRectF &sceneRect) const;
    QColor diskColor(int disk) const;
    bool usesDarkTheme() const;
    void drawMoveArrow(QPainter &painter, const QRectF &area, const Move &move, qreal towerWidth) const;

    const HanoiModel *m_model = nullptr;
    HanoiTheme m_theme = HanoiTheme::System;
};
