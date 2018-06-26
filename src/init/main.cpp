#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <alsa/asoundlib.h>

int main ()
{
   printf ("Hello, world!\n");

   int card_idx = -1;
   int errc = snd_card_next (&card_idx);
   if (errc != 0) printf ("error: snd_card_next\n");
   if (card_idx == -1) printf ("error: card_idx\n");

   char * name_0 = 0;
   errc = snd_card_get_name (card_idx, &name_0);
   if (errc != 0) printf ("error: snd_card_get_name\n");
   char * longname_0 = 0;
   errc = snd_card_get_longname (card_idx, &longname_0);
   if (errc != 0) printf ("error: snd_card_get_longname\n");

   printf ("card %d: %s | %s\n", card_idx, name_0, longname_0);

   snd_pcm_t * pcm_handle = 0;

   unsigned int err = snd_pcm_open (&pcm_handle, "hw:0,7", SND_PCM_STREAM_PLAYBACK, 0);
   if (err < 0) printf ("error: snd_pcm_open\n");

   snd_pcm_hw_params_t * params = 0;
   snd_pcm_hw_params_alloca (&params);
   snd_pcm_hw_params_any (pcm_handle, params);

   err = snd_pcm_hw_params_set_access (pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
   if (err < 0) printf ("error: snd_pcm_hw_params_set_access\n");

   err = snd_pcm_hw_params_set_format (pcm_handle, params, SND_PCM_FORMAT_S16_LE);
   if (err < 0) printf ("error: snd_pcm_hw_params_set_format\n");

   err = snd_pcm_hw_params_set_channels (pcm_handle, params, 2);
   if (err < 0) printf ("error: snd_pcm_hw_params_set_channels\n");

   unsigned int rate = 44100;
   err = snd_pcm_hw_params_set_rate_near (pcm_handle, params, &rate, 0);
   if (err < 0) printf ("error: snd_pcm_hw_params_set_rate_near\n");

   err = snd_pcm_hw_params(pcm_handle, params);
   if (err < 0) printf ("error: snd_pcm_hw_params\n");

   printf ("name: '%s'\n", snd_pcm_name (pcm_handle));
   printf ("state: %s\n", snd_pcm_state_name (snd_pcm_state (pcm_handle)));

   unsigned int channels;
   snd_pcm_hw_params_get_channels (params, &channels);
   printf ("channels: '%i'\n", channels);

   unsigned int rate_current;
   snd_pcm_hw_params_get_rate (params, &rate_current, 0);
   printf ("rate: '%d'\n", rate_current);

   snd_pcm_uframes_t frames;
   snd_pcm_hw_params_get_period_size (params, &frames, 0);

   int buf_size = frames * channels;
   int16_t * buf_ptr = (int16_t *) malloc (buf_size * sizeof (int16_t));

   unsigned int period_us;
   snd_pcm_hw_params_get_period_time(params, &period_us, 0);

   float phase = 0.f;
   const float phase_step = 440.f * 6.28318530717959f / 44100.f;

   printf ("Playing A440 tone for 2 seconds\n");

   for (int i = 0 ; i < 2000000 / period_us ; ++i)
   {
      for (snd_pcm_uframes_t j = 0 ; j < frames ; ++j)
      {
         float val = sinf (phase);
         phase += phase_step;
         int16_t val_d = int16_t (val * 32767.f);

         buf_ptr [2 * j + 0] = val_d;
         buf_ptr [2 * j + 1] = val_d;
      }

      err = snd_pcm_writei (pcm_handle, buf_ptr, frames);
      if (err == -EPIPE)
      {
         printf ("XRUN\n");
         snd_pcm_prepare (pcm_handle);
      }
      else if (err < 0)
      {
         printf ("error: snd_pcm_writei\n");
      }
   }

   snd_pcm_drain (pcm_handle);
   snd_pcm_close (pcm_handle);
   free (buf_ptr);

   return 0;
}
