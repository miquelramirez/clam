#!/usr/bin/python


localizations = [
	# elevation, azimuth
	(0, 30),
]

wavs = [
	"pinknoise.wav"
]

algorithms = [
	# name, n intermediate channels, encoder, decoder
	("hoa1", 4, "hoa1roomsimulator.clamnetwork", "hoa1to15decoder.clamnetwork"),
	("hoa2", 9, "hoa2roomsimulator.clamnetwork", "hoa2to15decoder.clamnetwork"),
	("hoa3", 16, "hoa3roomsimulator.clamnetwork", "hoa3to15decoder.clamnetwork"),
]

absortions = [
	# TODO: different z?
	("anechoich", ""),
	("littlereverb", ""),
	("fullreverb", ""),
]


print "Generate choreography"
for localization in localizations :
	# TODO dump several lines in the coreo
	print "Dumping lines for localization", localization

for space_name, z in absortions :
	print "Generate geometry for", space_name
	# TODO:
	for algorithm_name, channels, encoder, decoder in algorithms :
		for wav in wavs :
			print "Generating", algorithm_name, "for", wav, "with absortion", z
			print "OfflinePlayer %(encoder)s %(wav)s -o -c %(channels)s encoded_%(algorithm_name)s_%(space_name)s_%(wav)s"%dict(globals())
			print "OfflinePlayer %(decoder)s decoded_%(algorithm_name)s_%(space_name)s_%(wav)s -o -c 15 decoded_%(algorithm_name)s_%(space_name)s_%(wav)s"%dict(globals())



