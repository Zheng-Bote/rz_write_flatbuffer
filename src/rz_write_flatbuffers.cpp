/**
 * @file rz_write_flatbuffers.cpp
 * @author ZHENG Robert (robert.hase-zheng.net)
 * @brief write image metadata into FlatBuffers format
 * @version 0.1.1
 * @date 2025-11-25
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 */

#include "includes/rz_write_flatbuffers.hpp"

#include "includes/rz_config.hpp"
#include <format>

using namespace Image::Metadatas;

Rz_writeFlatbuffers::Rz_writeFlatbuffers(QObject *parent)
{
  Q_UNUSED(parent);
}

flatbuffers::Offset<flatbuffers::String> Rz_writeFlatbuffers::getStr(
    flatbuffers::FlatBufferBuilder &fbb, const QHash<QString, QString> &hash, const QString &key)
{
  auto it = hash.find(key);
  if (it == hash.end())
  {
    return fbb.CreateString("");
  }
  return fbb.CreateString(it.value().toStdString());
}

flatbuffers::DetachedBuffer Rz_writeFlatbuffers::createPictureFlatbuffer(
    const QHash<QString, QString> &pictureData,
    const QHash<QString, QString> &exifData,
    const QHash<QString, QString> &iptcData,
    const QHash<QString, QString> &xmpData)
{
  flatbuffers::FlatBufferBuilder builder(4096);

  auto exifObj = CreateExif(builder,
                            getStr(builder, exifData, "file_name"),
                            getStr(builder, exifData, "gpstag"),
                            getStr(builder, exifData, "imagedescription"),
                            getStr(builder, exifData, "gpslongituderef"),
                            getStr(builder, exifData, "gpsmapdatum"),
                            getStr(builder, exifData, "imageuniqueid"),
                            getStr(builder, exifData, "imageid"),
                            getStr(builder, exifData, "gpslatituderef"),
                            getStr(builder, exifData, "usercomment"),
                            getStr(builder, exifData, "gpsaltitude"),
                            getStr(builder, exifData, "documentname"),
                            getStr(builder, exifData, "gpstimestamp"),
                            getStr(builder, exifData, "copyright"),
                            getStr(builder, exifData, "gpsaltituderef"),
                            getStr(builder, exifData, "gpsdatestamp"),
                            getStr(builder, exifData, "gpslongitude"),
                            getStr(builder, exifData, "gpslatitude"),
                            getStr(builder, exifData, "datetimeoriginal"),
                            getStr(builder, exifData, "securityclassification"));
  auto iptcObj = CreateIptc(builder,
                            getStr(builder, iptcData, "file_name"),
                            getStr(builder, iptcData, "objectname"),
                            getStr(builder, iptcData, "copyright"),
                            getStr(builder, iptcData, "caption"));
  auto xmpObj = CreateXmp(builder,
                          getStr(builder, xmpData, "file_name"),
                          getStr(builder, xmpData, "copyrightowner"),
                          getStr(builder, xmpData, "documentname"),
                          getStr(builder, xmpData, "zipcode"),
                          getStr(builder, xmpData, "language"),
                          getStr(builder, xmpData, "countrycode"),
                          getStr(builder, xmpData, "localaddress"),
                          getStr(builder, xmpData, "sublocation"),
                          getStr(builder, xmpData, "category"),
                          getStr(builder, xmpData, "provincestate"),
                          getStr(builder, xmpData, "city"),
                          getStr(builder, xmpData, "imageid"),
                          getStr(builder, xmpData, "keywords"),
                          getStr(builder, xmpData, "countryname"),
                          getStr(builder, xmpData, "streetname"),
                          getStr(builder, xmpData, "rights"),
                          getStr(builder, xmpData, "description"),
                          getStr(builder, xmpData, "title"),
                          getStr(builder, xmpData, "subject"),
                          getStr(builder, xmpData, "securityclassification"));

  auto pictureObj = CreatePicture(builder,
                                  getStr(builder, pictureData, "file_name"),
                                  pictureData.value("filesize").toFloat(),
                                  pictureData.value("filesize_x").toFloat(),
                                  pictureData.value("filesize_y").toFloat(),
                                  getStr(builder, pictureData, "filepath"),
                                  exifObj,
                                  iptcObj,
                                  xmpObj);

  builder.Finish(pictureObj, "PLUG");
  return builder.Release();
}

