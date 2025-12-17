#!/bin/zsh

# This is just a list of common commands used in various automation scripts.

# Convert images to a single vertical image with transparent background
# magick image1.png image2.png -background none -alpha set -append output.png

# Converting mp4 to gif
# ffmpeg -y -i input.mp4 -vf "scale=iw:-1:flags=lanczos,palettegen=stats_mode=full" palette.png
# ffmpeg -y -i input.mp4 -i palette.png -filter_complex "scale=iw:-1:flags=lanczos[x];[x][1:v]paletteuse=dither=sierra2_4a" -loop 0 output.gif
# rm palette.png

# Convert to DDS from png
# General textures are already srgb so no need to convert color space.
# texconv -f R8G8B8A8_UNORM -m 1 -ft dds -y input.png

# Shader compilation
# Release
# dxc -T vs_6_0 -E VS -Fo "sprite_atlas_vs.cso" -O3 -I . "sprite_atlas.hlsl"
# dxc -T ps_6_0 -E PS -Fo "sprite_atlas_ps.cso" -O3 -I . "sprite_atlas.hlsl"
# Debug
# dxc -T vs_6_0 -E VS -Fo "sprite_atlas_vs.cso" -Zi -Qembed_debug -Od -I . "sprite_atlas.hlsl"
# dxc -T ps_6_0 -E PS -Fo "sprite_atlas_ps.cso" -Zi -Qembed_debug -Od -I . "sprite_atlas.hlsl" 
