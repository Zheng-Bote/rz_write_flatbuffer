/**
 * @file test_write.cpp
 * @author ZHENG Robert (robert.hase-zheng.net)
 * @brief testing the FlatBuffers plugin (write bin)
 * @version 0.1.0
 * @date 2025-11-25
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 */

#include <QCoreApplication>
#include <QDebug>
#include <QHash>
#include <QPluginLoader>

#include <cstdlib>
#include <exception>
#include <iostream>

#include <tuple>

#include "includes/rz_config.hpp"
#include "includes/rz_photo-gallery_plugins.hpp"
#include <print>

void exit_handler();
void terminate_handler();

void getDetailedInfo();

QPluginLoader loader;
Plugin *plugin = nullptr;

int main()
{
    std::atexit(exit_handler);
    std::at_quick_exit(exit_handler);
    std::set_terminate(terminate_handler);

    bool oknok{false};
    std::string msg{""};
    struct imageStruct
    {
        QString fileName{"2014-04-18_203353.jpg"};
        QString fileBasename{"2014-04-18_203353"};
        QString fileSuffix{"jpg"};
        QString fileAbolutePath{"/home/zb_bamboo/pictures/images"};
    };
    const imageStruct imgStruct;

    getDetailedInfo();

    // QT Creator
    // loader.setFileName("/home/zb_bamboo/DEV/__NEW__/CPP/Qt_Plugins/plugins/rz_write_flatbuffer/"
    //                    "build/Desktop_Qt_6_10_0-Debug/librz_write_flatbuffers");

    // VSC
    loader.setFileName("/home/zb_bamboo/DEV/__NEW__/CPP/Qt_Plugins/plugins/rz_write_flatbuffer/"
                       "build/librz_write_flatbuffers");

    if (!loader.load())
    {
        std::cerr << "Exception: " << loader.errorString().toStdString();

        std::terminate();
    }

    qDebug() << "\nLoaded: " << loader.fileName();

    plugin = qobject_cast<Plugin *>(loader.instance());
    if (plugin)
    {
        qDebug() << "Plugin object OK";
        qDebug() << "Plugin: " << plugin->getPluginNameLong() << " " << plugin->getPluginVersion();
    }

    QHash<QString, QString> pictureData{{"file_name", imgStruct.fileName},
                                        {"filesize", "12345"},
                                        {"filewidth", "4096"},
                                        {"fileheight", "2160"},
                                        {"filepath", imgStruct.fileAbolutePath},
                                        {"filedatetime", "2014-04-18_203353"},
                                        {"access_groups", "admin"}};
    std::tie(oknok, msg) = plugin->setQHash(pictureData, "PICTURE");
    if (!oknok)
    {
        std::cerr << msg;
        std::terminate();
    }
    else
    {
        qDebug() << oknok << ":" << msg;
    }

    QHash<QString, QString> exifData{{"file_name", imgStruct.fileName},
                                     {"gpstag", "ACTIVE"},
                                     {"imagedescription", "Ein schönes Bild"},
                                     {"gpslatitude", "52.5200"},
                                     {"gpslongitude", "13.4050"}};

    std::tie(oknok, msg) = plugin->setQHash(exifData, "EXIF");
    QHash<QString, QString> iptcData{{"file_name", imgStruct.fileName},
                                     {"objectname", "Sonnenuntergang"},
                                     {"caption", "Sehr schönes Abendlicht"},
                                     {"copyright", "© Mustermann"}};

    std::tie(oknok, msg) = plugin->setQHash(iptcData, "IPTC");
    QHash<QString, QString> xmpData{{"file_name", imgStruct.fileName},
                                    {"imageid", "IMG2025"},
                                    {"keywords", "sommer,meer,urlaub"},
                                    {"title", "Sommer am Meer"},
                                    {"city", "Berlin"}};
    std::tie(oknok, msg) = plugin->setQHash(xmpData, "XMP");

    // create Image struct, needed for FlatBuffer filename
    std::tie(oknok, msg) = plugin->setQstring(imgStruct.fileAbolutePath + "/" + imgStruct.fileName,
                                              "imgStruct");

    // write FlatBuffer to the given folder (<fileBasename>.bin)
    plugin->writeFile("/home/zb_bamboo/DEV/__NEW__/CPP/Qt_Plugins/plugins/rz_write_flatbuffer/"
                      "build/FlatBuffers/test/ImageBins");

    return EXIT_SUCCESS;
}

void exit_handler()
{
    qDebug() << "exit handler";
    try
    {
        plugin->doClose();
        loader.unload();
        qDebug() << "Plugin closed and unloaded";
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Exception: " << ex.what();
    }
    catch (...)
    {
        std::cerr << "Exception: unknown";
    }
}

void terminate_handler()
{
    std::cerr << "\n=terminated=\n";
}

void getDetailedInfo()
{
    const auto width = 20;
    std::cout << "\n";
    std::cout << std::left << std::setfill('.') << std::setw(width) << "Name:" << PROG_LONGNAME
              << std::endl;
    std::cout << std::left << std::setfill('.') << std::setw(width) << "Name:" << PROJECT_NAME
              << std::endl;
    std::cout << std::left << std::setfill('.') << std::setw(width) << "Version:" << PROJECT_VERSION
              << std::endl;
    std::cout << std::left << std::setfill('.') << std::setw(width)
              << "Description:" << PROJECT_DESCRIPTION << std::endl;
    std::cout << std::left << std::setfill('.') << std::setw(width)
              << "Technical:" << CMAKE_CXX_COMPILER << " " << CMAKE_CXX_STANDARD << std::endl;
    std::cout << std::left << std::setfill('.') << std::setw(width) << "Author:" << PROG_AUTHOR
              << std::endl;
    std::cout << std::left << std::setfill('.') << std::setw(width)
              << "Homepage:" << PROJECT_HOMEPAGE_URL << std::endl;
}
