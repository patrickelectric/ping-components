#pragma once

#include <QDebug>
#include <QMap>
#include <QString>
#include <QStringList>

#include "abstractlinknamespace.h"

/**
 * @brief Link configuration class
 *
 */
class LinkConfiguration : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Link configuration structure
     *
     */
    struct LinkConf {
        QStringList args;
        QString name;
        LinkType type = LinkType::None;
    };

    /**
     * @brief Link configuration errors
     *
     */
    enum Error {
        MissingConfiguration, // This can be used in future for warnings and not real errors
        NoErrors = 0,
        NoType,
        InvalidType,
        NoArgs,
        InvalidArgsNumber,
        ArgsAreEmpty,
        InvalidUrl,
    };

    /**
     * @brief Construct a new Link Configuration object
     *
     * @param linkType
     * @param args
     * @param name
     */
    LinkConfiguration(LinkType linkType = LinkType::None, QStringList args = QStringList(), QString name = QString())
        : _linkConf{args, name, linkType} {};

    /**
     * @brief Construct a new Link Configuration object
     *
     * @param confLinkStructure
     */
    LinkConfiguration(LinkConf& confLinkStructure)
        : _linkConf{confLinkStructure} {};

    /**
     * @brief Construct a new Link Configuration object
     *
     * @param other
     * @param parent
     */
    LinkConfiguration(const LinkConfiguration& other, QObject* parent = nullptr)
        : QObject(parent)
        , _linkConf{other.configurationStruct()} {};

    /**
     * @brief Destroy the Link Configuration object
     *
     */
    ~LinkConfiguration() = default;

    /**
     * @brief Return a list with the arguments
     *
     * @return const QStringList*
     */
    const QStringList* args() const { return &_linkConf.args; };

    /**
     * @brief Return list as copy
     *
     * @return Q_INVOKABLE argsAsConst
     */
    Q_INVOKABLE QStringList argsAsConst() const { return _linkConf.args; };

    /**
     * @brief Check if type is the one in the configuration
     *
     * @param type
     * @return true
     * @return false
     */
    bool checkType(LinkType type) { return _linkConf.type == type; };

    /**
     * @brief Return configuration as structure
     *
     * @return LinkConf
     */
    LinkConf configurationStruct() const { return _linkConf; };

    /**
     * @brief Return a pointer for the configuration structure
     *
     * @return const LinkConf*
     */
    const LinkConf* configurationStructPtr() const { return &_linkConf; };

    /**
     * @brief Create and return a configuration string
     *
     * @return const QString
     */
    const QString createConfString() const { return _linkConf.args.join(":"); };

    /**
     * @brief Create and return a configuration in string list format
     *
     * @return const QStringList
     */
    const QStringList createConfStringList() const { return _linkConf.args; };

    /**
     * @brief Create and return old style configuration link
     *
     * @return const QString
     */
    const QString createFullConfString() const;

    /**
     * @brief Create a full configuration in string list format
     *
     * @return const QStringList
     */
    const QStringList createFullConfStringList() const;

    /**
     * @brief Return error numbers
     *
     * @return Error
     */
    Error error() const;

    /**
     * @brief Convert error number in a friendly human message
     *
     * @param error
     * @return QString
     */
    static QString errorToString(Error error) { return _errorMap[error]; }

    /**
     * @brief Check if configuration is correct
     *
     * @return bool
     */
    Q_INVOKABLE bool isValid() const { return error() <= NoErrors; }

    /**
     * @brief Return configuration name
     *
     * @return Q_INVOKABLE name
     */
    Q_INVOKABLE QString name() const { return _linkConf.name; };

    /**
     * @brief Set configuration name
     *
     * @param name
     */
    void setName(QString name) { _linkConf.name = name; };

    /**
     * @brief Return serialport system path
     *
     * @return QString
     */
    QString serialPort();

    /**
     * @brief Return serial baudrate
     *
     * @return int
     */
    int serialBaudrate();

    /**
     * @brief Set the Type object
     *
     * @param type
     */
    void setType(LinkType type) { _linkConf.type = type; };

    /**
     * @brief Return link configuration type
     *
     * @return AbstractLinkNamespace::LinkType
     */
    Q_INVOKABLE AbstractLinkNamespace::LinkType type() const { return _linkConf.type; };

    /**
     * @brief Will return argument with UDP host name
     *
     * @return QString
     */
    QString udpHost();

    /**
     * @brief Will return port used in UDP connection
     *
     * @return int
     */
    int udpPort();

    /**
     * @brief Copy operator
     *
     * @param other
     * @return LinkConfiguration&
     */
    LinkConfiguration& operator = (const LinkConfiguration& other)
    {
        this->_linkConf = other.configurationStruct();
        return *this;
    }

private:
    static const QMap<Error, QString> _errorMap;
    LinkConf _linkConf;
};

bool operator==(const LinkConfiguration& first, const LinkConfiguration& second);
QDebug operator<<(QDebug d, const LinkConfiguration& other);
QDataStream& operator<<(QDataStream &out, const LinkConfiguration linkConfiguration);
QDataStream& operator>>(QDataStream &in, LinkConfiguration &linkConfiguration);

Q_DECLARE_METATYPE(LinkConfiguration)

/**
 * @brief This allows us to register LinkConfiguration before singletons and main function
 * This is something like Q_DECLARE_METATYPE but for operators
 * We should create a Q_DECLARE_METATYPE_STREAM_OPERATORS defined function to deal with future cases
 * For any further explanation:
 * http://doc.qt.io/qt-5/qmetatype.html#Q_DECLARE_METATYPE
 * https://code.woboq.org/qt5/qtbase/src/corelib/kernel/qmetatype.h.html#_M/Q_DECLARE_METATYPE
 * https://code.woboq.org/qt5/qtbase/src/corelib/kernel/qmetatype.h.html#_Z32qRegisterMetaTypeStreamOperatorsPKcPT_
 */
struct LinkConfigurationRegisterStruct {
    LinkConfigurationRegisterStruct()
    {
        static QBasicAtomicInt metatypeId = Q_BASIC_ATOMIC_INITIALIZER(0);
        if (const int id = metatypeId.loadAcquire()) {
            return;
        }

        const int newId = qRegisterMetaType<LinkConfiguration>("LinkConfiguration");
        metatypeId.storeRelease(newId);
        qRegisterMetaTypeStreamOperators<LinkConfiguration>("LinkConfiguration");
    }
};

namespace
{
LinkConfigurationRegisterStruct _linkConfigurationRegisterStruct;
}
