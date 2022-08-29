ffmpeg -i %1 -vf "fps=30,scale=-1:240" -q:v 9 "%~n1.mp4"
pause