/**
 * @file rz_photo-gallery_plugins.hpp
 * @author ZHENG Bote (robert.hase-zheng.net)
 * @brief QT plugin interface
 * @version 2.4.1
 * @date 2025-11-26
 *
 * @copyright Copyright (c) 2023-2025 ZHENG Robert
 *
 */

#pragma once

#include <QDebug>
#include <QHash>
#include <QMap>
#include <QString>
#include <QtPlugin>
#include <string>
#include <tuple>

/**
 * @brief The Plugin class
 * @details plugin interface class with virtual methods
 */
class Plugin
{
private:
public:
    // virtual ~Plugin() = 0;

    /**
     * @brief getPluginNameShort
     * @return QString <rz_write_flatbuffers>
     */
    virtual QString getPluginNameShort() = 0;

    /**
     * @brief getPluginNameLong
     * @return QString <write FlatBuffer>
     */
    virtual QString getPluginNameLong() = 0;

    /**
     * @brief getPluginVersion
     * @return QString <rz_write_flatbuffers-v0.0.0>
     */
    virtual QString getPluginVersion() = 0;

    virtual int32_t getPluginMajorVersion() = 0;
    virtual int32_t getPluginMinorVersion() = 0;
    virtual int32_t getPluginPatchVersion() = 0;

    virtual QString getPluginDescription() = 0;

    /**
     * @brief getPluginHomepageUrl
     * @return QString <https://github.com/Zheng-Bote/rz_write_flatbuffer>
     */
    virtual QString getPluginHomepageUrl() = 0;

    /**
     * @brief getPluginCopyright
     * @return QString <Copyright 2025-2038 ZHENG Robert>
     */
    virtual QString getPluginCopyright() = 0;

    /**
     * @brief getPluginTechInfo
     * @return QString <CMAKE_CXX_COMPILER CMAKE_CXX_STANDARD>
     */
    virtual QString getPluginTechInfo() = 0;

    virtual std::tuple<bool, std::string> parseFile(const QString &type = "") = 0;
    virtual std::tuple<bool, std::string> writeFile(const QString &type = "") = 0;

    virtual std::tuple<bool, std::string> doClose(const QString &type = "") = 0;
    virtual std::tuple<bool, std::string> doRun(const QString &type = "") = 0;

    virtual std::tuple<bool, std::string> setQstring(const QString &string, const QString &type = "") = 0;
    virtual QString getQstring(const QString &type = "") = 0;

    virtual std::tuple<bool, std::string> setQList(const QList<QString> &stringList,
                                                   const QString &type = "")
        = 0;
    virtual QList<QString> getQList(const QString &type = "") = 0;

    virtual std::tuple<bool, std::string> setQMap(const QMap<QString, QString> &setQmap,
                                                  const QString &type = "") = 0;
    virtual QMap<QString, QString> getQMap(const QString &type = "") = 0;

    virtual std::tuple<bool, std::string> setQHash(const QHash<QString, QString> &setQhash,
                                                   const QString &type = "") = 0;
    virtual QHash<QString, QString> getQHash(const QString &type = "") = 0;
};

#define Plugin_iid "net.hase-zheng.photo_gallery_plugins"
Q_DECLARE_INTERFACE(Plugin, Plugin_iid)
