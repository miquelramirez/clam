#! /bin/bash

rm -f /tmp/outraytracing

./OfflinePlayer offline_networks/on_the_fly_surround-source_1.clamnetwork ~/acustica/wavs/pluck_1s.wav c.wav l.wav r.wav sl.wav sr.wav
#./OfflinePlayer offline_networks/on_the_fly_surround-source_1.clamnetwork ~/acustica/wavs/3InLanguage.wav c.wav l.wav r.wav sl.wav sr.wav
sox -M l.wav r.wav sl.wav sr.wav c.wav surround.wav
