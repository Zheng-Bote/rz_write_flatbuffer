/**
 * @file main.cpp
 * @author ZHENG Robert (robert.hase-zheng.net)
 * @brief test Flatbuffers
 * @version 0.1.0
 * @date 2025-11-23
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 */

#include <print>
#include <fstream>

#include <flatbuffers/flatbuffers.h>
#include "includes/rz_write_flatbuffers_generated.h"

void write_monster_to_file(const std::string &filename);
void read_monster_from_file(const std::string &filename);

int main()
{
  std::println("Test Flatbuffers");

  const std::string filename = "image.bin";

  write_monster_to_file(filename);

  read_monster_from_file(filename);

  return 0;
}

void write_monster_to_file(const std::string &fname)
{
  flatbuffers::FlatBufferBuilder builder(1024);

  auto exif_name = builder.CreateString("exif_test.png");
  auto exif_copyright = builder.CreateString("2024 ZHENG Robert");
  Image::Metadatas::ExifBuilder exif_builder(builder);
  exif_builder.add_file_name(exif_name);
  exif_builder.add_copyright(exif_copyright);
  auto exif = exif_builder.Finish();

  // auto exif = Image::Metadatas::CreateExif(builder, exif_name, exif_copyright);

  std::vector<flatbuffers::Offset<Image::Metadatas::Exif>> exif_vector;
  exif_vector.push_back(exif);
  auto exif_data = builder.CreateVector(exif_vector);

  auto filename = builder.CreateString("test.png");
  float filesize = 2048.0f;
  float filesize_x = 1920.0f;
  float filesize_y = 1080.0f;
  auto filepath = builder.CreateString("/images/");

  Image::Metadatas::PictureBuilder pic_builder(builder);
  pic_builder.add_file_name(filename);
  pic_builder.add_filesize(filesize);
  pic_builder.add_filesize_x(filesize_x);
  pic_builder.add_filesize_y(filesize_y);
  pic_builder.add_filepath(filepath);
  pic_builder.add_exifdata(exif_data);

  auto fin_image = pic_builder.Finish();

  builder.Finish(fin_image);

  // write file
  uint8_t *buf = builder.GetBufferPointer();
  int size = builder.GetSize();

  std::ofstream ofs(fname, std::ios::binary);
  ofs.write(reinterpret_cast<char *>(buf), size);
  ofs.close();
}

void read_monster_from_file(const std::string &filename)
{
  std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
  std::streamsize size = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  if (ifs.read(buffer.data(), size))
  {

    const Image::Metadatas::Picture *picture = Image::Metadatas::GetPicture(buffer.data());
    std::println("Filename: {}", picture->file_name()->c_str());
    std::println("Filepath: {}", picture->filepath()->c_str());

    auto exifdata = picture->exifdata();
    for (auto it = exifdata->begin(); it != exifdata->end(); ++it)
    {
      std::println("  Exif Name: {}", it->file_name()->c_str());
      std::println("  Exif Copyright: {}", it->copyright()->c_str());
      std::println("  Exif UserComment: {}", it->usercomment() ? it->usercomment()->c_str() : "N/A");
    }
  }
}