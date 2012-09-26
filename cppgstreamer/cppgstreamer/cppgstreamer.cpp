// cppgstreamer.cpp : main project file.

#include "stdafx.h"
#include <gstreamer-0.10\gst\gst.h>

using namespace System;

int main(int argc, char *argv[])
{
 char str [] = "autovideosrc !  ffmpegcolorspace !  ffenc_mpeg4 bitrate=300000 ! rtpmp4vpay ! .send_rtp_sink gstrtpsession .send_rtp_src ! udpsink port=5004 host= 136.152.38.6";
 char str2 [] = "autoaudiosrc ! faac ! rtpmp4gpay ! .send_rtp_sink gstrtpsession .send_rtp_src ! udpsink host=136.152.38.6 port=5555";
 GError *error = NULL;
 GstElement *element;
 GstElement *element2;
 
 gst_init (NULL, NULL);
 element = gst_parse_launch(str, &error);
 gst_init(NULL, NULL);
 element2 = gst_parse_launch(str2, &error);
 
 gst_element_set_state (element, GST_STATE_PLAYING);
 gst_element_set_state(element2, GST_STATE_PLAYING);

 if (!element) {
   fprintf (stderr, "Parse error: %s\n", error->message);
   exit(1);
 }
 if (!element2) {
   fprintf (stderr, "Parse error: %s\n", error->message);
   exit(1);
 }
 
 while(1){}
 
 return 1;
}

//array<System::String ^> ^args