#pragma once
/*****************************************************************************
 * 
 * Copyright 2016 Varol Okan. All rights reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 ****************************************************************************/

#ifndef __METADATA_UTILS_H__
#define __METADATA_UTILS_H__

#include <stdint.h>
#include <string>
#include <fstream>
#include <map>

#include "mxml.h"

#include "parser.h"
#include "mpeg/constants.h"

// Utilities for examining/injecting spatial media metadata in MP4/MOV files."""

static const char *MPEG_FILE_EXTENSIONS[2] = { ".mp4", ".mov" };

static const uint8_t SPHERICAL_UUID_ID[] = {0xff, 0xcc, 0x82, 0x63, 0xf8, 0x55, 0x4a, 0x93, 0x88, 0x14, 0x58, 0x7a, 0x02, 0x52, 0x1f, 0xdd };
//    "\xff\xcc\x82\x63\xf8\x55\x4a\x93\x88\x14\x58\x7a\x02\x52\x1f\xdd")

// # XML contents.
static std::string RDF_PREFIX = " xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" ";

static std::string SPHERICAL_XML_HEADER = "<?xml version=\"1.0\"?>"\
    "<rdf:SphericalVideo\n"\
    "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"\
    "xmlns:GSpherical=\"http://ns.google.com/videos/1.0/spherical/\">";

static std::string SPHERICAL_XML_CONTENTS = "<GSpherical:Spherical>true</GSpherical:Spherical>"\
    "<GSpherical:Stitched>true</GSpherical:Stitched>"\
    "<GSpherical:StitchingSoftware>"\
    "Spherical Metadata Tool"\
    "</GSpherical:StitchingSoftware>"\
    "<GSpherical:ProjectionType>equirectangular</GSpherical:ProjectionType>";

static std::string SPHERICAL_XML_CONTENTS_TOP_BOTTOM = "<GSpherical:StereoMode>top-bottom</GSpherical:StereoMode>";
static std::string SPHERICAL_XML_CONTENTS_LEFT_RIGHT = "<GSpherical:StereoMode>left-right</GSpherical:StereoMode>";

// Parameter order matches that of the crop option.
static std::string SPHERICAL_XML_CONTENTS_CROP_FORMAT = "<GSpherical:CroppedAreaImageWidthPixels>{0}"\
    "</GSpherical:CroppedAreaImageWidthPixels>"\
    "<GSpherical:CroppedAreaImageHeightPixels>{1}"\
    "</GSpherical:CroppedAreaImageHeightPixels>"\
    "<GSpherical:FullPanoWidthPixels>{2}</GSpherical:FullPanoWidthPixels>"\
    "<GSpherical:FullPanoHeightPixels>{3}</GSpherical:FullPanoHeightPixels>"\
    "<GSpherical:CroppedAreaLeftPixels>{4}</GSpherical:CroppedAreaLeftPixels>"\
    "<GSpherical:CroppedAreaTopPixels>{5}</GSpherical:CroppedAreaTopPixels>";

static std::string SPHERICAL_XML_FOOTER = "</rdf:SphericalVideo>";
static std::string SPHERICAL_PREFIX = "{http://ns.google.com/videos/1.0/spherical/}";

//struct SPATIAL_AUDIO_DEFAULT_METADATA {
//  SPATIAL_AUDIO_DEFAULT_METADATA ( )  {
//    ambisonic_order = 1;
//    ambisonic_type  = "periphonic";
//    ambisonic_channel_ordering = "ACN";
//    ambisonic_normalization    = "SN3D";
//    for ( uint32_t t=0; t<4; t++ )
//      channel_map[t] = t;
//  };
//  uint32_t ambisonic_order;
//  std::string ambisonic_type;
//  std::string ambisonic_channel_ordering;
//  std::string ambisonic_normalization;
//  uint32_t channel_map[4];
//} static g_DefAudioMetadata;
static AudioMetadata g_DefAudioMetadata;

class Box;
class SA3DBox;
class Mpeg4Container;

class Metadata
{
  public:
    Metadata ( );
    virtual ~Metadata ( );

    void setVideoXML ( std::string &, mxml_node_t * );
    void setVideoXML ( std::string & );
    void setAudio ( void * );

  private:
    std::string m_strVideoXML;
    std::map<std::string, mxml_node_t *> m_mapVideo;
    void *m_pAudio;
};


class ParsedMetadata : public Metadata
{
  public:
    ParsedMetadata ( );
    virtual ~ParsedMetadata ( );


//  private:
    uint32_t m_iNumAudioChannels;
};


class Utils
{
  public:
    Utils ( );
    virtual ~Utils ( );

    Box *spherical_uuid ( std::string & );
    bool mpeg4_add_spherical      ( Mpeg4Container *, std::fstream &, std::string & );
    bool mpeg4_add_spatial_audio  ( Mpeg4Container *, std::fstream &, AudioMetadata * );
    bool mpeg4_add_audio_metadata ( Mpeg4Container *, std::fstream &, AudioMetadata * );
    bool inject_spatial_audio_atom( std::fstream &, Box *, AudioMetadata * );
    std::map<std::string, std::string> parse_spherical_xml ( uint8_t * ); // return sphericalDictionary
    Metadata *parse_spherical_mpeg4 ( Mpeg4Container *,std::fstream & ); // return metadata
    void parse_mpeg4     ( std::string & );
    void inject_mpeg4    ( std::string &, std::string &, Box * );
    void parse_metadata  ( std::string & );
    void inject_metadata ( std::string &, std::string &, Metadata * );
    std::string &generate_spherical_xml ( SpatialMedia::Parser::enMode, int * );
    void get_descriptor_length  ( std::fstream & );
    int32_t  get_expected_num_audio_components ( std::string &, uint32_t );
    int32_t  get_num_audio_channels ( Box *, std::fstream & );
    uint32_t get_sample_description_num_channels ( Box *, std::fstream & ); 
    int32_t  get_aac_num_channels ( Box *, std::fstream & );
    uint32_t get_num_audio_tracks ( Mpeg4Container *, std::fstream & );

  private:
    std::string m_strSphericalXML;
};

#endif // __METADATA_UTILS_H__

