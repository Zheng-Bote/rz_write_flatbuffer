/**
 * @file rz_photo-gallery_plugins.hpp
 * @author ZHENG Bote (robert.hase-zheng.net)
 * @brief QT plugin interface
 * @version 2.3.1
 * @date 2025-11-25
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

class Plugin
{
private:
public:
    // virtual ~Plugin() = 0;

    virtual QString getPluginNameShort() = 0;
    virtual QString getPluginNameLong() = 0;
    virtual QString getPluginVersion() = 0;
    virtual QString getPluginDescription() = 0;

    virtual std::tuple<bool, std::string> parseFile(
        QMap<QString, QString> &mapParseKeys,
        QString pathToFile) = 0;
    virtual std::tuple<bool, std::string> writeFile(
        QMap<QString, QString> mapParseKeys,
        QMap<QString, QString> mapFileAttribs,
        QString pathToFile) = 0;

    virtual void doClose() = 0;
    virtual std::tuple<bool, std::string> doRun(const QString &type = "") = 0;

    virtual std::tuple<bool, std::string> setQMap(const QMap<QString, QString> &setQmap,
                                                  const QString &type)
        = 0;
    virtual QMap<QString, QString> getQMap(const QString &type = "") = 0;

    virtual std::tuple<bool, std::string> setQHash(
        const QHash<QString, QString> &setQhash,
        const QString &type) = 0;
    virtual QHash<QString, QString> getQHash(const QString &type = "") = 0;
};

#define Plugin_iid "net.hase-zheng.photo_gallery_plugins"
Q_DECLARE_INTERFACE(Plugin, Plugin_iid)
