#!/bin/zsh

# This is just a list of common commands used in various automation scripts.

# Convert images to a single vertical image with transparent background
# magick image1.png image2.png -background none -alpha set -append output.png

# Converting mp4 to gif
# ffmpeg -y -i input.mp4 -vf "scale=iw:-1:flags=lanczos,palettegen=stats_mode=full" palette.png
# ffmpeg -y -i input.mp4 -i palette.png -filter_complex "scale=iw:-1:flags=lanczos[x];[x][1:v]paletteuse=dither=sierra2_4a" -loop 0 output.gif
# rm palette.png
