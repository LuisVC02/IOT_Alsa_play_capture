#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <alsa/asoundlib.h>

#define FRAMES   768
#define CHANNELS 2

int main(int args, char * argv[])
{
    snd_pcm_t * pcm;
    snd_pcm_hw_params_t * params;
    FILE * fd = fopen(argv[1], "R");

    int ret = snd_pcm_open(&pcm, "hw:1", SND_PCM_STREAM_PLAYBACK, 0);

    snd_pcm_hw_params_alloca(&params);
    ret = snd_pcm_hw_params_any(pcm, params);

    ret = snd_pcm_hw_params_set_access(pcm, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    ret = snd_pcm_hw_params_set_format(pcm, params, SND_PCM_FORMAT_S32_LE);
    ret = snd_pcm_hw_params_set_channels(pcm, params, CHANNELS);
    int rate = 48000;
    ret = snd_pcm_hw_params_set_rate_near(pcm, params, &rate, 0);
    ret = snd_pcm_hw_params(pcm, params);

    int size = FRAMES * CHANNELS * sizeof(uint32_t);
    char * buffer = malloc(size);

    while(1)
    {
        int n_bytes = fread(buffer, 1, size, fd);
        snd_pcm_sframes_t frames = snd_pcm_writei(pcm, buffer, FRAMES);
        fflush(fd);
    }

    return 0;
}