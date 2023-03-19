#!/bin/bash

set -e

BASE_DIR=$(dirname "$0")

rm -rf $BASE_DIR/dist
mkdir $BASE_DIR/dist
cp -R $BASE_DIR/../common/assets $BASE_DIR/dist/assets

gcc \
  -I$BASE_DIR/../../SDL/include \
  -I$BASE_DIR/../../SDL_image \
  -I$BASE_DIR/../common/include \
  -I$BASE_DIR/include \
  -L$BASE_DIR/../../SDL/build/.libs \
  -L$BASE_DIR/../../SDL_image/.libs \
  -lcurl -lSDL2 -lSDL2_image \
  -Wl,-rpath,. \
  $BASE_DIR/../common/src/main.c \
  $BASE_DIR/../common/src/core/navigation.c \
  $BASE_DIR/../common/src/core/positioning.c \
  $BASE_DIR/../common/src/core/resources.c \
  $BASE_DIR/../common/src/core/audio.c \
  $BASE_DIR/../common/src/screens/world/components/options-button.c \
  $BASE_DIR/../common/src/screens/world/components/map.c \
  $BASE_DIR/../common/src/screens/world/components/main-character.c \
  $BASE_DIR/../common/src/screens/world/components/mobile-motion-controller.c \
  $BASE_DIR/../common/src/screens/world/world.c \
  $BASE_DIR/../common/src/screens/home.c \
  $BASE_DIR/src/core/log.c \
  -o $BASE_DIR/dist/veysarchives1

cp $BASE_DIR/../../SDL/build/.libs/libSDL2-2.0.so.0 $BASE_DIR/dist
cp $BASE_DIR/../../SDL_image/.libs/libSDL2_image-2.0.so.0 $BASE_DIR/dist
