#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <cstdint>

const QString INVALID_TIME_STAMP = QStringLiteral("Invalid!");

QString convertToDateTime(const QString &arg)
{
    bool ok = false;
    qulonglong stamp = arg.toULongLong(&ok);
    if (stamp > static_cast<std::uint32_t>(-1)) {
        stamp = stamp / 1000;
        if (stamp > static_cast<std::uint32_t>(-1)) {
            ok = false;
        }
    }

    if (ok) {
        return QDateTime::fromTime_t(stamp).toUTC().toString("dd.MM.yyyy hh:mm:ss.zzz");
    }

    return INVALID_TIME_STAMP;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            qDebug() << convertToDateTime(QString(argv[i])) << "\n";
        }
    } else {
        const QClipboard *clip = app.clipboard();
        const QString text = clip->text(QClipboard::Clipboard);
        QStringList stamps = text.split("\n", QString::SkipEmptyParts);

        if (stamps.size() == 1 && convertToDateTime(stamps[0]) == INVALID_TIME_STAMP) {
            stamps = text.split(" ", QString::SkipEmptyParts);
        }

        if (stamps.size() == 1 && convertToDateTime(stamps[0]) == INVALID_TIME_STAMP) {
            stamps = text.split("\t", QString::SkipEmptyParts);
        }

        for (const auto & arg : stamps) {
            qDebug() << convertToDateTime(arg);
        }
    }
}
