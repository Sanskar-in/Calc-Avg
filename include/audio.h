// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/Sanskar-in/Calc-Avg"
//
// Follow Sanskar or Creator on:-
// GitHub:- "https://www.github.com/Sanskar-in"
// YouTube:- "https://youtube.com/@Sanskar-in"
// LinkedIn:- "https://www.linkedin.com/in/sanskar-in/"
// X (Formally known as Twitter):- "https://www.x.com/Sanskar_in"
// Need support:- supportramsandesh@gmail.com
// Want to make your own projects, apps, and games via Sanskar:- sanskaryadavfrom2012to2026@gmail.com
// =============================================================================

#ifndef AUDIO_H
#define AUDIO_H

/**
 * Mathematically maps data points to physical sound frequencies and plays
 * them through the hardware PC speakers.
 * @param data_points Array of data points.
 * @param count Number of data points.
 */
void play_data_sonification(double data_points[], int count);
void start_microphone_stream_thread();
char* get_latest_mic_base64();

#endif // AUDIO_H
