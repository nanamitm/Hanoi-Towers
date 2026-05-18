#include "hanoiview.h"

#include <QPainter>
#include <QPainterPath>
#include <QtMath>

HanoiView::HanoiView(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(520, 240);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void HanoiView::setModel(const HanoiModel *model)
{
    m_model = model;
    update();
}

void HanoiView::setTheme(HanoiTheme theme)
{
    m_theme = theme;
    update();
}

void HanoiView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const bool dark = usesDarkTheme();
    QLinearGradient outerBackground(rect().topLeft(), rect().bottomRight());
    outerBackground.setColorAt(0.0, dark ? QColor(23, 28, 34) : QColor(246, 248, 250));
    outerBackground.setColorAt(1.0, dark ? QColor(34, 43, 52) : QColor(229, 235, 239));
    painter.fillRect(rect(), outerBackground);

    constexpr qreal virtualWidth = 760.0;
    constexpr qreal virtualHeight = 420.0;
    const qreal scale = std::min(width() / virtualWidth, height() / virtualHeight);
    const qreal scaledWidth = virtualWidth * scale;
    const qreal scaledHeight = virtualHeight * scale;

    painter.translate((width() - scaledWidth) / 2.0, (height() - scaledHeight) / 2.0);
    painter.scale(scale, scale);

    drawScene(painter, QRectF(0.0, 0.0, virtualWidth, virtualHeight));
}

