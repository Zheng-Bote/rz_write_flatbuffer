/**
 * @file rz_write_flatbuffers.hpp
 * @author ZHENG Robert (robert.hase-zheng.net)
 * @brief write image metadata into FlatBuffers format
 * @version 0.1.0
 * @date 2025-11-25
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 */

#pragma once

#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QMap>
#include <QObject>
#include <QRegularExpression>
#include <QtPlugin>

#include <flatbuffers/flatbuffers.h>

#include "rz_photo-gallery_plugins.hpp"
#include "rz_write_flatbuffers_generated.h"

class Rz_writeFlatbuffers : public QObject, public Plugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "net.hase-zheng.photo_gallery_plugins");
  Q_INTERFACES(Plugin);

public:
  explicit Rz_writeFlatbuffers(QObject *parent = nullptr);
  ~Rz_writeFlatbuffers() = default;

private:
  bool oknok{false};
  std::string msg{"blank"};
  QString debugMsg{"blank"};

  struct imageStruct
  {
    QString fileName{""};        // 2014-04-18_203353.jpg
    QString fileBasename{""};    // 2014-04-18_203353
    QString fileSuffix{""};      // jpg
    QString fileAbolutePath{""}; // /home/zb_bamboo/pictures/images
  };
  imageStruct imgStruct;

  QMap<QString, QString> qMap;

  QHash<QString, QString> pictureData;
  QHash<QString, QString> exifData;
  QHash<QString, QString> iptcData;
  QHash<QString, QString> xmpData;

  static flatbuffers::Offset<flatbuffers::String> getStr(flatbuffers::FlatBufferBuilder &fbb,
                                                         const QHash<QString, QString> &hash,
                                                         const QString &key);

  flatbuffers::DetachedBuffer createPictureFlatbuffer(const QHash<QString, QString> &pictureData,
                                                      const QHash<QString, QString> &exifData,
                                                      const QHash<QString, QString> &iptcData,
                                                      const QHash<QString, QString> &xmpData);

public:
  QString getPluginNameShort() Q_DECL_OVERRIDE;
  QString getPluginNameLong() Q_DECL_OVERRIDE;
  QString getPluginVersion() Q_DECL_OVERRIDE;
  QString getPluginDescription() Q_DECL_OVERRIDE;

  std::tuple<bool, std::string> parseFile(QMap<QString, QString> &mapParseKeys,
                                          QString pathToFile) Q_DECL_OVERRIDE;
  std::tuple<bool, std::string> writeFile(QMap<QString, QString> mapParseKeys,
                                          QMap<QString, QString> mapFileAttribs,
                                          QString pathToFile) Q_DECL_OVERRIDE;

  std::tuple<bool, std::string> doRun(const QString &outFile = "") Q_DECL_OVERRIDE;
  void doClose() Q_DECL_OVERRIDE;

  std::tuple<bool, std::string> setQMap(const QMap<QString, QString> &setQmap,
                                        const QString &type) Q_DECL_OVERRIDE;
  QMap<QString, QString> getQMap(const QString &type = "") Q_DECL_OVERRIDE;

  std::tuple<bool, std::string> setQHash(const QHash<QString, QString> &setQhash,
                                         const QString &type) Q_DECL_OVERRIDE;
  QHash<QString, QString> getQHash(const QString &type = "") Q_DECL_OVERRIDE;
};
