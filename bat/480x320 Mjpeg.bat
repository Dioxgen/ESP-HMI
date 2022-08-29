ffmpeg -i %1 -vf "fps=30,scale=-1:320:flags=lanczos,crop=480:in_h:(in_w-320)/2:0" -q:v 9 "%~n1.mjpeg"
pause