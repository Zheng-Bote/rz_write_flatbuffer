/**
 * @file rz_write_flatbuffers.cpp
 * @author ZHENG Robert (robert.hase-zheng.net)
 * @brief write image metadata into FlatBuffers format
 * @version 0.2.0
 * @date 2025-11-26
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 */

#include "includes/rz_write_flatbuffers.hpp"
#include "includes/rz_write_flatbuffers_generated.h"

#include <QDir>
#include "includes/rz_config.hpp"
#include <chrono>
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
                                  pictureData.value("filewidth").toFloat(),
                                  pictureData.value("fileheight").toFloat(),
                                  getStr(builder, pictureData, "filepath"),
                                  pictureData.value("filedatetime").toLong(),
                                  getStr(builder, pictureData, "access_groups"),
                                  exifObj,
                                  iptcObj,
                                  xmpObj);

  builder.Finish(pictureObj, "PLUG");
  return builder.Release();
}

std::tuple<bool, std::string> Rz_writeFlatbuffers::isTargetExist(const QFile &pathToTarget,
                                                                 const QString &type)
{
    const QFileInfo fInfo(pathToTarget);

    msg = "";
    oknok = false;

    if (type.contains("dir")) {
        if (!pathToTarget.exists()) {
            qDebug() << "createDirectories(fInfo.absolutePath().toStdString(): "
                     << fInfo.absolutePath().toStdString();
            std::tie(oknok, msg) = createDirectories(fInfo.absoluteFilePath().toStdString());
        }
        if (fInfo.isDir() && fInfo.isWritable()) {
            return std::make_tuple(true,
                                   std::format("{}:{}:{}: Folder exists and is writeable.",
                                               __FILE__,
                                               __FUNCTION__,
                                               __LINE__));
        } else {
            return std::make_tuple(
                true,
                std::format("{}:{}:{}: Target is not a directory or not writeable.",
                            __FILE__,
                            __FUNCTION__,
                            __LINE__));
        }
    }
    if (!pathToTarget.exists()) {
        return std::make_tuple(false,
                               std::format("{}:{}:{}: Target doesn't exist.",
                                           __FILE__,
                                           __FUNCTION__,
                                           __LINE__));
    }
    if (type.contains("file") && fInfo.isFile() && fInfo.isWritable()) {
        return std::make_tuple(true,
                               std::format("{}:{}:{}: File exists and is writeable",
                                           __FILE__,
                                           __FUNCTION__,
                                           __LINE__));
    }
    return std::make_tuple(false, std::format("{}:{}", __FILE__, __FUNCTION__));
}

