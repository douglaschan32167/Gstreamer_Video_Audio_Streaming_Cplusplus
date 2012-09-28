// cppreceiveav.cpp : main project file.

/* Note that this project relies on the directories where the gstreamer files are in my computer. For other computers, you will have to change
the include directories and library directories. There may be a way to bundle gstreamer with this project and thus make it fully functional
without having to install anything, but I have not found out how to do that yet. This app receives the video and audio, decodes them, and plays
them. You will have to change the ip address manually as of right now. I've found that it will only listen on certain ports; 5004 and 5555 work,
but 5000 and 5005 do not. If you are going to try to change the ports, don't be surprised if you come across some unhelpful errors. 
Please also note that you will have change the IP address to match the one on your computer.

This app is designed so that it will go with the other streaming app that I wrote. 
Here is a short overview of the pipelines:
video:
udpsrc- the RTP stream will come from here. Set the URI equal to udp://<your private ip address found from ipconfig>:<port number that the streaming app specified>
the caps basically tell gstreamer what the data coming in is. To find them, go to command line and do "gst-launch -v (the sending pipeline)
gstrtpsession- this starts and rtp session. The .recv_rtp_sink and .recv_rtp_src help connect to to the rtp session in the sending app.
gstrtpjitterbuffer-I originally did not have this, but the quality is better with this. This is a buffer that receives the data coming in and holds
it for a set amount of time before playing it, so that late packets have a chance to catch back up and be reordered into the feed instead of 
just being dropped. This made the picture more stable. It has a latency property that sets the time in nanoseconds it should wait for late
packets. The default is 200. There is a trade-off between lag generated this way and picture quality. I found the benefit of making latency
more than 200 nanoseconds did not outweigh the lag, so I just kept it at 200ns.
rtpmp4vdepay- breaks down the rtp packets back into video data.
ffdec_mpeg4- decodes the mpeg4 video.
ffmpegcolorspace- puts the resulting decoded video into a form that the autovideosink can play.
autovideosink-plays the video.

audio:
udpsrc and gstrtpsession work the same way as they do in video.
rtpmp4gdepay- extracts audio from the rtp packets.
faad- decodes the AAC audio.
audioconvert- converts the audio into different formats.
autoaudiosink-plays the audio
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
	/*I set the two pipelines that dictate what gstreamer will do to receive the audio and video.*/
	char video [] = "udpsrc uri=udp://10.10.64.71:5004 caps=\"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)MP4V-ES, profile-level-id=(string)1, config=(string)000001b001000001b58913000001000000012000c48d8800f514043c1463000001b24c61766335322e36362e30, ssrc=(guint)4078450447, payload=(int)96, clock-base=(guint)1605988047, seqnum-base=(guint)18279\"; ! .recv_rtp_sink gstrtpsession .recv_rtp_src ! gstrtpjitterbuffer ! rtpmp4vdepay ! ffdec_mpeg4 ! ffmpegcolorspace ! autovideosink ";
	//the next line is experimental. the one above works on its own
	//char video [] = "udpsrc uri=udp://10.10.64.71:5004 caps=videoCaps; ! .recv_rtp_sink gstrtpsession .recv_rtp_src ! gstrtpjitterbuffer ! rtpmp4vdepay ! ffdec_mpeg4 ! ffmpegcolorspace ! autovideosink ";
	 char audio [] = "udpsrc uri=udp://10.10.64.71:5555 caps=\"application/x-rtp, media=(string)audio, clock-rate=(int)44100, encoding-name=(string)MPEG4-GENERIC, encoding-params=(string)1, streamtype=(string)5, profile-level-id=(string)2,mode=(string)AAC-hbr, config=(string)1208, sizelength=(string)13, indexlength=(string)3, indexdeltalength=(string)3, payload=(int)96, ssrc=(guint)914250985, clock-base=(guint)1675232974, seqnum-base=(guint)17778\" ! .recv_rtp_sink gstrtpsession .recv_rtp_src ! queue ! rtpmp4gdepay ! faad ! audioconvert ! autoaudiosink";
	GError *error = NULL;
	GstElement *videoelement;
	GstElement *audioelement;
 
	/*Create GstElements according to the pipelines.*/
	 gst_init (NULL, NULL);
	videoelement = gst_parse_launch(video, &error);
	audioelement = gst_parse_launch(audio, &error);
 
	/*Start playing audio and video.*/
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
