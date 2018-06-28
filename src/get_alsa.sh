#!/bin/sh

set -e

# Load common properties and functions in the current script.
. ./common.sh

echo "*** GET ALSA BEGIN ***"

# Read the 'ALSA_SOURCE_URL' property from '.config'.
DOWNLOAD_URL=`read_property ALSA_SOURCE_URL`

# Grab everything after the last '/' character.
ARCHIVE_FILE=${DOWNLOAD_URL##*/}

# Download Alsa source archive in the 'source' directory.
download_source $DOWNLOAD_URL $SOURCE_DIR/$ARCHIVE_FILE

# Extract the Alsa sources in the 'work/alsa' directory.
extract_source $SOURCE_DIR/$ARCHIVE_FILE alsa

# We go back to the main MLL source folder.
cd $SRC_DIR

echo "*** GET ALSA END ***"
