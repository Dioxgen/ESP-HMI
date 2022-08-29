ffmpeg -i %1 -f u16le -acodec pcm_u16le -ar 44100 -ac 1 "%~n1.pcm"
pause