QString Rz_writeFlatbuffers::getPluginNameShort()
{
  return PROJECT_NAME.c_str();
}

QString Rz_writeFlatbuffers::getPluginNameLong()
{
  return PROG_LONGNAME.c_str();
}

QString Rz_writeFlatbuffers::getPluginVersion()
{
  std::string ret = std::format("{}-v{}", PROJECT_NAME, PROJECT_VERSION);
  return ret.c_str();
}

QString Rz_writeFlatbuffers::getPluginDescription()
{
  return PROJECT_DESCRIPTION.c_str();
}

std::tuple<bool, std::string> Rz_writeFlatbuffers::parseFile(QMap<QString, QString> &mapParseKeys,
                                                             QString pathToFile)
{
  return std::make_tuple(true, std::format("{}:{}", __FILE__, __FUNCTION__));
}

std::tuple<bool, std::string> Rz_writeFlatbuffers::writeFile(QMap<QString, QString> mapParseKeys,
                                                             QMap<QString, QString> mapFileAttribs,
                                                             QString pathToFile)
{
  return std::make_tuple(true, std::format("{}:{}", __FILE__, __FUNCTION__));
}

std::tuple<bool, std::string> Rz_writeFlatbuffers::doRun(const QString &outFile)
{
  auto buf = createPictureFlatbuffer(pictureData, exifData, iptcData, xmpData);

  QFile out(outFile);
  if (out.open(QIODevice::WriteOnly))
  {
    out.write(reinterpret_cast<const char *>(buf.data()), buf.size());
    out.close();
    qDebug() << "FlatBuffer saved: " << outFile << "(" << buf.size() << " bytes)\n";
  }
  else
  {
    qDebug() << "Unable to write file!\n";
  }

  return std::make_tuple(true, "Rz_writeSQLfile::parseFile");
}

void Rz_writeFlatbuffers::doClose() {}

std::tuple<bool, std::string> Rz_writeFlatbuffers::setQMap(const QMap<QString, QString> &setQmap,
                                                           const QString &type)
{
  return std::make_tuple(true, "Rz_writeSQLfile::parseFile");
}

QMap<QString, QString> Rz_writeFlatbuffers::getQMap(const QString &type)
{
  return qMap;
}

std::tuple<bool, std::string> Rz_writeFlatbuffers::setQHash(const QHash<QString, QString> &setQhash,
                                                            const QString &type)
{
  if (type.contains("PICTURE"))
  {
    pictureData = setQhash;
    return std::make_tuple(true, std::format("{}:{}: PICTURE", __FILE__, __FUNCTION__));
  }
  if (type.contains("EXIF"))
  {
    exifData = setQhash;
    return std::make_tuple(true, std::format("{}:{}: EXIF", __FILE__, __FUNCTION__));
  }
  if (type.contains("IPTC"))
  {
    iptcData = setQhash;
    return std::make_tuple(true, std::format("{}:{}: IPTC", __FILE__, __FUNCTION__));
  }
  if (type.contains("XMP"))
  {
    xmpData = setQhash;
    return std::make_tuple(true, std::format("{}:{}: XMP", __FILE__, __FUNCTION__));
  }

  return std::make_tuple(false,
                         std::format("{}:{}:{}: wrong parameter",
                                     __FILE__,
                                     __FUNCTION__,
                                     __LINE__));
}

QHash<QString, QString> Rz_writeFlatbuffers::getQHash(const QString &type)
{
  return exifData;
}