void HanoiView::drawScene(QPainter &painter, const QRectF &sceneRect) const
{
    const bool dark = usesDarkTheme();
    const QRectF area = sceneRect.adjusted(18, 18, -18, -18);

    if (!m_model) {
        return;
    }

    const int diskCount = m_model->diskCount();
    const Move activeMove = m_model->currentMove();
    const qreal baseY = area.bottom() - 28.0;
    const qreal pegHeight = area.height() * 0.66;
    const qreal pegTop = baseY - pegHeight;
    const qreal pegWidth = 8.0;
    const qreal diskHeight = std::max<qreal>(10.0, std::min<qreal>(22.0, pegHeight / (diskCount + 2)));
    const qreal towerWidth = area.width() / 3.0;
    const qreal maxDiskWidth = towerWidth * 0.78;
    const qreal minDiskWidth = towerWidth * 0.24;

    if (activeMove.disk > 0) {
        drawMoveArrow(painter, area, activeMove, towerWidth);
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(dark ? QColor(120, 132, 143) : QColor(88, 98, 105));
    painter.drawRoundedRect(QRectF(area.left() + 16.0, baseY, area.width() - 32.0, 12.0), 5.0, 5.0);

    const auto &towers = m_model->towers();
    for (int towerIndex = 0; towerIndex < 3; ++towerIndex) {
        const qreal centerX = area.left() + towerWidth * (towerIndex + 0.5);
        const bool isMoveEndpoint = activeMove.disk > 0 && (towerIndex == activeMove.from || towerIndex == activeMove.to);
        painter.setBrush(isMoveEndpoint ? (dark ? QColor(178, 191, 204) : QColor(65, 78, 91))
                                        : (dark ? QColor(144, 156, 167) : QColor(106, 115, 125)));
        painter.drawRoundedRect(QRectF(centerX - pegWidth / 2.0, pegTop, pegWidth, pegHeight), 4.0, 4.0);

        painter.setPen(isMoveEndpoint ? (dark ? QColor(230, 236, 242) : QColor(28, 38, 48))
                                      : (dark ? QColor(204, 213, 222) : QColor(83, 91, 98)));
        painter.setFont(QFont(font().family(), 10, QFont::DemiBold));
        painter.drawText(QRectF(centerX - 20.0, baseY + 16.0, 40.0, 20.0), Qt::AlignCenter, QString(QChar('A' + towerIndex)));
        painter.setPen(Qt::NoPen);

        const auto &tower = towers[static_cast<std::size_t>(towerIndex)];
        for (std::size_t level = 0; level < tower.size(); ++level) {
            const int disk = tower[level];
            const qreal ratio = diskCount == 1 ? 1.0 : static_cast<qreal>(disk - 1) / static_cast<qreal>(diskCount - 1);
            const qreal diskWidth = minDiskWidth + (maxDiskWidth - minDiskWidth) * ratio;
            const qreal x = centerX - diskWidth / 2.0;
            const qreal y = baseY - diskHeight * (static_cast<qreal>(level) + 1.0);

            QRectF diskRect(x, y + 1.0, diskWidth, diskHeight - 2.0);
            QLinearGradient diskGradient(diskRect.topLeft(), diskRect.bottomLeft());
            const QColor color = diskColor(disk);
            diskGradient.setColorAt(0.0, color.lighter(125));
            diskGradient.setColorAt(1.0, color.darker(115));
            painter.setBrush(diskGradient);
            painter.drawRoundedRect(diskRect, 5.0, 5.0);

            if (disk == activeMove.disk) {
                painter.setBrush(Qt::NoBrush);
                painter.setPen(QPen(QColor(255, 247, 168, 230), 3.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.drawRoundedRect(diskRect.adjusted(3.0, 3.0, -3.0, -3.0), 4.0, 4.0);
                painter.setPen(QPen(QColor(255, 255, 255, 170), 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                painter.drawRoundedRect(diskRect.adjusted(6.0, 5.0, -6.0, -5.0), 3.0, 3.0);
                painter.setPen(Qt::NoPen);
            }
        }
    }
}

QSize HanoiView::minimumSizeHint() const
{
    return QSize(520, 240);
}

QColor HanoiView::diskColor(int disk) const
{
    const int hue = (disk * 37 + 205) % 360;
    return usesDarkTheme() ? QColor::fromHsv(hue, 148, 232) : QColor::fromHsv(hue, 132, 216);
}

bool HanoiView::usesDarkTheme() const
{
    if (m_theme == HanoiTheme::Dark) {
        return true;
    }
    if (m_theme == HanoiTheme::Light) {
        return false;
    }
    return palette().color(QPalette::Window).lightness() < 128;
}

void HanoiView::drawMoveArrow(QPainter &painter, const QRectF &area, const Move &move, qreal towerWidth) const
{
    const bool dark = usesDarkTheme();
    const qreal fromX = area.left() + towerWidth * (move.from + 0.5);
    const qreal toX = area.left() + towerWidth * (move.to + 0.5);
    const qreal y = area.top() + 56.0;
    const qreal direction = toX > fromX ? 1.0 : -1.0;
    const QPointF start(fromX + direction * 26.0, y);
    const QPointF end(toX - direction * 26.0, y);

    QPainterPath path(start);
    const qreal lift = qBound<qreal>(14.0, qAbs(toX - fromX) * 0.08, 24.0);
    path.cubicTo(QPointF(start.x(), y - lift), QPointF(end.x(), y - lift), end);

    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(dark ? QColor(78, 146, 205, 190) : QColor(42, 104, 158, 170), 8.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPath(path);
    painter.setPen(QPen(dark ? QColor(206, 229, 248) : QColor(244, 250, 255), 3.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPath(path);

    const qreal arrowSize = 11.0;
    QPolygonF head;
    head << end
         << QPointF(end.x() - direction * arrowSize, end.y() - arrowSize * 0.62)
         << QPointF(end.x() - direction * arrowSize, end.y() + arrowSize * 0.62);
    painter.setPen(Qt::NoPen);
    painter.setBrush(dark ? QColor(92, 166, 229) : QColor(42, 104, 158));
    painter.drawPolygon(head);

    painter.setFont(QFont(font().family(), 10, QFont::DemiBold));
    const QString moveText = move.from < move.to
                                 ? QString("%1 -> %2").arg(QChar('A' + move.from)).arg(QChar('A' + move.to))
                                 : QString("%1 <- %2").arg(QChar('A' + move.to)).arg(QChar('A' + move.from));
    const QString label = QString("Disk %1  %2").arg(move.disk).arg(moveText);
    const qreal labelWidth = QFontMetricsF(painter.font()).horizontalAdvance(label) + 22.0;
    const QRectF labelRect((fromX + toX) / 2.0 - labelWidth / 2.0, y - lift - 19.0, labelWidth, 22.0);
    painter.setBrush(dark ? QColor(30, 38, 46, 225) : QColor(255, 255, 255, 215));
    painter.drawRoundedRect(labelRect, 8.0, 8.0);
    painter.setPen(dark ? QColor(228, 237, 245) : QColor(42, 63, 82));
    painter.drawText(labelRect, Qt::AlignCenter, label);
    painter.setPen(Qt::NoPen);
}
