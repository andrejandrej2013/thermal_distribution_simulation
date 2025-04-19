#include <QtTest>
#include "../ui/HeatMapWidget.h"

class HeatMapWidgetTest : public QObject {
    Q_OBJECT

private slots:
    void test_renderPerformance() {
        HeatMapWidget widget;
        widget.resize(800, 800);
        widget.show();

        QTest::qWait(100);

        auto start = std::chrono::high_resolution_clock::now();
        QTest::qWait(1);  // trigger at least one paintEvent
        widget.repaint();
        QTest::qWait(1);

        auto end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double>(end - start).count();

        QVERIFY2(duration < 0.5, "UI rendering took too long!");  // e.g., 50ms
    }
};

QTEST_MAIN(HeatMapWidgetTest)
#include "HeatMapWidgetTest.moc"
