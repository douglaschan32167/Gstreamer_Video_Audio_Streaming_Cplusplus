// cppreceiveav.cpp : main project file.

/* Note that this project relies on the directories where the gstreamer files are in my computer. For other computers, you will have to change
the include directories and library directories. There may be a way to bundle gstreamer with this project and thus make it fully functional
without having to install anything, but I have not found out how to do that yet. This app receives the video and audio, decodes them, and plays
them. You will have to change the ip address manually as of right now. I've found that it will only listen on certain ports; 5004 and 5555 work,
but 5000 and 5005 do not. If you are going to try to change the ports, don't be surprised if you come across some unhelpful errors. 

*/

#include "stdafx.h"
#include <gstreamer-0.10\gst\gst.h>

using namespace System;

int main(array<System::String ^> ^args)
{
	//GstCaps *videoCaps;
	//gchar *thecaps;
	//thecaps = "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)MP4V-ES, profile-level-id=(string)1, config=(string)000001b001000001b58913000001000000012000c48d8800f514043c1463000001b24c61766335322e36362e30, ssrc=(guint)4078450447, payload=(int)96, clock-base=(guint)1605988047, seqnum-base=(guint)18279";
	//videoCaps = gst_caps_from_string(thecaps); 
	char video [] = "udpsrc uri=udp://10.10.64.71:5004 caps=\"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)MP4V-ES, profile-level-id=(string)1, config=(string)000001b001000001b58913000001000000012000c48d8800f514043c1463000001b24c61766335322e36362e30, ssrc=(guint)4078450447, payload=(int)96, clock-base=(guint)1605988047, seqnum-base=(guint)18279\"; ! .recv_rtp_sink gstrtpsession .recv_rtp_src ! gstrtpjitterbuffer ! rtpmp4vdepay ! ffdec_mpeg4 ! ffmpegcolorspace ! autovideosink ";
	//the next line is experimental. the one above works on its own
	//char video [] = "udpsrc uri=udp://10.10.64.71:5004 caps=videoCaps; ! .recv_rtp_sink gstrtpsession .recv_rtp_src ! gstrtpjitterbuffer ! rtpmp4vdepay ! ffdec_mpeg4 ! ffmpegcolorspace ! autovideosink ";
	 char audio [] = "udpsrc uri=udp://10.10.64.71:5555 caps=\"application/x-rtp, media=(string)audio, clock-rate=(int)44100, encoding-name=(string)MPEG4-GENERIC, encoding-params=(string)1, streamtype=(string)5, profile-level-id=(string)2,mode=(string)AAC-hbr, config=(string)1208, sizelength=(string)13, indexlength=(string)3, indexdeltalength=(string)3, payload=(int)96, ssrc=(guint)914250985, clock-base=(guint)1675232974, seqnum-base=(guint)17778\" ! .recv_rtp_sink gstrtpsession .recv_rtp_src ! queue ! rtpmp4gdepay ! faad ! audioconvert ! autoaudiosink";
	GError *error = NULL;
	GstElement *videoelement;
	GstElement *audioelement;
 
	 gst_init (NULL, NULL);
	videoelement = gst_parse_launch(video, &error);
	audioelement = gst_parse_launch(audio, &error);
 
	gst_element_set_state (videoelement, GST_STATE_PLAYING);
	gst_element_set_state(audioelement, GST_STATE_PLAYING);
 
	if (!videoelement) {
		fprintf (stderr, "Parse error: %s\n", error->message);
		exit(1);
 }
	if (!audioelement) {
		fprintf (stderr, "Parse error: %s\n", error->message);
		exit(1);
	}
 
 while(1){}
 
 return 1;
}
