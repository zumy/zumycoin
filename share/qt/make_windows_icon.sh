#!/bin/bash
# create multiresolution windows icon
#mainnet
ICON_SRC=../../src/qt/res/icons/zumy.png
ICON_DST=../../src/qt/res/icons/zumy.ico
convert ${ICON_SRC} -resize 16x16 zumy-16.png
convert ${ICON_SRC} -resize 32x32 zumy-32.png
convert ${ICON_SRC} -resize 48x48 zumy-48.png
convert zumy-16.png zumy-32.png zumy-48.png ${ICON_DST}
#testnet
ICON_SRC=../../src/qt/res/icons/zumy_testnet.png
ICON_DST=../../src/qt/res/icons/zumy_testnet.ico
convert ${ICON_SRC} -resize 16x16 zumy-16.png
convert ${ICON_SRC} -resize 32x32 zumy-32.png
convert ${ICON_SRC} -resize 48x48 zumy-48.png
convert zumy-16.png zumy-32.png zumy-48.png ${ICON_DST}
rm zumy-16.png zumy-32.png zumy-48.png
