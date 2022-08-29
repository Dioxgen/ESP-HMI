ffmpeg -i %1 -vf subtitles="%~n1.ass" -vcodec libx264 "%~n1.mp4"
pause