std::tuple<bool, std::string> Rz_writeFlatbuffers::createDirectories(const std::filesystem::path &p)
{
    std::filesystem::path nested = p;

    try {
        if (std::filesystem::create_directories(nested)) {
            return std::make_tuple(true,
                                   std::format("{}:{}:{}: Nested directories created successfully",
                                               __FILE__,
                                               __FUNCTION__,
                                               __LINE__));

        } else {
            return std::make_tuple(false,
                                   std::format("{}:{}:{}: Failed to create nested directories",
                                               __FILE__,
                                               __FUNCTION__,
                                               __LINE__));
            // std::cout << ec.message() << '\n';
        }
    } catch (const std::exception &ex) {
        std::string msg = std::format("{}:{}:{}: Failed creating directories: ",
                                      __FILE__,
                                      __FUNCTION__,
                                      __LINE__);
        msg.append(ex.what());
        return std::make_tuple(false, msg);
    }
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

int32_t Rz_writeFlatbuffers::getPluginMajorVersion()
{
    return PROJECT_VERSION_MAJOR;
}

int32_t Rz_writeFlatbuffers::getPluginMinorVersion()
{
    return PROJECT_VERSION_MINOR;
}

int32_t Rz_writeFlatbuffers::getPluginPatchVersion()
{
    return PROJECT_VERSION_PATCH;
}

QString Rz_writeFlatbuffers::getPluginHomepageUrl()
{
    return PROJECT_HOMEPAGE_URL.c_str();
}

QString Rz_writeFlatbuffers::getPluginCopyright()
{
    const auto now = std::chrono::system_clock::now();
    std::string ret = std::format("Copyright {}-{:%Y} {}", PROG_CREATED, now, PROG_AUTHOR);
    return ret.c_str();
}

QString Rz_writeFlatbuffers::getPluginTechInfo()
{
    std::string ret = std::format("{} {}", CMAKE_CXX_COMPILER, CMAKE_CXX_STANDARD);
    return ret.c_str();
}

std::tuple<bool, std::string> Rz_writeFlatbuffers::parseFile(const QString &type)
{
  return std::make_tuple(true, std::format("{}:{}", __FILE__, __FUNCTION__));
}

/**
 * @brief Rz_writeFlatbuffers::writeFile
 * @param type <path to flatbuffers folder>
 * @return <bool, msg string>
 */
std::tuple<bool, std::string> Rz_writeFlatbuffers::writeFile(const QString &pathToBinDir)
{
    std::tie(oknok, msg) = isTargetExist(QFile(pathToBinDir), "dir");
    if (!oknok) {
        return std::make_tuple(false,
                               std::format("{}:{}:{}: {}", __FILE__, __FUNCTION__, __LINE__, msg));
    }

    auto buf = createPictureFlatbuffer(pictureData, exifData, iptcData, xmpData);

    QString binFile = pathToBinDir + "/" + imgStruct.fileBasename + ".bin";

    QFile out(binFile);
    if (out.open(QIODevice::WriteOnly)) {
        out.write(reinterpret_cast<const char *>(buf.data()), buf.size());
        out.close();
        qDebug() << "FlatBuffer saved: " << binFile << "(" << buf.size() << " bytes)\n";
    } else {
        qDebug() << "Unable to write file " << out.errorString();
        return std::make_tuple(false,
                               std::format("{}:{}: Unable to write file {}.",
                                           __FILE__,
                                           __FUNCTION__,
                                           out.errorString().toStdString()));
    }
    return std::make_tuple(true,
                           std::format("{}:{}: {}", __FILE__, __FUNCTION__, binFile.toStdString()));
}

std::tuple<bool, std::string> Rz_writeFlatbuffers::doRun(const QString &type)
{
    return std::make_tuple(true, std::format("{}:{}", __FILE__, __FUNCTION__));
}

std::tuple<bool, std::string> Rz_writeFlatbuffers::doClose(const QString &type)
{
    return std::make_tuple(true, std::format("{}:{}", __FILE__, __FUNCTION__));
}

std::tuple<bool, std::string> Rz_writeFlatbuffers::setQstring(const QString &string,
                                                              const QString &type)
{
    if (type.contains("imgStruct")) {
        QFileInfo fileInfo(string);

        imgStruct.fileName = fileInfo.fileName();
        imgStruct.fileBasename = fileInfo.completeBaseName();
        imgStruct.fileSuffix = fileInfo.completeSuffix();
        imgStruct.fileAbolutePath = fileInfo.absolutePath();
        imgStruct.homePath = QDir::homePath();
        return std::make_tuple(true,
                               std::format("{}:{}:{}: imgStruct", __FILE__, __FUNCTION__, __LINE__));
    }

    return std::make_tuple(false, std::format("{}:{}: wrong paramater", __FILE__, __FUNCTION__));
}

QString Rz_writeFlatbuffers::getQstring(const QString &type)
{
    return "";
}

std::tuple<bool, std::string> Rz_writeFlatbuffers::setQList(const QList<QString> &stringList,
                                                            const QString &type)
{
    return std::make_tuple(true, std::format("{}:{}:{}", __FILE__, __FUNCTION__, __LINE__));
}

QList<QString> Rz_writeFlatbuffers::getQList(const QString &type)
{
    QList<QString> list("blender");
    return list;
}

std::tuple<bool, std::string> Rz_writeFlatbuffers::setQMap(const QMap<QString, QString> &setQmap,
                                                           const QString &type)
{
    return std::make_tuple(true, std::format("{}:{}:{}", __FILE__, __FUNCTION__, __LINE__));
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
                         std::format("{}:{}:{}: wrong parameter", __FILE__, __FUNCTION__, __LINE__));
}

QHash<QString, QString> Rz_writeFlatbuffers::getQHash(const QString &type)
{
  return exifData;
}
