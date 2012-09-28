// cppgstreamer.cpp : main project file.

#include "stdafx.h"
#include <gstreamer-0.10\gst\gst.h>

using namespace System;

/**
This is the application I put together for streaming audio and video using gstreamer. I have a separate application for receiving the streams.
Warning: when I try to run both apps simultaneously on my four year old Lenovo Y550 laptop it is extremely taxing and I can barely do anything else.
The quality suffers as the computer struggles to keep up. Be wary when trying to run this on older computers. When I run video by itself with only
the receiving video part of the other app going it seems to work fine. The same is true with just the audio. When I combine them, though, it becomes
noticeably lower quality with more skips, just because of my computer's limitations. Once I get a webcam and a microphone for my desktop I will
try running it on that. I expect better results.
Here is a quick explanation of what the pipelines do:

video:
autovideosrc- looks for a video source on the computer and sets it as the source. For my computer I can use dshowvideosrc here, but I set it to autovideosrc
so that it will work with other computers as well.
ffmpegcolorspace- puts the raw data into a form that the video playing part of gstreamer will be able to play.
ffenc_mpeg4- encodes the video as mpeg 4 video. I set the bitrate to 300000. Bitrate is in bits/second, not kilobits per second.
rtpmp4vpay- puts the mp4 video into packets that can be sent over rtp
gstrtpsession- what it sounds like: starts an rtp streaming session. the .send_rtp_sink and .send_rtp_src pads are necessary for using this
to send data over rtp. The receiving app will have a .recv_rtp_sink and .recg_rtp_src to be able to connect to this stream.
udpsink- specifies the port and host where the video is going. The default host is localhost. Here I had to set it to the public IP address of 
the computer that I was sending it to. The default port is 4951.

audio:
autoaudiosrc-automatically detects an audio input device (ie a microphone) and sets it to the audio source.
faac- this encodes the audio in the AAC format.
rtpmp4gpay- mp4g means mp4 general. This takes the AAC audio and puts it into packets to be sent via rtp.
gstrtpsession- this works the same way it does with the video.
udpsink- also works the same as it does with the video. I have to change the port to a be different from the video port. 

Things that can be played around with:
encoders: I initially used theora as my video format, using theoraenc and rtptheorapay. I found mpeg4 was the most stable and least buggy format
to use, with less lag and encoding errors.
bitrate: the number can be adjusted to change the picture quality. Higher = better.
Note that if you change the pipelines here, you will have to change the caps in the receiving app. The caps can be found by going to 
the command line and running gst-launch -v (the pipeline).
*/

int main(int argc, char *argv[])
{
	/* The following lines specify the pipelines that will be used in the streaming process.*/
 char video [] = "autovideosrc !  ffmpegcolorspace !  ffenc_mpeg4 bitrate=300000 ! rtpmp4vpay ! .send_rtp_sink gstrtpsession .send_rtp_src ! udpsink port=5004 host= 136.152.38.6";
 char audio [] = "autoaudiosrc ! faac ! rtpmp4gpay ! .send_rtp_sink gstrtpsession .send_rtp_src ! udpsink host=136.152.38.6 port=5555";
 GError *error = NULL;
  /*Create Gstreamer elements, which when run will provide the streaming functionality.*/
 GstElement *element;
 GstElement *element2;
 
 /*According to the documentation, gst_init "initializes the GStreamer library, setting up internal path lists, registering built-in elements,
and loading standard plugins." This will prepare the program to use gstreamer. gst_parse_launch then parses the pipelines I have created above
and creates objects that implement them.
 */
 gst_init (NULL, NULL);
 element = gst_parse_launch(video, &error);
 gst_init(NULL, NULL);
 element2 = gst_parse_launch(audio, &error);
 
 /*Start streaming.*/
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