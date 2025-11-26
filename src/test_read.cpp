/**
 * @file test_read.cpp
 * @author ZHENG Robert (robert.hase-zheng.net)
 * @brief testing (reading) the FlatBuffers bin file
 * @version 0.2.0
 * @date 2025-11-26
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 */

#include <QCoreApplication>

#include <cstdlib>
#include <exception>

#include <QHash>
#include <QString>
#include <QFile>
#include <QDebug>
#include <iostream>

#include <flatbuffers/flatbuffers.h>
#include "includes/rz_write_flatbuffers_generated.h"

using namespace Image::Metadatas;

static QString fbStr(const flatbuffers::String *s)
{
    if (!s)
        return "";
    return QString::fromUtf8(s->c_str());
}

bool loadPictureFlatbuffer(
    const QString &filename,
    QHash<QString, QString> &pictureData,
    QHash<QString, QString> &exifData,
    QHash<QString, QString> &iptcData,
    QHash<QString, QString> &xmpData)
{

    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
    {
        std::cerr << "Kann Datei nicht öffnen!\n";
        return false;
    }

    QByteArray bytes = f.readAll();
    f.close();

    // -------------------------------
    // Prüfen ob gültiger FlatBuffer
    // -------------------------------
    if (!flatbuffers::Verifier(
             reinterpret_cast<const uint8_t *>(bytes.data()), bytes.size())
             .VerifyBuffer<Picture>(nullptr))
    {
        std::cerr << "Ungültiger FlatBuffer!\n";
        return false;
    }

    // -------------------------------
    // GetRoot
    // -------------------------------
    auto pic = flatbuffers::GetRoot<Picture>(bytes.data());
    if (!pic)
    {
        std::cerr << "Picture konnte nicht gelesen werden!\n";
        return false;
    }

    // -------------------------------
    // Picture → QHash
    // -------------------------------
    pictureData["file_name"] = fbStr(pic->file_name());
    pictureData["filesize"] = QString::number(pic->filesize());
    pictureData["filewidth"] = QString::number(pic->filewidth());
    pictureData["fileheight"] = QString::number(pic->fileheight());
    pictureData["filepath"] = fbStr(pic->filepath());
    pictureData["filedatetime"] = QString::number(pic->filedatetime());
    pictureData["access_groups"] = fbStr(pic->access_group());

    // -------------------------------
    // Exif → QHash
    // -------------------------------
    if (auto ex = pic->exifdata())
    {
        exifData["file_name"] = fbStr(ex->file_name());
        exifData["gpstag"] = fbStr(ex->gpstag());
        exifData["imagedescription"] = fbStr(ex->imagedescription());
        exifData["gpslongituderef"] = fbStr(ex->gpslongituderef());
        exifData["gpsmapdatum"] = fbStr(ex->gpsmapdatum());
        exifData["imageuniqueid"] = fbStr(ex->imageuniqueid());
        exifData["imageid"] = fbStr(ex->imageid());
        exifData["gpslatituderef"] = fbStr(ex->gpslatituderef());
        exifData["usercomment"] = fbStr(ex->usercomment());
        exifData["gpsaltitude"] = fbStr(ex->gpsaltitude());
        exifData["documentname"] = fbStr(ex->documentname());
        exifData["gpstimestamp"] = fbStr(ex->gpstimestamp());
        exifData["copyright"] = fbStr(ex->copyright());
        exifData["gpsaltituderef"] = fbStr(ex->gpsaltituderef());
        exifData["gpsdatestamp"] = fbStr(ex->gpsdatestamp());
        exifData["gpslongitude"] = fbStr(ex->gpslongitude());
        exifData["gpslatitude"] = fbStr(ex->gpslatitude());
        exifData["datetimeoriginal"] = fbStr(ex->datetimeoriginal());
        exifData["securityclassification"] = fbStr(ex->securityclassification());
    }

    // -------------------------------
    // IPTC → QHash
    // -------------------------------
    if (auto iptc = pic->iptcdata())
    {
        iptcData["file_name"] = fbStr(iptc->file_name());
        iptcData["objectname"] = fbStr(iptc->objectname());
        iptcData["copyright"] = fbStr(iptc->copyright());
        iptcData["caption"] = fbStr(iptc->caption());
    }

    // -------------------------------
    // XMP → QHash
    // -------------------------------
    if (auto xmp = pic->xmpdata())
    {
        xmpData["file_name"] = fbStr(xmp->file_name());
        xmpData["copyrightowner"] = fbStr(xmp->copyrightowner());
        xmpData["documentname"] = fbStr(xmp->documentname());
        xmpData["zipcode"] = fbStr(xmp->zipcode());
        xmpData["language"] = fbStr(xmp->language());
        xmpData["countrycode"] = fbStr(xmp->countrycode());
        xmpData["localaddress"] = fbStr(xmp->localaddress());
        xmpData["sublocation"] = fbStr(xmp->sublocation());
        xmpData["category"] = fbStr(xmp->category());
        xmpData["provincestate"] = fbStr(xmp->provincestate());
        xmpData["city"] = fbStr(xmp->city());
        xmpData["imageid"] = fbStr(xmp->imageid());
        xmpData["keywords"] = fbStr(xmp->keywords());
        xmpData["countryname"] = fbStr(xmp->countryname());
        xmpData["streetname"] = fbStr(xmp->streetname());
        xmpData["rights"] = fbStr(xmp->rights());
        xmpData["description"] = fbStr(xmp->description());
        xmpData["title"] = fbStr(xmp->title());
        xmpData["subject"] = fbStr(xmp->subject());
        xmpData["securityclassification"] = fbStr(xmp->securityclassification());
    }

    return true;
}

int main()
{
    QHash<QString, QString> pictureHash;
    QHash<QString, QString> exifHash;
    QHash<QString, QString> iptcHash;
    QHash<QString, QString> xmpHash;

    if (loadPictureFlatbuffer("2014-04-18_203353.bin", pictureHash, exifHash, iptcHash, xmpHash))
    {
        qDebug() << "Picture Name:" << pictureHash["file_name"];
        qDebug() << "Exif Description: " << exifHash["imagedescription"];
        qDebug() << "IPTC Object: " << iptcHash["objectname"];
        qDebug() << "XMP Title:" << xmpHash["title"];
    }
}
