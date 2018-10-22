#include "logger.h"
#include "notificationmanager.h"
#include "stylemanager.h"

#include <QDebug>

PING_LOGGING_CATEGORY(NOTIFICATIONMANAGER, "ping.notificationmanager")

NotificationManager::NotificationManager()
{
    connect(&_model, &NotificationModel::dataChanged, this, [this] { emit modelUpdate(); });
}

void NotificationManager::create(const QString& text, QColor color, QString icon)
{
    _model.add(text, icon, color);
    qDebug(NOTIFICATIONMANAGER) << "New Notification:" << text;
}

QObject* NotificationManager::qmlSingletonRegister(QQmlEngine* engine, QJSEngine* scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return self();
}

NotificationManager* NotificationManager::self()
{
    static NotificationManager* self = new NotificationManager();
    return self;
}
