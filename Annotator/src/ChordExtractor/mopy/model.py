
# ===================================================================
# = model.py - Core and External Classes of the Music Ontology 
# =            Generated automatically on Thu Aug 30 10:24:06 2007
# ===================================================================


from mopy.PropertySet import PropertySet, protector


def objToStr(c):
	s = "-- "+c.shortname
	if c.URI != None :
		s+=" @ "+str(c.URI)
	s+=" --\n"
	for p in c._props.keys():
		for v in c._props[p]:
			s+=c._props[p].shortname + " : "
			if isinstance(v, c._props[p].Lits):
				s+=str(v)
			else:
				s+=str(type(v))
				if hasattr(v,"URI"):
					s+=" @ "+v.URI
			s +="\n"
	return s

# ======================== Property Docstrings ====================== 

propDocs = {}
propDocs["gender"]=\
"""The gender of this Agent (typically but not necessarily 'male' or 'female')."""
propDocs["preview"]=\
"""Relates a musical manifestation to a musical item (this album, and my particular cd), which holds
		a preview of the manifestation (eg. one track for an album, or a snippet for a track)"""
propDocs["sha1"]=\
"""A sha1sum hash, in hex."""
propDocs["discography"]=\
"""Used to links an artist to an online discography of their musical works. The discography should provide a summary of each released musical work of the artist."""
propDocs["encoding"]=\
"""Method used to convert analog electronic signals into digital format such as "MP3 CBR @ 128kbps", "OGG @ 160kbps", "FLAC", etc."""
propDocs["surname"]=\
"""The surname of some person."""
propDocs["records"]=\
"""This is the inverse of the shortcut property recordedAs, allowing to relate directly a performance
		to a signal."""
propDocs["atDateTime"]=\
"""Place a time point on the universal time line by using xsd:dateTime."""
propDocs["made"]=\
"""Something that was made by this agent.
		Relates an agent to a manifestation he contributed to create."""
propDocs["schoolHomepage"]=\
"""A homepage of a school attended by the person."""
propDocs["thumbnail"]=\
"""A derived thumbnail image."""
propDocs["beginsAtDuration"]=\
"""Links an interval on a semi-infinite continuous time line to its
		start point, addressed using xsd:duration (duration between 0 and the
		start point)"""
propDocs["supporting_musician"]=\
"""Used to relate an artist doing long-time instrumental or vocal support for another artist."""
propDocs["produced_score"]=\
"""Associates an arrangement event to a score product (score here does not refer to a published score, but more
		an abstract arrangement of a particular work)."""
propDocs["similar_to"]=\
"""A similarity relationships between two objects (so far, either an agent, a signal or a genre, but
		this could grow).
		This relationship is pretty general and doesn't make any assumptions on how the similarity claim
		was derived.
		Such similarity statements can come from a range of different sources (Musicbrainz similarities between
		artists, or coming from some automatic content analysis).
		However, the origin of such statements should be kept using a named graph approach - and ultimately, the 
		documents providing such statements should attach some metadata to themselves (confidence of the claim, etc.)."""
propDocs["release_type"]=\
"""Relates a musical manifestation to its release type (interview, spoken word, album, ...)"""
propDocs["track"]=\
"""Indicates a part of a musical manifestation - in this particular case, a track."""
propDocs["discogs"]=\
"""Used to link a musical work or the expression of a musical work, an artist or a corporate body to to its corresponding Discogs page."""
propDocs["musicmoz"]=\
"""Used to link an artist, a musical work or the expression of a musical work to its corresponding MusicMoz page."""
propDocs["mbox_sha1sum"]=\
"""The sha1sum of the URI of an Internet mailbox associated with exactly one owner, the  first owner of the mailbox."""
propDocs["conductor"]=\
"""Relates a performance to the conductor involved"""
propDocs["publishedAs"]=""
propDocs["remaster_of"]=\
"""This relates two musical work or the expression of a musical work, where one is a remaster of the other. 
    
A remaster is a new version made for release from source recordings that were earlier released separately. This is usually done to improve the audio quality or adjust for more modern playback equipment. The process generally doesn't involve changing the music in any artistically important way. It may, however, result in tracks that are a few seconds longer or shorter."""
propDocs["engineer"]=\
"""Relates a performance or a recording to the engineer involved"""
propDocs["creator"]=\
"""An entity primarily responsible for making 
		        the resource."""
propDocs["available_as"]=\
"""Relates a musical manifestation to a musical item (this album, and my particular cd). By using
		this property, there is no assumption on wether the full content is available on the linked item.
		To be explicit about this, you can use a sub-property, such as mo:item (the full manifestation
		is available on that item) or mo:preview (only a part of the manifestation is available on
		that item).

		This is a subproperty of frbr:examplar."""
propDocs["instrument"]=\
"""Relates a performance to a musical instrument involved"""
propDocs["producesWork"]=""
propDocs["movement_number"]=\
"""Indicates the position of a movement in a musical work."""
propDocs["remix_of"]=\
"""Used to relate the remix of a musical work in a substantially altered version produced by mixing together individual tracks or segments of an original musical source work."""
propDocs["compilation_of"]=\
"""Indicates that a musical manifestation is a compilation of several Signals."""
propDocs["theme"]=\
"""A theme."""
propDocs["sampledVersionOf"]=""
propDocs["releaseStatus"]=""
propDocs["download"]=\
"""This property can be used to link from a person to the website where they make their works available, or from
                a manifestation (a track or an album, for example) to a web page where it is available for
                download.
		
		It is better to use one of the three sub-properties instead of this one in order to specify wether the
		content can be accessed for free (mo:freedownload), if it is just free preview material (mo:previewdownload), or
		if it can be accessed for some money (mo:paiddownload) (this includes links to the Amazon store, for example).

                This property MUST be used only if the content is just available through a web page (holding, for example
                a Flash application) - it is better to link to actual content directly through the use of mo:availableAs and
                mo:Stream, mo:Torrent or mo:ED2K, etc. Therefore, Semantic Web user agents that don't know how to read HTML and even
                less to rip streams from Flash applications can still access the audio content."""
propDocs["atDuration"]=\
"""Place a time point on an abstract time line by expressing its distance to
		the point 0, through xsd:duration (example: this instant is at 2s after 0 --> T2S)"""
propDocs["producedIn"]=\
"""Associates the product of an event to the event itself"""
propDocs["release_status"]=\
"""Relates a musical manifestation to its release status (bootleg, ...)"""
propDocs["usedInRecording"]=""
propDocs["tempo"]=\
"""Rate of speed or pace of music. Tempo markings are traditionally given in Italian; 
		common markings include: grave (solemn; very, very slow); largo (broad; very slow); 
		adagio (quite slow); andante (a walking pace); moderato (moderate); allegro (fast; cheerful); 
		vivace (lively); presto (very fast); accelerando (getting faster); ritardando (getting slower); 
		and a tempo (in time; returning to the original pace)."""
propDocs["hasSubEvent"]=\
"""Allows to link an event to a sub-event. A sub-event might be an event split by time,
		space, agents, factors... This property can be used to express things such as "during
		this performance, this person was playing this instrument at this particular time", through 
		the creation of a sub-event, occuring at this given time, and having as agent the person and
		as factor the instrument"""
propDocs["composed_in"]=\
"""Associates a MusicalWork to the Composition event pertaining
		to its creation. For example, I might use this property to associate
		the Magic Flute to its composition event, occuring during 1782 and having as
		a mo:composer Mozart."""
propDocs["mashup_of"]=\
"""Indicates that musical works or the expressions of a musical work were mashed up on this album or track. 
    
This means that two musical works or the expressions of a musical work by different artists are mixed together, over each other, or otherwise combined into a single musical work (usually by a third artist, the remixer)."""
propDocs["olga"]=\
"""Used to link a track to a tabulature file for track in the On-Line Guitar Archive."""
propDocs["sell_item"]=\
"""A person, a group of person or an organization selling an exemplar of a single manifestation."""
propDocs["recording_of"]=\
"""Associates a Recording event to a physical Sound being recorded.
                For example, I might use this property to
                associate a given recording, done using my cell phone, to the 
		sound produced by a particular performance of the magic flute."""
propDocs["manifestation"]=\
"""Links a MusicalExpression to its manifestations, subproperty of frbr:embodiment"""
propDocs["freedownload"]=""
propDocs["maker"]=\
"""An agent that  made this thing.
		Relates a manifestation to an agent who contributed to create it.
		This property might be used for weak assertion of such a relationship. In case we want
		to attach a more concrete role (this agent performed, or was the composer, etc.), we must 
		use mo:Performer, mo:MusicalWork/mo:Composition, etc. This indeed allows to specify where a 
		particular agent took part in the actual workflow."""
propDocs["remixer"]=\
"""Used to relate a musical work or the expression of a musical work to an artist who remixed it. 
    
This involves taking just one other musical work and using audio editing to make it sound like a significantly different, but usually still recognisable, song. It can be used to link an artist to a single song that they remixed, or, if they remixed an entire musical work."""
propDocs["isPrimaryTopicOf"]=\
"""A document that this thing is the primary topic of."""
propDocs["page"]=\
"""A page or document about this thing."""
propDocs["holdsAccount"]=\
"""Indicates an account held by this agent."""
propDocs["produced_work"]=\
"""Associates a composition event to the produced MusicalWork. For example,
                this property could link the event corresponding to the composition of the
                Magic Flute in 1782 to the Magic Flute musical work itself. This musical work
		can then be used in particular performances."""
propDocs["genre"]=\
"""Associates an event (like a performance or a recording) to a particular musical genre.
		Further version of this property may also include works and scores in the domain."""
propDocs["imdb"]=\
"""Used to link an artist, a musical work or the expression of a musical work to their equivalent page on IMDb, the InternetMovieDatabase."""
propDocs["beginsAtDateTime"]=\
"""Links an interval on a physical timeline to its actual start point,
		expressed using xsd:dateTime"""
propDocs["has_movement"]=\
"""Indicates that a musical work has movements"""
propDocs["plan"]=\
"""A .plan comment, in the tradition of finger and '.plan' files."""
propDocs["isFactorOf"]=\
"""Associates a factor to an event"""
propDocs["dnaChecksum"]=\
"""A checksum for the DNA of some thing. Joke."""
propDocs["exchange_item"]=\
"""A person, a group of person or an organization exchanging an exemplar of a single manifestation."""
propDocs["channels"]=\
"""Associates a signal to the number of channels it holds (mono --> 1, stereo --> 2). Range is xsd:int."""
propDocs["description"]=\
"""An account of the resource."""
propDocs["item"]=\
"""Relates a musical manifestation to a musical item (this album, and my particular cd) holding the
		entire manifestation, and not just a part of it."""
propDocs["bpm"]=\
"""Indicates the BPM of a MusicalWork or a particular Performance 
		Beats per minute: the pace of music measured by the number of beats occurring in 60 seconds."""
propDocs["msnChatID"]=\
"""An MSN chat ID"""
propDocs["produced_signal"]=\
"""Associates a Recording to the outputted signal."""
propDocs["mbox"]=\
"""A  personal mailbox, ie. an Internet mailbox associated with exactly one owner, the first owner of this mailbox. This is a 'static inverse functional property', in that  there is (across time and change) at most one individual that ever has any particular value for foaf:mbox."""
propDocs["phone"]=\
"""A phone,  specified using fully qualified tel: URI scheme (refs: http://www.w3.org/Addressing/schemes.html#tel)."""
propDocs["composer"]=\
"""Associates a composition event to the actual composer. For example,
		this property could link the event corresponding to the composition of the
		Magic Flute in 1782 to Mozart himself (who obviously has a FOAF profile:-) )."""
propDocs["firstName"]=\
"""The first name of a person."""
propDocs["translation_of"]=\
"""Indicates that a work or the expression of a work has translated or transliterated into another expression of a work."""
propDocs["compiler"]=\
"""Used to relate the manifestation of a musical work to a person or a group of person who compiled it."""
propDocs["onlinecommunity"]=\
"""Used to link a person with an online community web page like a blog, a wiki, a forum, a livejournal page, Etc."""
propDocs["publishing_location"]=\
"""Relates a musical manifestation to its publication location."""
propDocs["track_number"]=\
"""Indicates the position of a track on a record medium (a CD, etc.)."""
propDocs["possess_item"]=\
"""A person, a group of person or an organization possessing an exemplar of a single manifestation."""
propDocs["publishingLocation"]=""
propDocs["producesSound"]=""
propDocs["published"]=\
"""Used to relate an person or a group of person who published the manifestation of a work."""
propDocs["preview_download"]=\
"""This property can be used to link from a person to the website where they make previews of their works available, or from
                a manifestation (a track or an album, for example) to a web page where a preview download is available.

                This property MUST be used only if the content is just available through a web page (holding, for example
                a Flash application) - it is better to link to actual content directly through the use of mo:availableAs and
                mo:Stream, mo:Torrent or mo:ED2K, etc. Therefore, Semantic Web user agents that don't know how to read HTML and even
                less to rip streams from Flash applications can still access the audio content."""
propDocs["trackNum"]=""
propDocs["publisher"]=\
"""Used to relate a musical manifestation to a person or a group of person who published it."""
propDocs["hasManifestation"]=""
propDocs["jabberID"]=\
"""A jabber ID for something."""
propDocs["produced"]=\
"""Used to relate an person or a group of person who produced the manifestation of a work."""
propDocs["intervalDuring"]=\
"""One of Allen's relations. Specifies that an interval occurs during an other.
		It is really handy to express things like "it happened the 15th of august, but I do not remember exactly when"."""
propDocs["isrc"]=\
"""The ISRC (International Standard Recording Code) is the international identification system for sound recordings and music videorecordings. 
	Each ISRC is a unique and permanent identifier for a specific recording which can be permanently encoded into a product as its digital fingerprint. 
	Encoded ISRC provide the means to automatically identify recordings for royalty payments."""
propDocs["hasProduct"]=\
"""Associates an event to something it produces.
		Example: a sound, a score, a musical work..."""
propDocs["has_track"]=""
propDocs["durationXSD"]=\
"""Links an interval to its duration using xsd:duration"""
propDocs["paiddownload"]=""
propDocs["bitsPerSample"]=\
"""Associates a digital signal to the number a bits used to encode one sample. Range is xsd:int."""
propDocs["movementNum"]=""
propDocs["musicbrainz"]=\
"""Linking an agent, a track or a record to its corresponding Musicbrainz page."""
propDocs["image"]=\
"""Indicates a pictorial image (JPEG, GIF, PNG, Etc.) of a musical work, the expression of a musical work, the manifestation of a work or the examplar of a manifestation."""
propDocs["accountName"]=\
"""Indicates the name (identifier) associated with this online account."""
propDocs["topic"]=\
"""A topic of some page or document."""
propDocs["compiled"]=\
"""Used to relate an person or a group of person who compiled the manifestation of a musical work."""
propDocs["rangeTimeLine"]=\
"""This property allows to associate a TimeLineMap with the second TimeLine it maps"""
propDocs["review"]=\
"""Used to link a work or the expression of a work to a review. 
    
The review does not have to be open content, as long as it is accessible to the general internet population."""
propDocs["name"]=\
"""A name for some thing."""
propDocs["topic_interest"]=\
"""A thing of interest to this person."""
propDocs["sampled_version_of"]=\
"""Associates a digital signal with the analog version of it"""
propDocs["fanpage"]=\
"""Used to link an artist to a fan-created webpage devoted to that artist."""
propDocs["sampler"]=\
"""Used to relate the manifestation of a musical work to an artist who sampled it."""
propDocs["place"]=\
"""Associate an event with a place.

		The associated geographic object contains the actual geographic extent of the event.

		For example, linking an event to Paris, France, will specify that this event happened somewhere
		within this geographical object."""
propDocs["medley_of"]=\
"""Indicates that a musical expression is a medley of several other musical expressions. 
    
This means that the orignial musical expression were rearranged to create a new musical expression in the form of a medley."""
propDocs["usedInPerformance"]=""
propDocs["member"]=\
"""Indicates a member of a Group
	Indicates a member of a Group"""
propDocs["depicts"]=\
"""A thing depicted in this representation."""
propDocs["tribute_to"]=\
"""Indicates a musical work or the expression of a musical work that is a tribute to an artist - normally consisting of music being composed by the artist but performed by other artists."""
propDocs["primaryTopic"]=\
"""The primary topic of some page or document."""
propDocs["usesSound"]=""
propDocs["availableAs"]=""
propDocs["performer"]=\
"""Relates a performance to the performers involved"""
propDocs["producesSignal"]=""
propDocs["sampled_version"]=\
"""Associates an analog signal with a sampled version of it"""
propDocs["event_homepage"]=\
"""Links a particular event to a web page"""
propDocs["other_release_of"]=\
"""Indicates that two musical manifestations are essentially the same."""
propDocs["recorded_in"]=\
"""Associates a physical Sound to a Recording event where it is being used 
		in order to produce a signal. For example, I might use this property to 
		associate the sound produced by a particular performance of the magic flute
		to a given recording, done using my cell-phone."""
propDocs["sampleRate"]=""
propDocs["nick"]=\
"""A short informal nickname characterising an agent (includes login identifiers, IRC and other chat nicknames)."""
propDocs["usesWork"]=""
propDocs["sample_rate"]=\
"""Associates a digital signal to its sample rate. It might be easier to express it this way instead of
		defining a timeline map:-) Range is xsd:float."""
propDocs["publications"]=\
"""A link to the publications of this person."""
propDocs["workplaceHomepage"]=\
"""A workplace homepage of some person; the homepage of an organization they work for."""
propDocs["member_of"]=\
"""Inverse of the foaf:member property"""
propDocs["title"]=\
"""A name given to the resource."""
propDocs["publicationOf"]=""
propDocs["arranged_in"]=\
"""Associates a work to an arrangement event where it was arranged"""
propDocs["engineered"]=\
"""Relates agents to the performances/recordings they were engineering in"""
propDocs["arrangement_of"]=\
"""Associates an arrangement event to a work"""
propDocs["givenname"]=\
"""The given name of some person."""
propDocs["collaborated_with"]=\
"""Used to relate two collaborating people on a work."""
propDocs["knows"]=\
"""A person known by this person (indicating some level of reciprocated interaction between the parties)."""
propDocs["yahooChatID"]=\
"""A Yahoo chat ID"""
propDocs["intervalBefore"]=\
"""One of Allen's relations. Specifies that an interval is before an other."""
propDocs["sampled"]=\
"""Used to relate an artist who sampled a Signal."""
propDocs["djmixed"]=\
"""Used to relate an artist who djmixed a musical work or the expression of a musical work. 
    
The artist usually selected the tracks, chose their sequence, and slightly changed them by fading (so that one track blends in smoothly with the other) or pitching (so that the tempo of one track matches the tempo of the following track). This applies to a 'Mixtape' in which all tracks were DJ-mixed together into one single long track."""
propDocs["licence"]=\
"""Used to link a work or the expression of a work to the license under which they can be manipulated (downloaded, modified, etc). 
    
This is usually used to link to a Creative Commons licence."""
propDocs["depiction"]=\
"""A depiction of some thing."""
propDocs["onTimeLine"]=\
"""Links an instant or an interval to the timeline it is defined on (eg. "1970 is defined on the
		time line universaltimeline", or "the interval between 0 and 2 minutes is defined on the time
		line backing this sound and this signal")."""
propDocs["hasAgent"]=\
"""Associates an event to an active agent.
		Example: an engineer, a performer, a composer..."""
propDocs["signalTime"]=""
propDocs["currentProject"]=\
"""A current project this person works on."""
propDocs["geekcode"]=\
"""A textual geekcode for this person, see http://www.geekcode.com/geek.html"""
propDocs["key"]=\
"""Indicated the key used by the musicians during a performance, or the key of a MusicalWork.
		Any of 24 major or minor diatonic scales that provide the tonal framework for a piece of music."""
propDocs["produced_sound"]=\
"""Associates a Performance to a physical Sound that is being produced by it."""
propDocs["performed_in"]=\
"""Associates a Musical Work or an Score to Performances in which they were
		a factor. For example, I might use this property in order to 
		associate the Magic Flute to a particular performance at the Opera
		Bastille last year."""
propDocs["icqChatID"]=\
"""An ICQ chat ID"""
propDocs["based_near"]=\
"""A location that something is based near, for some broadly human notion of near."""
propDocs["djmix_of"]=\
"""Indicates that all (or most of) the tracks of a musical work or the expression of a musical work were mixed together from all (or most of) the tracks from another musical work or the expression of a musical work to form a so called DJ-Mix. 
    
The tracks might have been altered by pitching (so that the tempo of one track matches the tempo of the following track) and fading (so that one track blends in smoothly with the other). If the tracks have been more substantially altered, the "mo:remix" relationship type is more appropriate."""
propDocs["wikipedia"]=\
"""Used to link an work, an expression of a work, a manifestation of a work, 
		a person, an instrument or a musical genre to its corresponding WikiPedia page. 
		The full URL should be used, not just the WikiName."""
propDocs["recorded_as"]=\
"""This is a shortcut property, allowing to bypass all the Sound/Recording steps. This property
		allows to directly link a Performance to the recorded Signal. This is recommended for "normal"
		users. However, advanced users wanting to express things such as the location of the microphone will
		have to create this shortcut as well as the whole workflow, in order to let the "normal" users access
		simply the, well, simple information:-) ."""
propDocs["isAgentIn"]=\
"""Associates an agent to an event."""
propDocs["recordedAs"]=""
propDocs["producer"]=\
"""Used to relate the manifestation of a work to a person or a group of person who produced it."""
propDocs["publication_of"]=\
"""Link a particular manifestation to the related signal, score, libretto, or lyrics"""
propDocs["originAt"]=\
"""This property specifies, for an OriginMap, the time point on the physical time line
		0 on a RelativeTimeLine is equivalent to."""
propDocs["contains_sample_from"]=\
"""Relates a signal to another signal, which has been sampled."""
propDocs["hasFactor"]=\
"""Associates an event to a passive factor of it.
		Example: a flute, a musical score, a musical work..."""
propDocs["releaseType"]=""
propDocs["djmixed_by"]=\
"""Used to relate a work or the expression of a work to an artist who djmixed it. 
    
The artist usually selected the tracks, chose their sequence, and slightly changed them by fading (so that one track blends in smoothly with the other) or pitching (so that the tempo of one track matches the tempo of the following track). This applies to a 'Mixtape' in which all tracks were DJ-mixed together into one single long track."""
propDocs["title"]=\
"""Title (Mr, Mrs, Ms, Dr. etc)"""
propDocs["myersBriggs"]=\
"""A Myers Briggs (MBTI) personality classification."""
propDocs["performance_of"]=\
"""Associates a Performance to a musical work or an arrangement that is being used as a factor in it.
		For example, I might use this property to attach the Magic Flute musical work to 
		a particular Performance."""
propDocs["paid_download"]=\
"""Provide a link from an artist to a web page where all of that artist's musical work is available for some money,
                or a link from a manifestation (record/track, for example) to a web page providing a paid access to this manifestation."""
propDocs["tipjar"]=\
"""A tipjar document for this agent, describing means for payment and reward."""
propDocs["homepage"]=\
"""A homepage for some thing."""
propDocs["listened"]=\
"""Relates agents to the performances they were listening in"""
propDocs["family_name"]=\
"""The family_name of some person."""
propDocs["eventHomePage"]=""
propDocs["intervalMeets"]=\
"""One of Allen's relations. Specifies that an interval meets an other one."""
propDocs["myspace"]=\
"""Used to link a person to its corresponding MySpace page."""
propDocs["domainTimeLine"]=\
"""This property allows to associate a TimeLineMap with the first TimeLine it maps"""
propDocs["fundedBy"]=\
"""An organization funding a project or person."""
propDocs["interest"]=\
"""A page about a topic of interest to this person."""
propDocs["workInfoHomepage"]=\
"""A work info homepage of some person; a page about their work for some organization."""
propDocs["remixed"]=\
"""Used to relate an artist who remixed a musical work or the expression of a musical work. 
    
This involves taking just one other musical work and using audio editing to make it sound like a significantly different, but usually still recognisable, song. It can be used to link an artist to a single song that they remixed, or, if they remixed an entire musical work."""
propDocs["performed"]=\
"""Relates agents to the performances they were performing in"""
propDocs["time"]=\
"""Associates a Signal to a time object - its actual domain"""
propDocs["weblog"]=\
"""A weblog of some thing (whether person, group, company etc.)."""
propDocs["img"]=\
"""An image that can be used to represent some thing (ie. those depictions which are particularly representative of something, eg. one's photo on a homepage)."""
propDocs["time"]=\
"""Associate an event with a time interval or a time instant.

		The associated time object contains the actual temporal extent of the event.

		For example, linking an event to the 9th of August, 2007, will specify that this event 
		happened during this day."""
propDocs["membershipClass"]=\
"""Indicates the class of individuals that are a member of a Group"""
propDocs["amazon_asin"]=\
"""Used to link a work or the expression of a work to its corresponding Amazon ASINs page."""
propDocs["opus"]=\
"""Used to define a creative work, especially a musical composition numbered to designate the order of a composer's works."""
propDocs["biography"]=\
"""Used to link an artist to their online biography."""
propDocs["listener"]=\
"""Relates a performance to the listener involved"""
propDocs["accountServiceHomepage"]=\
"""Indicates a homepage of the service provide for this online account."""
propDocs["puid"]=\
"""Link a signal to the PUIDs associated with it, that is, PUID computed from MusicalItems (mo:AudioFile) 
		derived from this signal.

		PUIDs (Portable Unique IDentifier) are the IDs used in the 
		proprietary MusicDNS AudioFingerprinting system which is operated by MusicIP.

		Using PUIDs, one (with some luck) can identify the Signal object associated with a particular audio file, therefore allowing
		to access further information (on which release this track is featured? etc.). Using some more metadata one can identify
		the particular Track corresponding to the audio file (a track on a particular release)."""
propDocs["conducted"]=\
"""Relates agents to the performances they were conducting"""
propDocs["mailorder"]=\
"""Used to link a musical work or the expression of a musical work to a website where people can buy a copy of the musical manifestation."""
propDocs["productOfComposition"]=""
propDocs["pastProject"]=\
"""A project this person has previously worked on."""
propDocs["published_as"]=\
"""Links a signal or a score to its manifestations - this is a subproperty of frbr:embodiment (linking
		an Expression to a Manifestation)"""
propDocs["birthday"]=\
"""The birthday of this Agent, represented in mm-dd string form, eg. '12-31'."""
propDocs["free_download"]=\
"""This property can be used to link from a person to the website where they make their works available, or from
		a manifestation (a track or an album, for example) to a web page where it is available for free 
		download.

		This property MUST be used only if the content is just available through a web page (holding, for example
		a Flash application) - it is better to link to actual content directly through the use of mo:availableAs and 
		mo:Stream, mo:Torrent or mo:ED2K, etc. Therefore, Semantic Web user agents that don't know how to read HTML and even
		less to rip streams from Flash applications can still access the audio content."""
propDocs["openid"]=\
"""An OpenID  for an Agent."""
propDocs["aimChatID"]=\
"""An AIM chat ID"""
propDocs["trmid"]=\
"""Indicates the TRMID of a track.
		TRM IDs are MusicBrainz' old AudioFingerprinting system. 
		TRM (TRM Recognizes Music) IDs are (somewhat) unique ids that represent 
		the audio signature of a musical piece (see AudioFingerprint)."""
propDocs["logo"]=\
"""A logo representing some thing."""
propDocs["want_item"]=\
"""A person, a group of person or an organization wanting an exemplar of a single manifestation."""
propDocs["encodes"]=\
"""Relates a MusicalItem (a track on a particular CD, an audio file, a stream somewhere) to the signal it encodes.

		This is usually a lower-resolution version of the master signal (issued from a Recording event)."""




# ========================  Class Definitions  ====================== 

class Thing(object):
	"""
	owl:Thing
	"""
	def __init__(self,URI=None):
		self._initialised = False
		self.shortname = "Thing"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["depiction"] = PropertySet("depiction","http://xmlns.com/foaf/0.1/depiction", Image, False)
		self._props["fundedBy"] = PropertySet("fundedBy","http://xmlns.com/foaf/0.1/fundedBy", Thing, False)
		self._props["homepage"] = PropertySet("homepage","http://xmlns.com/foaf/0.1/homepage", Document, False)
		self._props["isPrimaryTopicOf"] = PropertySet("isPrimaryTopicOf","http://xmlns.com/foaf/0.1/isPrimaryTopicOf", Document, False)
		self._props["logo"] = PropertySet("logo","http://xmlns.com/foaf/0.1/logo", Thing, False)
		self._props["maker"] = PropertySet("maker","http://xmlns.com/foaf/0.1/maker", Agent, False)
		self._props["name"] = PropertySet("name","http://xmlns.com/foaf/0.1/name", None, True)
		self._props["page"] = PropertySet("page","http://xmlns.com/foaf/0.1/page", Document, False)
		self._props["theme"] = PropertySet("theme","http://xmlns.com/foaf/0.1/theme", Thing, False)
		self._initialised = True
	classURI = "http://www.w3.org/2002/07/owl#Thing"


	# Python class properties to wrap the PropertySet objects
	depiction = property(fget=lambda x: x._props["depiction"].get(), fset=lambda x,y : x._props["depiction"].set(y), fdel=None, doc=propDocs["depiction"])
	fundedBy = property(fget=lambda x: x._props["fundedBy"].get(), fset=lambda x,y : x._props["fundedBy"].set(y), fdel=None, doc=propDocs["fundedBy"])
	homepage = property(fget=lambda x: x._props["homepage"].get(), fset=lambda x,y : x._props["homepage"].set(y), fdel=None, doc=propDocs["homepage"])
	isPrimaryTopicOf = property(fget=lambda x: x._props["isPrimaryTopicOf"].get(), fset=lambda x,y : x._props["isPrimaryTopicOf"].set(y), fdel=None, doc=propDocs["isPrimaryTopicOf"])
	logo = property(fget=lambda x: x._props["logo"].get(), fset=lambda x,y : x._props["logo"].set(y), fdel=None, doc=propDocs["logo"])
	maker = property(fget=lambda x: x._props["maker"].get(), fset=lambda x,y : x._props["maker"].set(y), fdel=None, doc=propDocs["maker"])
	name = property(fget=lambda x: x._props["name"].get(), fset=lambda x,y : x._props["name"].set(y), fdel=None, doc=propDocs["name"])
	page = property(fget=lambda x: x._props["page"].get(), fset=lambda x,y : x._props["page"].set(y), fdel=None, doc=propDocs["page"])
	theme = property(fget=lambda x: x._props["theme"].get(), fset=lambda x,y : x._props["theme"].set(y), fdel=None, doc=propDocs["theme"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class OnlineAccount(Thing):
	"""
	foaf:OnlineAccount
	An online account.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Thing.__init__(self)
		self._initialised = False
		self.shortname = "OnlineAccount"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["accountName"] = PropertySet("accountName","http://xmlns.com/foaf/0.1/accountName", None, True)
		self._props["accountServiceHomepage"] = PropertySet("accountServiceHomepage","http://xmlns.com/foaf/0.1/accountServiceHomepage", Document, False)
		self._initialised = True
	classURI = "http://xmlns.com/foaf/0.1/OnlineAccount"


	# Python class properties to wrap the PropertySet objects
	accountName = property(fget=lambda x: x._props["accountName"].get(), fset=lambda x,y : x._props["accountName"].set(y), fdel=None, doc=propDocs["accountName"])
	accountServiceHomepage = property(fget=lambda x: x._props["accountServiceHomepage"].get(), fset=lambda x,y : x._props["accountServiceHomepage"].set(y), fdel=None, doc=propDocs["accountServiceHomepage"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class OnlineEcommerceAccount(OnlineAccount):
	"""
	foaf:OnlineEcommerceAccount
	An online e-commerce account.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		OnlineAccount.__init__(self)
		self._initialised = False
		self.shortname = "OnlineEcommerceAccount"
		self.URI = URI
		self._initialised = True
	classURI = "http://xmlns.com/foaf/0.1/OnlineEcommerceAccount"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Resource(Thing):
	"""
	rdfs:Resource
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Thing.__init__(self)
		self._initialised = False
		self.shortname = "Resource"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["isFactorOf"] = PropertySet("isFactorOf","http://purl.org/NET/c4dm/event.owl#isFactorOf", Event, False)
		self._props["producedIn"] = PropertySet("producedIn","http://purl.org/NET/c4dm/event.owl#producedIn", Event, False)
		self._props["creator"] = PropertySet("creator","http://purl.org/dc/elements/1.1/creator", Agent, False)
		self._props["description"] = PropertySet("description","http://purl.org/dc/elements/1.1/description", None, True)
		self._props["title"] = PropertySet("title","http://purl.org/dc/elements/1.1/title", None, True)
		self._initialised = True
	classURI = "http://www.w3.org/2000/01/rdf-schema#Resource"


	# Python class properties to wrap the PropertySet objects
	isFactorOf = property(fget=lambda x: x._props["isFactorOf"].get(), fset=lambda x,y : x._props["isFactorOf"].set(y), fdel=None, doc=propDocs["isFactorOf"])
	producedIn = property(fget=lambda x: x._props["producedIn"].get(), fset=lambda x,y : x._props["producedIn"].set(y), fdel=None, doc=propDocs["producedIn"])
	creator = property(fget=lambda x: x._props["creator"].get(), fset=lambda x,y : x._props["creator"].set(y), fdel=None, doc=propDocs["creator"])
	description = property(fget=lambda x: x._props["description"].get(), fset=lambda x,y : x._props["description"].set(y), fdel=None, doc=propDocs["description"])
	title = property(fget=lambda x: x._props["title"].get(), fset=lambda x,y : x._props["title"].set(y), fdel=None, doc=propDocs["title"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class TemporalEntity(Resource):
	"""
	time:TemporalEntity
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "TemporalEntity"
		self.URI = URI
		self._initialised = True
	classURI = "http://www.w3.org/2006/time#TemporalEntity"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Document(Resource):
	"""
	foaf:Document
	A document.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Document"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["primaryTopic"] = PropertySet("primaryTopic","http://xmlns.com/foaf/0.1/primaryTopic", Thing, False)
		self._props["sha1"] = PropertySet("sha1","http://xmlns.com/foaf/0.1/sha1", None, False)
		self._props["topic"] = PropertySet("topic","http://xmlns.com/foaf/0.1/topic", Thing, False)
		self._initialised = True
	classURI = "http://xmlns.com/foaf/0.1/Document"


	# Python class properties to wrap the PropertySet objects
	primaryTopic = property(fget=lambda x: x._props["primaryTopic"].get(), fset=lambda x,y : x._props["primaryTopic"].set(y), fdel=None, doc=propDocs["primaryTopic"])
	sha1 = property(fget=lambda x: x._props["sha1"].get(), fset=lambda x,y : x._props["sha1"].set(y), fdel=None, doc=propDocs["sha1"])
	topic = property(fget=lambda x: x._props["topic"].get(), fset=lambda x,y : x._props["topic"].set(y), fdel=None, doc=propDocs["topic"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Image(Resource):
	"""
	foaf:Image
	An image.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Image"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["depicts"] = PropertySet("depicts","http://xmlns.com/foaf/0.1/depicts", Thing, False)
		self._props["thumbnail"] = PropertySet("thumbnail","http://xmlns.com/foaf/0.1/thumbnail", Image, False)
		self._initialised = True
	classURI = "http://xmlns.com/foaf/0.1/Image"


	# Python class properties to wrap the PropertySet objects
	depicts = property(fget=lambda x: x._props["depicts"].get(), fset=lambda x,y : x._props["depicts"].set(y), fdel=None, doc=propDocs["depicts"])
	thumbnail = property(fget=lambda x: x._props["thumbnail"].get(), fset=lambda x,y : x._props["thumbnail"].set(y), fdel=None, doc=propDocs["thumbnail"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class OnlineGamingAccount(OnlineAccount):
	"""
	foaf:OnlineGamingAccount
	An online gaming account.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		OnlineAccount.__init__(self)
		self._initialised = False
		self.shortname = "OnlineGamingAccount"
		self.URI = URI
		self._initialised = True
	classURI = "http://xmlns.com/foaf/0.1/OnlineGamingAccount"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class PersonalProfileDocument(Document):
	"""
	foaf:PersonalProfileDocument
	A personal profile RDF document.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Document.__init__(self)
		self._initialised = False
		self.shortname = "PersonalProfileDocument"
		self.URI = URI
		self._initialised = True
	classURI = "http://xmlns.com/foaf/0.1/PersonalProfileDocument"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Event(Resource):
	"""
	event:Event
	 
		An event: a way of arbitrary classifying a space/time region.
		An event has agents (active entities contributing to the event -- a performer, a composer, an engineer, ...),
		factors (passive entities contributing to the event -- flute, score, ...),
		and products (things produced by the event -- sound, signal, score, ...). For
		example, we may describe as Events: performances, composition events, recordings, arrangements,
		creation of a musical group, separation of a musical group,
		but also sounds, signals, notes (in a score)...
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Event"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["hasAgent"] = PropertySet("hasAgent","http://purl.org/NET/c4dm/event.owl#hasAgent", Agent, False)
		self._props["hasFactor"] = PropertySet("hasFactor","http://purl.org/NET/c4dm/event.owl#hasFactor", Resource, False)
		self._props["hasProduct"] = PropertySet("hasProduct","http://purl.org/NET/c4dm/event.owl#hasProduct", Resource, False)
		self._props["hasSubEvent"] = PropertySet("hasSubEvent","http://purl.org/NET/c4dm/event.owl#hasSubEvent", Event, False)
		self._props["place"] = PropertySet("place","http://purl.org/NET/c4dm/event.owl#place", SpatialThing, False)
		self._props["time"] = PropertySet("time","http://purl.org/NET/c4dm/event.owl#time", TemporalEntity, False)
		self._props["event_homepage"] = PropertySet("event_homepage","http://purl.org/ontology/mo/event_homepage", Document, False)
		self._initialised = True
	classURI = "http://purl.org/NET/c4dm/event.owl#Event"


	# Python class properties to wrap the PropertySet objects
	hasAgent = property(fget=lambda x: x._props["hasAgent"].get(), fset=lambda x,y : x._props["hasAgent"].set(y), fdel=None, doc=propDocs["hasAgent"])
	hasFactor = property(fget=lambda x: x._props["hasFactor"].get(), fset=lambda x,y : x._props["hasFactor"].set(y), fdel=None, doc=propDocs["hasFactor"])
	hasProduct = property(fget=lambda x: x._props["hasProduct"].get(), fset=lambda x,y : x._props["hasProduct"].set(y), fdel=None, doc=propDocs["hasProduct"])
	hasSubEvent = property(fget=lambda x: x._props["hasSubEvent"].get(), fset=lambda x,y : x._props["hasSubEvent"].set(y), fdel=None, doc=propDocs["hasSubEvent"])
	place = property(fget=lambda x: x._props["place"].get(), fset=lambda x,y : x._props["place"].set(y), fdel=None, doc=propDocs["place"])
	time = property(fget=lambda x: x._props["time"].get(), fset=lambda x,y : x._props["time"].set(y), fdel=None, doc=propDocs["time"])
	event_homepage = property(fget=lambda x: x._props["event_homepage"].get(), fset=lambda x,y : x._props["event_homepage"].set(y), fdel=None, doc=propDocs["event_homepage"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Note(Resource):
	"""
	key:Note
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Note"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/NET/c4dm/keys.owl#Note"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class TimeLine(Resource):
	"""
	timeline:TimeLine
	 
		A time line -- a coherent "backbone" for addressing points and intervals.
		We can consider the timeline backing an audio/video signal, the one
		corresponding to the "physical" time, or even the one backing a score.
		Here, we consider that the timeline is *also* its coordinate system, for
		simplification purposes. In the DL version of the timeline ontology,
		coordinate systems are defined through restrictions on the way to 
		address time points/intervals on a timeline.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "TimeLine"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/NET/c4dm/timeline.owl#TimeLine"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Arrangement(Event):
	"""
	mo:Arrangement
	
		An arrangement event.
		Takes as agent the arranger, and produces a score (informational object, not the actually published score).
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Event.__init__(self)
		self._initialised = False
		self.shortname = "Arrangement"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["arrangement_of"] = PropertySet("arrangement_of","http://purl.org/ontology/mo/arrangement_of", MusicalWork, False)
		self._props["genre"] = PropertySet("genre","http://purl.org/ontology/mo/genre", Genre, False)
		self._props["produced_score"] = PropertySet("produced_score","http://purl.org/ontology/mo/produced_score", Score, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Arrangement"


	# Python class properties to wrap the PropertySet objects
	arrangement_of = property(fget=lambda x: x._props["arrangement_of"].get(), fset=lambda x,y : x._props["arrangement_of"].set(y), fdel=None, doc=propDocs["arrangement_of"])
	genre = property(fget=lambda x: x._props["genre"].get(), fset=lambda x,y : x._props["genre"].set(y), fdel=None, doc=propDocs["genre"])
	produced_score = property(fget=lambda x: x._props["produced_score"].get(), fset=lambda x,y : x._props["produced_score"].set(y), fdel=None, doc=propDocs["produced_score"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Composition(Event):
	"""
	mo:Composition
	
		A composition event.
		Takes as agent the composer himself.
		It produces a MusicalWork, or a MusicalExpression (when the initial "product" is a score, for example), or both...
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Event.__init__(self)
		self._initialised = False
		self.shortname = "Composition"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["composer"] = PropertySet("composer","http://purl.org/ontology/mo/composer", Agent, False)
		self._props["genre"] = PropertySet("genre","http://purl.org/ontology/mo/genre", Genre, False)
		self._props["produced_work"] = PropertySet("produced_work","http://purl.org/ontology/mo/produced_work", MusicalWork, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Composition"


	# Python class properties to wrap the PropertySet objects
	composer = property(fget=lambda x: x._props["composer"].get(), fset=lambda x,y : x._props["composer"].set(y), fdel=None, doc=propDocs["composer"])
	genre = property(fget=lambda x: x._props["genre"].get(), fset=lambda x,y : x._props["genre"].set(y), fdel=None, doc=propDocs["genre"])
	produced_work = property(fget=lambda x: x._props["produced_work"].get(), fset=lambda x,y : x._props["produced_work"].set(y), fdel=None, doc=propDocs["produced_work"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Festival(Event):
	"""
	mo:Festival
	
		A festival - musical/artistic event lasting several days, like Glastonbury, Rock Am Ring...
		We migth decompose this event (which is in fact just a classification of the space/time region related to 
		a particular festival) using hasSubEvent in several performances at different space/time.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Event.__init__(self)
		self._initialised = False
		self.shortname = "Festival"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Festival"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Instrument(Resource):
	"""
	mo:Instrument
	
		Any of various devices or contrivances that can be used to produce musical tones or sound.
		
		Any taxonomy can be used to subsume this concept. The default one is one extracted by Ivan Herman
		from the Musicbrainz instrument taxonomy, conforming to SKOS. This concept holds a seeAlso link 
		towards this taxonomy.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Instrument"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["wikipedia"] = PropertySet("wikipedia","http://purl.org/ontology/mo/wikipedia", Document, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Instrument"


	# Python class properties to wrap the PropertySet objects
	wikipedia = property(fget=lambda x: x._props["wikipedia"].get(), fset=lambda x,y : x._props["wikipedia"].set(y), fdel=None, doc=propDocs["wikipedia"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class MusicalItem(Resource):
	"""
	mo:MusicalItem
	A single exemplar of a musical expression.
    
For example, it could be a single exemplar of a CD. This is normally an single object (a CD) possessed by somebody.

From the FRBR final report: The entity defined as item is a concrete entity. It is in many instances a single physical object (e.g., a copy of a one-volume monograph, a single audio cassette, etc.). There are instances, however, where the entity defined as item comprises more than one physical object (e.g., a monograph issued as two separately bound volumes, a recording issued on three separate compact discs, etc.).

In terms of intellectual content and physical form, an item exemplifying a manifestation is normally the same as the manifestation itself. However, variations may occur from one item to another, even when the items exemplify the same manifestation, where those variations are the result of actions external to the intent of the producer of the manifestation (e.g., damage occurring after the item was produced, binding performed by a library, etc.). 
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "MusicalItem"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["encodes"] = PropertySet("encodes","http://purl.org/ontology/mo/encodes", Signal, False)
		self._props["image"] = PropertySet("image","http://purl.org/ontology/mo/image", Resource, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/MusicalItem"


	# Python class properties to wrap the PropertySet objects
	encodes = property(fget=lambda x: x._props["encodes"].get(), fset=lambda x,y : x._props["encodes"].set(y), fdel=None, doc=propDocs["encodes"])
	image = property(fget=lambda x: x._props["image"].get(), fset=lambda x,y : x._props["image"].set(y), fdel=None, doc=propDocs["image"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Orchestration(Arrangement):
	"""
	mo:Orchestration
	
        	Orchestration includes, in addition to instrumentation, the handling of groups of instruments and their balance and interaction.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Arrangement.__init__(self)
		self._initialised = False
		self.shortname = "Orchestration"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Orchestration"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Recording(Event):
	"""
	mo:Recording
	
		A recording event.
		Takes a sound as a factor to produce a signal (analog or digital).
		The location of such events (if any) is the actual location of the corresponding
		microphone or the "recording device".
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Event.__init__(self)
		self._initialised = False
		self.shortname = "Recording"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["engineer"] = PropertySet("engineer","http://purl.org/ontology/mo/engineer", Agent, False)
		self._props["genre"] = PropertySet("genre","http://purl.org/ontology/mo/genre", Genre, False)
		self._props["produced_signal"] = PropertySet("produced_signal","http://purl.org/ontology/mo/produced_signal", Signal, False)
		self._props["recording_of"] = PropertySet("recording_of","http://purl.org/ontology/mo/recording_of", Sound, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Recording"


	# Python class properties to wrap the PropertySet objects
	engineer = property(fget=lambda x: x._props["engineer"].get(), fset=lambda x,y : x._props["engineer"].set(y), fdel=None, doc=propDocs["engineer"])
	genre = property(fget=lambda x: x._props["genre"].get(), fset=lambda x,y : x._props["genre"].set(y), fdel=None, doc=propDocs["genre"])
	produced_signal = property(fget=lambda x: x._props["produced_signal"].get(), fset=lambda x,y : x._props["produced_signal"].set(y), fdel=None, doc=propDocs["produced_signal"])
	recording_of = property(fget=lambda x: x._props["recording_of"].get(), fset=lambda x,y : x._props["recording_of"].set(y), fdel=None, doc=propDocs["recording_of"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class ReleaseType(Resource):
	"""
	mo:ReleaseType
	
		Release type of a particular manifestation, such as "album" or "interview"...
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "ReleaseType"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/ReleaseType"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Show(Event):
	"""
	mo:Show
	
		A show - a musical event lasting several days, in a particular venue. Examples can be
		"The Magic Flute" at the Opera Bastille, August 2005, or a musical in the west end...
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Event.__init__(self)
		self._initialised = False
		self.shortname = "Show"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Show"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Expression(Resource):
	"""
	frbr:Expression
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Expression"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["amazon_asin"] = PropertySet("amazon_asin","http://purl.org/ontology/mo/amazon_asin", Document, False)
		self._props["licence"] = PropertySet("licence","http://purl.org/ontology/mo/licence", Document, False)
		self._props["review"] = PropertySet("review","http://purl.org/ontology/mo/review", Document, False)
		self._props["translation_of"] = PropertySet("translation_of","http://purl.org/ontology/mo/translation_of", Expression, False)
		self._props["wikipedia"] = PropertySet("wikipedia","http://purl.org/ontology/mo/wikipedia", Document, False)
		self._initialised = True
	classURI = "http://purl.org/vocab/frbr/core#Expression"


	# Python class properties to wrap the PropertySet objects
	amazon_asin = property(fget=lambda x: x._props["amazon_asin"].get(), fset=lambda x,y : x._props["amazon_asin"].set(y), fdel=None, doc=propDocs["amazon_asin"])
	licence = property(fget=lambda x: x._props["licence"].get(), fset=lambda x,y : x._props["licence"].set(y), fdel=None, doc=propDocs["licence"])
	review = property(fget=lambda x: x._props["review"].get(), fset=lambda x,y : x._props["review"].set(y), fdel=None, doc=propDocs["review"])
	translation_of = property(fget=lambda x: x._props["translation_of"].get(), fset=lambda x,y : x._props["translation_of"].set(y), fdel=None, doc=propDocs["translation_of"])
	wikipedia = property(fget=lambda x: x._props["wikipedia"].get(), fset=lambda x,y : x._props["wikipedia"].set(y), fdel=None, doc=propDocs["wikipedia"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Manifestation(Resource):
	"""
	frbr:Manifestation
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Manifestation"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["amazon_asin"] = PropertySet("amazon_asin","http://purl.org/ontology/mo/amazon_asin", Document, False)
		self._props["download"] = PropertySet("download","http://purl.org/ontology/mo/download", Document, False)
		self._props["free_download"] = PropertySet("free_download","http://purl.org/ontology/mo/free_download", Document, False)
		self._props["licence"] = PropertySet("licence","http://purl.org/ontology/mo/licence", Document, False)
		self._props["mailorder"] = PropertySet("mailorder","http://purl.org/ontology/mo/mailorder", Document, False)
		self._props["paid_download"] = PropertySet("paid_download","http://purl.org/ontology/mo/paid_download", Document, False)
		self._props["preview_download"] = PropertySet("preview_download","http://purl.org/ontology/mo/preview_download", Document, False)
		self._props["review"] = PropertySet("review","http://purl.org/ontology/mo/review", Document, False)
		self._props["wikipedia"] = PropertySet("wikipedia","http://purl.org/ontology/mo/wikipedia", Document, False)
		self._initialised = True
	classURI = "http://purl.org/vocab/frbr/core#Manifestation"


	# Python class properties to wrap the PropertySet objects
	amazon_asin = property(fget=lambda x: x._props["amazon_asin"].get(), fset=lambda x,y : x._props["amazon_asin"].set(y), fdel=None, doc=propDocs["amazon_asin"])
	download = property(fget=lambda x: x._props["download"].get(), fset=lambda x,y : x._props["download"].set(y), fdel=None, doc=propDocs["download"])
	free_download = property(fget=lambda x: x._props["free_download"].get(), fset=lambda x,y : x._props["free_download"].set(y), fdel=None, doc=propDocs["free_download"])
	licence = property(fget=lambda x: x._props["licence"].get(), fset=lambda x,y : x._props["licence"].set(y), fdel=None, doc=propDocs["licence"])
	mailorder = property(fget=lambda x: x._props["mailorder"].get(), fset=lambda x,y : x._props["mailorder"].set(y), fdel=None, doc=propDocs["mailorder"])
	paid_download = property(fget=lambda x: x._props["paid_download"].get(), fset=lambda x,y : x._props["paid_download"].set(y), fdel=None, doc=propDocs["paid_download"])
	preview_download = property(fget=lambda x: x._props["preview_download"].get(), fset=lambda x,y : x._props["preview_download"].set(y), fdel=None, doc=propDocs["preview_download"])
	review = property(fget=lambda x: x._props["review"].get(), fset=lambda x,y : x._props["review"].set(y), fdel=None, doc=propDocs["review"])
	wikipedia = property(fget=lambda x: x._props["wikipedia"].get(), fset=lambda x,y : x._props["wikipedia"].set(y), fdel=None, doc=propDocs["wikipedia"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Class(Resource):
	"""
	rdfs:Class
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Class"
		self.URI = URI
		self._initialised = True
	classURI = "http://www.w3.org/2000/01/rdf-schema#Class"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Instant(TemporalEntity):
	"""
	time:Instant
	 A time point (eg. "now":-) )
	"""
	def __init__(self,URI=None):
		# Initialise parents
		TemporalEntity.__init__(self)
		self._initialised = False
		self.shortname = "Instant"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["atDateTime"] = PropertySet("atDateTime","http://purl.org/NET/c4dm/timeline.owl#atDateTime", None, True)
		self._props["atDuration"] = PropertySet("atDuration","http://purl.org/NET/c4dm/timeline.owl#atDuration", None, True)
		self._props["onTimeLine"] = PropertySet("onTimeLine","http://purl.org/NET/c4dm/timeline.owl#onTimeLine", TimeLine, False)
		self._initialised = True
	classURI = "http://www.w3.org/2006/time#Instant"


	# Python class properties to wrap the PropertySet objects
	atDateTime = property(fget=lambda x: x._props["atDateTime"].get(), fset=lambda x,y : x._props["atDateTime"].set(y), fdel=None, doc=propDocs["atDateTime"])
	atDuration = property(fget=lambda x: x._props["atDuration"].get(), fset=lambda x,y : x._props["atDuration"].set(y), fdel=None, doc=propDocs["atDuration"])
	onTimeLine = property(fget=lambda x: x._props["onTimeLine"].get(), fset=lambda x,y : x._props["onTimeLine"].set(y), fdel=None, doc=propDocs["onTimeLine"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Agent(Resource):
	"""
	foaf:Agent
	
		An agent (person, group, software, etc.).
	
	An agent (eg. person, group, software or physical artifact).
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Agent"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["isAgentIn"] = PropertySet("isAgentIn","http://purl.org/NET/c4dm/event.owl#isAgentIn", Event, False)
		self._props["collaborated_with"] = PropertySet("collaborated_with","http://purl.org/ontology/mo/collaborated_with", Agent, False)
		self._props["conducted"] = PropertySet("conducted","http://purl.org/ontology/mo/conducted", Performance, False)
		self._props["download"] = PropertySet("download","http://purl.org/ontology/mo/download", Document, False)
		self._props["engineered"] = PropertySet("engineered","http://purl.org/ontology/mo/engineered", (Performance,Recording), False)
		self._props["exchange_item"] = PropertySet("exchange_item","http://purl.org/ontology/mo/exchange_item", Item, False)
		self._props["free_download"] = PropertySet("free_download","http://purl.org/ontology/mo/free_download", Document, False)
		self._props["listened"] = PropertySet("listened","http://purl.org/ontology/mo/listened", Performance, False)
		self._props["musicbrainz"] = PropertySet("musicbrainz","http://purl.org/ontology/mo/musicbrainz", Document, False)
		self._props["myspace"] = PropertySet("myspace","http://purl.org/ontology/mo/myspace", Document, False)
		self._props["onlinecommunity"] = PropertySet("onlinecommunity","http://purl.org/ontology/mo/onlinecommunity", Document, False)
		self._props["performed"] = PropertySet("performed","http://purl.org/ontology/mo/performed", Performance, False)
		self._props["possess_item"] = PropertySet("possess_item","http://purl.org/ontology/mo/possess_item", Item, False)
		self._props["preview_download"] = PropertySet("preview_download","http://purl.org/ontology/mo/preview_download", Document, False)
		self._props["produced"] = PropertySet("produced","http://purl.org/ontology/mo/produced", MusicalManifestation, False)
		self._props["published"] = PropertySet("published","http://purl.org/ontology/mo/published", MusicalManifestation, False)
		self._props["sell_item"] = PropertySet("sell_item","http://purl.org/ontology/mo/sell_item", Item, False)
		self._props["similar_to"] = PropertySet("similar_to","http://purl.org/ontology/mo/similar_to", (Agent,Signal,Genre), False)
		self._props["want_item"] = PropertySet("want_item","http://purl.org/ontology/mo/want_item", Item, False)
		self._props["wikipedia"] = PropertySet("wikipedia","http://purl.org/ontology/mo/wikipedia", Document, False)
		self._props["aimChatID"] = PropertySet("aimChatID","http://xmlns.com/foaf/0.1/aimChatID", None, True)
		self._props["birthday"] = PropertySet("birthday","http://xmlns.com/foaf/0.1/birthday", None, True)
		self._props["gender"] = PropertySet("gender","http://xmlns.com/foaf/0.1/gender", None, True)
		self._props["holdsAccount"] = PropertySet("holdsAccount","http://xmlns.com/foaf/0.1/holdsAccount", OnlineAccount, False)
		self._props["icqChatID"] = PropertySet("icqChatID","http://xmlns.com/foaf/0.1/icqChatID", None, True)
		self._props["jabberID"] = PropertySet("jabberID","http://xmlns.com/foaf/0.1/jabberID", None, True)
		self._props["made"] = PropertySet("made","http://xmlns.com/foaf/0.1/made", (MusicalManifestation,Record,Track,Thing), False)
		self._props["mbox"] = PropertySet("mbox","http://xmlns.com/foaf/0.1/mbox", Thing, False)
		self._props["mbox_sha1sum"] = PropertySet("mbox_sha1sum","http://xmlns.com/foaf/0.1/mbox_sha1sum", None, True)
		self._props["msnChatID"] = PropertySet("msnChatID","http://xmlns.com/foaf/0.1/msnChatID", None, True)
		self._props["openid"] = PropertySet("openid","http://xmlns.com/foaf/0.1/openid", Document, False)
		self._props["tipjar"] = PropertySet("tipjar","http://xmlns.com/foaf/0.1/tipjar", Document, False)
		self._props["weblog"] = PropertySet("weblog","http://xmlns.com/foaf/0.1/weblog", Document, False)
		self._props["yahooChatID"] = PropertySet("yahooChatID","http://xmlns.com/foaf/0.1/yahooChatID", None, True)
		self._initialised = True
	classURI = "http://xmlns.com/foaf/0.1/Agent"


	# Python class properties to wrap the PropertySet objects
	isAgentIn = property(fget=lambda x: x._props["isAgentIn"].get(), fset=lambda x,y : x._props["isAgentIn"].set(y), fdel=None, doc=propDocs["isAgentIn"])
	collaborated_with = property(fget=lambda x: x._props["collaborated_with"].get(), fset=lambda x,y : x._props["collaborated_with"].set(y), fdel=None, doc=propDocs["collaborated_with"])
	conducted = property(fget=lambda x: x._props["conducted"].get(), fset=lambda x,y : x._props["conducted"].set(y), fdel=None, doc=propDocs["conducted"])
	download = property(fget=lambda x: x._props["download"].get(), fset=lambda x,y : x._props["download"].set(y), fdel=None, doc=propDocs["download"])
	engineered = property(fget=lambda x: x._props["engineered"].get(), fset=lambda x,y : x._props["engineered"].set(y), fdel=None, doc=propDocs["engineered"])
	exchange_item = property(fget=lambda x: x._props["exchange_item"].get(), fset=lambda x,y : x._props["exchange_item"].set(y), fdel=None, doc=propDocs["exchange_item"])
	free_download = property(fget=lambda x: x._props["free_download"].get(), fset=lambda x,y : x._props["free_download"].set(y), fdel=None, doc=propDocs["free_download"])
	listened = property(fget=lambda x: x._props["listened"].get(), fset=lambda x,y : x._props["listened"].set(y), fdel=None, doc=propDocs["listened"])
	musicbrainz = property(fget=lambda x: x._props["musicbrainz"].get(), fset=lambda x,y : x._props["musicbrainz"].set(y), fdel=None, doc=propDocs["musicbrainz"])
	myspace = property(fget=lambda x: x._props["myspace"].get(), fset=lambda x,y : x._props["myspace"].set(y), fdel=None, doc=propDocs["myspace"])
	onlinecommunity = property(fget=lambda x: x._props["onlinecommunity"].get(), fset=lambda x,y : x._props["onlinecommunity"].set(y), fdel=None, doc=propDocs["onlinecommunity"])
	performed = property(fget=lambda x: x._props["performed"].get(), fset=lambda x,y : x._props["performed"].set(y), fdel=None, doc=propDocs["performed"])
	possess_item = property(fget=lambda x: x._props["possess_item"].get(), fset=lambda x,y : x._props["possess_item"].set(y), fdel=None, doc=propDocs["possess_item"])
	preview_download = property(fget=lambda x: x._props["preview_download"].get(), fset=lambda x,y : x._props["preview_download"].set(y), fdel=None, doc=propDocs["preview_download"])
	produced = property(fget=lambda x: x._props["produced"].get(), fset=lambda x,y : x._props["produced"].set(y), fdel=None, doc=propDocs["produced"])
	published = property(fget=lambda x: x._props["published"].get(), fset=lambda x,y : x._props["published"].set(y), fdel=None, doc=propDocs["published"])
	sell_item = property(fget=lambda x: x._props["sell_item"].get(), fset=lambda x,y : x._props["sell_item"].set(y), fdel=None, doc=propDocs["sell_item"])
	similar_to = property(fget=lambda x: x._props["similar_to"].get(), fset=lambda x,y : x._props["similar_to"].set(y), fdel=None, doc=propDocs["similar_to"])
	want_item = property(fget=lambda x: x._props["want_item"].get(), fset=lambda x,y : x._props["want_item"].set(y), fdel=None, doc=propDocs["want_item"])
	wikipedia = property(fget=lambda x: x._props["wikipedia"].get(), fset=lambda x,y : x._props["wikipedia"].set(y), fdel=None, doc=propDocs["wikipedia"])
	aimChatID = property(fget=lambda x: x._props["aimChatID"].get(), fset=lambda x,y : x._props["aimChatID"].set(y), fdel=None, doc=propDocs["aimChatID"])
	birthday = property(fget=lambda x: x._props["birthday"].get(), fset=lambda x,y : x._props["birthday"].set(y), fdel=None, doc=propDocs["birthday"])
	gender = property(fget=lambda x: x._props["gender"].get(), fset=lambda x,y : x._props["gender"].set(y), fdel=None, doc=propDocs["gender"])
	holdsAccount = property(fget=lambda x: x._props["holdsAccount"].get(), fset=lambda x,y : x._props["holdsAccount"].set(y), fdel=None, doc=propDocs["holdsAccount"])
	icqChatID = property(fget=lambda x: x._props["icqChatID"].get(), fset=lambda x,y : x._props["icqChatID"].set(y), fdel=None, doc=propDocs["icqChatID"])
	jabberID = property(fget=lambda x: x._props["jabberID"].get(), fset=lambda x,y : x._props["jabberID"].set(y), fdel=None, doc=propDocs["jabberID"])
	made = property(fget=lambda x: x._props["made"].get(), fset=lambda x,y : x._props["made"].set(y), fdel=None, doc=propDocs["made"])
	mbox = property(fget=lambda x: x._props["mbox"].get(), fset=lambda x,y : x._props["mbox"].set(y), fdel=None, doc=propDocs["mbox"])
	mbox_sha1sum = property(fget=lambda x: x._props["mbox_sha1sum"].get(), fset=lambda x,y : x._props["mbox_sha1sum"].set(y), fdel=None, doc=propDocs["mbox_sha1sum"])
	msnChatID = property(fget=lambda x: x._props["msnChatID"].get(), fset=lambda x,y : x._props["msnChatID"].set(y), fdel=None, doc=propDocs["msnChatID"])
	openid = property(fget=lambda x: x._props["openid"].get(), fset=lambda x,y : x._props["openid"].set(y), fdel=None, doc=propDocs["openid"])
	tipjar = property(fget=lambda x: x._props["tipjar"].get(), fset=lambda x,y : x._props["tipjar"].set(y), fdel=None, doc=propDocs["tipjar"])
	weblog = property(fget=lambda x: x._props["weblog"].get(), fset=lambda x,y : x._props["weblog"].set(y), fdel=None, doc=propDocs["weblog"])
	yahooChatID = property(fget=lambda x: x._props["yahooChatID"].get(), fset=lambda x,y : x._props["yahooChatID"].set(y), fdel=None, doc=propDocs["yahooChatID"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class OnlineChatAccount(OnlineAccount):
	"""
	foaf:OnlineChatAccount
	An online chat account.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		OnlineAccount.__init__(self)
		self._initialised = False
		self.shortname = "OnlineChatAccount"
		self.URI = URI
		self._initialised = True
	classURI = "http://xmlns.com/foaf/0.1/OnlineChatAccount"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Project(Resource):
	"""
	foaf:Project
	A project (a collective endeavour of some kind).
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Project"
		self.URI = URI
		self._initialised = True
	classURI = "http://xmlns.com/foaf/0.1/Project"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Key(Resource):
	"""
	key:Key
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Key"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/NET/c4dm/keys.owl#Key"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class PhysicalTimeLine(TimeLine):
	"""
	timeline:PhysicalTimeLine
	
		Well, the actual physical time as we know it. I may want to address "yesterday" on instances
		of this class, or "the year 1789"...
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		TimeLine.__init__(self)
		self._initialised = False
		self.shortname = "PhysicalTimeLine"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/NET/c4dm/timeline.owl#PhysicalTimeLine"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class TimeLineMap(Resource):
	"""
	timeline:TimeLineMap
	
		Two time lines can be related, such as the one backing a continuous signal and
		the one backing the digitized signal. This sort of relation is expressed through an instance
		of a TimeLine map (eg. "the timeline backing this signal corresponds
		to the physical timeline: point 0 on this timeline corresponds to the
		20th of december at 5pm").
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "TimeLineMap"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["domainTimeLine"] = PropertySet("domainTimeLine","http://purl.org/NET/c4dm/timeline.owl#domainTimeLine", TimeLine, False)
		self._props["rangeTimeLine"] = PropertySet("rangeTimeLine","http://purl.org/NET/c4dm/timeline.owl#rangeTimeLine", TimeLine, False)
		self._initialised = True
	classURI = "http://purl.org/NET/c4dm/timeline.owl#TimeLineMap"


	# Python class properties to wrap the PropertySet objects
	domainTimeLine = property(fget=lambda x: x._props["domainTimeLine"].get(), fset=lambda x,y : x._props["domainTimeLine"].set(y), fdel=None, doc=propDocs["domainTimeLine"])
	rangeTimeLine = property(fget=lambda x: x._props["rangeTimeLine"].get(), fset=lambda x,y : x._props["rangeTimeLine"].set(y), fdel=None, doc=propDocs["rangeTimeLine"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Arranger(Agent):
	"""
	mo:Arranger
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Agent.__init__(self)
		self._initialised = False
		self.shortname = "Arranger"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Arranger"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Composer(Agent):
	"""
	mo:Composer
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Agent.__init__(self)
		self._initialised = False
		self.shortname = "Composer"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Composer"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Genre(Resource):
	"""
	mo:Genre
	
		An expressive style of music.
		
		Any taxonomy can be plug-in here. You can either define a genre by yourself, like this:

		:mygenre a mo:Genre; dc:title "electro rock".

		Or you can refer to a DBPedia genre (such as http://dbpedia.org/resource/Baroque_music), allowing semantic web
		clients to access easily really detailed structured information about the genre you are refering to.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Genre"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["similar_to"] = PropertySet("similar_to","http://purl.org/ontology/mo/similar_to", (Agent,Signal,Genre), False)
		self._props["wikipedia"] = PropertySet("wikipedia","http://purl.org/ontology/mo/wikipedia", Document, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Genre"


	# Python class properties to wrap the PropertySet objects
	similar_to = property(fget=lambda x: x._props["similar_to"].get(), fset=lambda x,y : x._props["similar_to"].set(y), fdel=None, doc=propDocs["similar_to"])
	wikipedia = property(fget=lambda x: x._props["wikipedia"].get(), fset=lambda x,y : x._props["wikipedia"].set(y), fdel=None, doc=propDocs["wikipedia"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Listener(Agent):
	"""
	mo:Listener
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Agent.__init__(self)
		self._initialised = False
		self.shortname = "Listener"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Listener"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Medium(MusicalItem):
	"""
	mo:Medium
	A means or instrumentality for storing or communicating musical manifestation.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		MusicalItem.__init__(self)
		self._initialised = False
		self.shortname = "Medium"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Medium"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class MusicArtist(Agent):
	"""
	mo:MusicArtist
	
		A person or a group of people (or a computer :-) ), whose musical 
		creative work shows sensitivity and imagination 
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Agent.__init__(self)
		self._initialised = False
		self.shortname = "MusicArtist"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["biography"] = PropertySet("biography","http://purl.org/ontology/mo/biography", Document, False)
		self._props["compiled"] = PropertySet("compiled","http://purl.org/ontology/mo/compiled", MusicalManifestation, False)
		self._props["discography"] = PropertySet("discography","http://purl.org/ontology/mo/discography", Document, False)
		self._props["discogs"] = PropertySet("discogs","http://purl.org/ontology/mo/discogs", Document, False)
		self._props["djmixed"] = PropertySet("djmixed","http://purl.org/ontology/mo/djmixed", Signal, False)
		self._props["fanpage"] = PropertySet("fanpage","http://purl.org/ontology/mo/fanpage", Document, False)
		self._props["imdb"] = PropertySet("imdb","http://purl.org/ontology/mo/imdb", Document, False)
		self._props["mailorder"] = PropertySet("mailorder","http://purl.org/ontology/mo/mailorder", Document, False)
		self._props["musicmoz"] = PropertySet("musicmoz","http://purl.org/ontology/mo/musicmoz", Document, False)
		self._props["paid_download"] = PropertySet("paid_download","http://purl.org/ontology/mo/paid_download", Document, False)
		self._props["remixed"] = PropertySet("remixed","http://purl.org/ontology/mo/remixed", Signal, False)
		self._props["sampled"] = PropertySet("sampled","http://purl.org/ontology/mo/sampled", Signal, False)
		self._props["supporting_musician"] = PropertySet("supporting_musician","http://purl.org/ontology/mo/supporting_musician", MusicArtist, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/MusicArtist"


	# Python class properties to wrap the PropertySet objects
	biography = property(fget=lambda x: x._props["biography"].get(), fset=lambda x,y : x._props["biography"].set(y), fdel=None, doc=propDocs["biography"])
	compiled = property(fget=lambda x: x._props["compiled"].get(), fset=lambda x,y : x._props["compiled"].set(y), fdel=None, doc=propDocs["compiled"])
	discography = property(fget=lambda x: x._props["discography"].get(), fset=lambda x,y : x._props["discography"].set(y), fdel=None, doc=propDocs["discography"])
	discogs = property(fget=lambda x: x._props["discogs"].get(), fset=lambda x,y : x._props["discogs"].set(y), fdel=None, doc=propDocs["discogs"])
	djmixed = property(fget=lambda x: x._props["djmixed"].get(), fset=lambda x,y : x._props["djmixed"].set(y), fdel=None, doc=propDocs["djmixed"])
	fanpage = property(fget=lambda x: x._props["fanpage"].get(), fset=lambda x,y : x._props["fanpage"].set(y), fdel=None, doc=propDocs["fanpage"])
	imdb = property(fget=lambda x: x._props["imdb"].get(), fset=lambda x,y : x._props["imdb"].set(y), fdel=None, doc=propDocs["imdb"])
	mailorder = property(fget=lambda x: x._props["mailorder"].get(), fset=lambda x,y : x._props["mailorder"].set(y), fdel=None, doc=propDocs["mailorder"])
	musicmoz = property(fget=lambda x: x._props["musicmoz"].get(), fset=lambda x,y : x._props["musicmoz"].set(y), fdel=None, doc=propDocs["musicmoz"])
	paid_download = property(fget=lambda x: x._props["paid_download"].get(), fset=lambda x,y : x._props["paid_download"].set(y), fdel=None, doc=propDocs["paid_download"])
	remixed = property(fget=lambda x: x._props["remixed"].get(), fset=lambda x,y : x._props["remixed"].set(y), fdel=None, doc=propDocs["remixed"])
	sampled = property(fget=lambda x: x._props["sampled"].get(), fset=lambda x,y : x._props["sampled"].set(y), fdel=None, doc=propDocs["sampled"])
	supporting_musician = property(fget=lambda x: x._props["supporting_musician"].get(), fset=lambda x,y : x._props["supporting_musician"].set(y), fdel=None, doc=propDocs["supporting_musician"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class MusicalExpression(Expression):
	"""
	mo:MusicalExpression
	
The intellectual or artistic realization of a work in the form of alpha-numeric, musical, or choreographic notation, sound, etc., or any combination of such forms.    


For example:

Work #1 Franz Schubert's Trout quintet

    * Expression #1 the composer's score
    * Expression #2 sound issued from the performance by the Amadeus Quartet and Hephzibah Menuhin on piano
    * Expression #3 sound issued from the performance by the Cleveland Quartet and Yo-Yo Ma on the cello
    * . . . . 

The Music Ontology defines the following sub-concepts of a MusicalExpression, which should be used instead of MusicalExpression itself: Score (the
result of an arrangement), Sound (produced during a performance), Signal. However, it is possible to stick to FRBR and bypass the worflow
mechanism this ontology defines by using the core FRBR properties on such objects. But it is often better to use events to interconnect such 
expressions (allowing to go deeply into the production process - `this performer was playing this particular instrument at that
particular time').
	
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Expression.__init__(self)
		self._initialised = False
		self.shortname = "MusicalExpression"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["discogs"] = PropertySet("discogs","http://purl.org/ontology/mo/discogs", Document, False)
		self._props["image"] = PropertySet("image","http://purl.org/ontology/mo/image", Resource, False)
		self._props["imdb"] = PropertySet("imdb","http://purl.org/ontology/mo/imdb", Document, False)
		self._props["manifestation"] = PropertySet("manifestation","http://purl.org/ontology/mo/manifestation", MusicalManifestation, False)
		self._props["musicmoz"] = PropertySet("musicmoz","http://purl.org/ontology/mo/musicmoz", Document, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/MusicalExpression"


	# Python class properties to wrap the PropertySet objects
	discogs = property(fget=lambda x: x._props["discogs"].get(), fset=lambda x,y : x._props["discogs"].set(y), fdel=None, doc=propDocs["discogs"])
	image = property(fget=lambda x: x._props["image"].get(), fset=lambda x,y : x._props["image"].set(y), fdel=None, doc=propDocs["image"])
	imdb = property(fget=lambda x: x._props["imdb"].get(), fset=lambda x,y : x._props["imdb"].set(y), fdel=None, doc=propDocs["imdb"])
	manifestation = property(fget=lambda x: x._props["manifestation"].get(), fset=lambda x,y : x._props["manifestation"].set(y), fdel=None, doc=propDocs["manifestation"])
	musicmoz = property(fget=lambda x: x._props["musicmoz"].get(), fset=lambda x,y : x._props["musicmoz"].set(y), fdel=None, doc=propDocs["musicmoz"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Performance(Event):
	"""
	mo:Performance
	
		A performance event. 
		It might include as agents performers, engineers, conductors, or even listeners.
		It might include as factors a score, a MusicalWork, musical instruments. 
		It might produce a sound:-)
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Event.__init__(self)
		self._initialised = False
		self.shortname = "Performance"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["bpm"] = PropertySet("bpm","http://purl.org/ontology/mo/bpm", None, True)
		self._props["conductor"] = PropertySet("conductor","http://purl.org/ontology/mo/conductor", Agent, False)
		self._props["engineer"] = PropertySet("engineer","http://purl.org/ontology/mo/engineer", Agent, False)
		self._props["genre"] = PropertySet("genre","http://purl.org/ontology/mo/genre", Genre, False)
		self._props["instrument"] = PropertySet("instrument","http://purl.org/ontology/mo/instrument", Instrument, False)
		self._props["key"] = PropertySet("key","http://purl.org/ontology/mo/key", Key, False)
		self._props["listener"] = PropertySet("listener","http://purl.org/ontology/mo/listener", Agent, False)
		self._props["performance_of"] = PropertySet("performance_of","http://purl.org/ontology/mo/performance_of", (Score,MusicalWork), False)
		self._props["performer"] = PropertySet("performer","http://purl.org/ontology/mo/performer", Agent, False)
		self._props["produced_sound"] = PropertySet("produced_sound","http://purl.org/ontology/mo/produced_sound", Sound, False)
		self._props["recorded_as"] = PropertySet("recorded_as","http://purl.org/ontology/mo/recorded_as", Signal, False)
		self._props["tempo"] = PropertySet("tempo","http://purl.org/ontology/mo/tempo", None, True)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Performance"


	# Python class properties to wrap the PropertySet objects
	bpm = property(fget=lambda x: x._props["bpm"].get(), fset=lambda x,y : x._props["bpm"].set(y), fdel=None, doc=propDocs["bpm"])
	conductor = property(fget=lambda x: x._props["conductor"].get(), fset=lambda x,y : x._props["conductor"].set(y), fdel=None, doc=propDocs["conductor"])
	engineer = property(fget=lambda x: x._props["engineer"].get(), fset=lambda x,y : x._props["engineer"].set(y), fdel=None, doc=propDocs["engineer"])
	genre = property(fget=lambda x: x._props["genre"].get(), fset=lambda x,y : x._props["genre"].set(y), fdel=None, doc=propDocs["genre"])
	instrument = property(fget=lambda x: x._props["instrument"].get(), fset=lambda x,y : x._props["instrument"].set(y), fdel=None, doc=propDocs["instrument"])
	key = property(fget=lambda x: x._props["key"].get(), fset=lambda x,y : x._props["key"].set(y), fdel=None, doc=propDocs["key"])
	listener = property(fget=lambda x: x._props["listener"].get(), fset=lambda x,y : x._props["listener"].set(y), fdel=None, doc=propDocs["listener"])
	performance_of = property(fget=lambda x: x._props["performance_of"].get(), fset=lambda x,y : x._props["performance_of"].set(y), fdel=None, doc=propDocs["performance_of"])
	performer = property(fget=lambda x: x._props["performer"].get(), fset=lambda x,y : x._props["performer"].set(y), fdel=None, doc=propDocs["performer"])
	produced_sound = property(fget=lambda x: x._props["produced_sound"].get(), fset=lambda x,y : x._props["produced_sound"].set(y), fdel=None, doc=propDocs["produced_sound"])
	recorded_as = property(fget=lambda x: x._props["recorded_as"].get(), fset=lambda x,y : x._props["recorded_as"].set(y), fdel=None, doc=propDocs["recorded_as"])
	tempo = property(fget=lambda x: x._props["tempo"].get(), fset=lambda x,y : x._props["tempo"].set(y), fdel=None, doc=propDocs["tempo"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class ReleaseStatus(Resource):
	"""
	mo:ReleaseStatus
	Musical manifestation release status.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "ReleaseStatus"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/ReleaseStatus"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Score(MusicalExpression):
	"""
	mo:Score
	
		Here, we are dealing with the informational object (the MusicalExpression), not the actually "published" score.
		This may be, for example, the product of an arrangement process.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		MusicalExpression.__init__(self)
		self._initialised = False
		self.shortname = "Score"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["performed_in"] = PropertySet("performed_in","http://purl.org/ontology/mo/performed_in", Performance, False)
		self._props["published_as"] = PropertySet("published_as","http://purl.org/ontology/mo/published_as", (Record,PublishedScore,PublishedLibretto,PublishedLyrics), False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Score"


	# Python class properties to wrap the PropertySet objects
	performed_in = property(fget=lambda x: x._props["performed_in"].get(), fset=lambda x,y : x._props["performed_in"].set(y), fdel=None, doc=propDocs["performed_in"])
	published_as = property(fget=lambda x: x._props["published_as"].get(), fset=lambda x,y : x._props["published_as"].set(y), fdel=None, doc=propDocs["published_as"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Sound(Event, MusicalExpression):
	"""
	mo:Sound
	
		A subclass of MusicalExpression, representing a sound. Realisation of a MusicalWork during a musical Performance.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Event.__init__(self)
		MusicalExpression.__init__(self)
		self._initialised = False
		self.shortname = "Sound"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["recorded_in"] = PropertySet("recorded_in","http://purl.org/ontology/mo/recorded_in", Recording, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Sound"


	# Python class properties to wrap the PropertySet objects
	recorded_in = property(fget=lambda x: x._props["recorded_in"].get(), fset=lambda x,y : x._props["recorded_in"].set(y), fdel=None, doc=propDocs["recorded_in"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Stream(Medium):
	"""
	mo:Stream
	Transmission over a network  used as medium to broadcast a musical manifestation
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Medium.__init__(self)
		self._initialised = False
		self.shortname = "Stream"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Stream"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Vinyl(Medium):
	"""
	mo:Vinyl
	Vinyl used as medium to record a musical manifestation
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Medium.__init__(self)
		self._initialised = False
		self.shortname = "Vinyl"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Vinyl"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Work(Resource):
	"""
	frbr:Work
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Work"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["amazon_asin"] = PropertySet("amazon_asin","http://purl.org/ontology/mo/amazon_asin", Document, False)
		self._props["licence"] = PropertySet("licence","http://purl.org/ontology/mo/licence", Document, False)
		self._props["review"] = PropertySet("review","http://purl.org/ontology/mo/review", Document, False)
		self._props["wikipedia"] = PropertySet("wikipedia","http://purl.org/ontology/mo/wikipedia", Document, False)
		self._initialised = True
	classURI = "http://purl.org/vocab/frbr/core#Work"


	# Python class properties to wrap the PropertySet objects
	amazon_asin = property(fget=lambda x: x._props["amazon_asin"].get(), fset=lambda x,y : x._props["amazon_asin"].set(y), fdel=None, doc=propDocs["amazon_asin"])
	licence = property(fget=lambda x: x._props["licence"].get(), fset=lambda x,y : x._props["licence"].set(y), fdel=None, doc=propDocs["licence"])
	review = property(fget=lambda x: x._props["review"].get(), fset=lambda x,y : x._props["review"].set(y), fdel=None, doc=propDocs["review"])
	wikipedia = property(fget=lambda x: x._props["wikipedia"].get(), fset=lambda x,y : x._props["wikipedia"].set(y), fdel=None, doc=propDocs["wikipedia"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Interval(TemporalEntity):
	"""
	time:Interval
	 A time interval (eg. "the year 1994")
	"""
	def __init__(self,URI=None):
		# Initialise parents
		TemporalEntity.__init__(self)
		self._initialised = False
		self.shortname = "Interval"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["beginsAtDateTime"] = PropertySet("beginsAtDateTime","http://purl.org/NET/c4dm/timeline.owl#beginsAtDateTime", None, True)
		self._props["beginsAtDuration"] = PropertySet("beginsAtDuration","http://purl.org/NET/c4dm/timeline.owl#beginsAtDuration", None, True)
		self._props["durationXSD"] = PropertySet("durationXSD","http://purl.org/NET/c4dm/timeline.owl#durationXSD", None, True)
		self._props["onTimeLine"] = PropertySet("onTimeLine","http://purl.org/NET/c4dm/timeline.owl#onTimeLine", TimeLine, False)
		self._props["intervalBefore"] = PropertySet("intervalBefore","http://www.w3.org/2006/time#intervalBefore", Interval, False)
		self._props["intervalDuring"] = PropertySet("intervalDuring","http://www.w3.org/2006/time#intervalDuring", (ProperInterval,Interval), False)
		self._props["intervalMeets"] = PropertySet("intervalMeets","http://www.w3.org/2006/time#intervalMeets", Interval, False)
		self._initialised = True
	classURI = "http://www.w3.org/2006/time#Interval"


	# Python class properties to wrap the PropertySet objects
	beginsAtDateTime = property(fget=lambda x: x._props["beginsAtDateTime"].get(), fset=lambda x,y : x._props["beginsAtDateTime"].set(y), fdel=None, doc=propDocs["beginsAtDateTime"])
	beginsAtDuration = property(fget=lambda x: x._props["beginsAtDuration"].get(), fset=lambda x,y : x._props["beginsAtDuration"].set(y), fdel=None, doc=propDocs["beginsAtDuration"])
	durationXSD = property(fget=lambda x: x._props["durationXSD"].get(), fset=lambda x,y : x._props["durationXSD"].set(y), fdel=None, doc=propDocs["durationXSD"])
	onTimeLine = property(fget=lambda x: x._props["onTimeLine"].get(), fset=lambda x,y : x._props["onTimeLine"].set(y), fdel=None, doc=propDocs["onTimeLine"])
	intervalBefore = property(fget=lambda x: x._props["intervalBefore"].get(), fset=lambda x,y : x._props["intervalBefore"].set(y), fdel=None, doc=propDocs["intervalBefore"])
	intervalDuring = property(fget=lambda x: x._props["intervalDuring"].get(), fset=lambda x,y : x._props["intervalDuring"].set(y), fdel=None, doc=propDocs["intervalDuring"])
	intervalMeets = property(fget=lambda x: x._props["intervalMeets"].get(), fset=lambda x,y : x._props["intervalMeets"].set(y), fdel=None, doc=propDocs["intervalMeets"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Group(Agent):
	"""
	foaf:Group
	
		A group of agents.
	
	A class of Agents.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Agent.__init__(self)
		self._initialised = False
		self.shortname = "Group"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["member"] = PropertySet("member","http://xmlns.com/foaf/0.1/member", Agent, False)
		self._initialised = True
	classURI = "http://xmlns.com/foaf/0.1/Group"


	# Python class properties to wrap the PropertySet objects
	member = property(fget=lambda x: x._props["member"].get(), fset=lambda x,y : x._props["member"].set(y), fdel=None, doc=propDocs["member"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class AbstractTimeLine(TimeLine):
	"""
	timeline:AbstractTimeLine
	
		Abstract time lines may be used as a backbone for Score, Works, ... 
		This allows for TimeLine maps to relate works to a given performance (this note was played at this time).
		No coordinate systems are defined for these timelines. Their structure is implicitly defined
		by the relations between time objects defined on them (eg. this note is before this note, which is
		before this silent, which is at the same time as this note).
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		TimeLine.__init__(self)
		self._initialised = False
		self.shortname = "AbstractTimeLine"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/NET/c4dm/timeline.owl#AbstractTimeLine"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class RelativeTimeLine(TimeLine):
	"""
	timeline:RelativeTimeLine
	
		A semi-infinite continuous timeline. Instances of RelativeTimeLine can
		back audio/video signals, sounds. Such timelines can
		be linked to a physical time line using the OriginMap.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		TimeLine.__init__(self)
		self._initialised = False
		self.shortname = "RelativeTimeLine"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/NET/c4dm/timeline.owl#RelativeTimeLine"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class AudioFile(Medium):
	"""
	mo:AudioFile
	An audio file, which may be available on a local file system or through http, ftp, etc.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Medium.__init__(self)
		self._initialised = False
		self.shortname = "AudioFile"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["encoding"] = PropertySet("encoding","http://purl.org/ontology/mo/encoding", None, True)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/AudioFile"


	# Python class properties to wrap the PropertySet objects
	encoding = property(fget=lambda x: x._props["encoding"].get(), fset=lambda x,y : x._props["encoding"].set(y), fdel=None, doc=propDocs["encoding"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Conductor(Agent):
	"""
	mo:Conductor
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Agent.__init__(self)
		self._initialised = False
		self.shortname = "Conductor"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Conductor"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class DAT(Medium):
	"""
	mo:DAT
	Data Audio Tape used as medium to record a musical manifestation.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Medium.__init__(self)
		self._initialised = False
		self.shortname = "DAT"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/DAT"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class DVDA(Medium):
	"""
	mo:DVDA
	DVD-Audio used as medium to record a musical manifestation.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Medium.__init__(self)
		self._initialised = False
		self.shortname = "DVDA"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/DVDA"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class ED2K(Medium):
	"""
	mo:ED2K
	Something available on the E-Donkey peer-2-peer filesharing network
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Medium.__init__(self)
		self._initialised = False
		self.shortname = "ED2K"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/ED2K"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Libretto(MusicalExpression):
	"""
	mo:Libretto
	
                Libretto
        
	"""
	def __init__(self,URI=None):
		# Initialise parents
		MusicalExpression.__init__(self)
		self._initialised = False
		self.shortname = "Libretto"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["published_as"] = PropertySet("published_as","http://purl.org/ontology/mo/published_as", (Record,PublishedScore,PublishedLibretto,PublishedLyrics), False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Libretto"


	# Python class properties to wrap the PropertySet objects
	published_as = property(fget=lambda x: x._props["published_as"].get(), fset=lambda x,y : x._props["published_as"].set(y), fdel=None, doc=propDocs["published_as"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class MD(Medium):
	"""
	mo:MD
	Mini Disc used as medium to record a musical manifestation.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Medium.__init__(self)
		self._initialised = False
		self.shortname = "MD"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/MD"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class MusicGroup(Group, MusicArtist):
	"""
	mo:MusicGroup
	Group of musicians, or musical ensemble, usually popular or folk, playing parts of or improvising off of a musical arrangement. 
	"""
	def __init__(self,URI=None):
		# Initialise parents
		MusicArtist.__init__(self)
		Group.__init__(self)
		self._initialised = False
		self.shortname = "MusicGroup"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/MusicGroup"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class MusicalWork(Work):
	"""
	mo:MusicalWork
	
	Distinct intellectual or artistic musical creation.
    
From the FRBR final report: A work is an abstract entity; there is no single material object one can point to as the work. We recognize the work through individual realizations or expressions of the work, but the work itself exists only in the commonality of
content between and among the various expressions of the work. When we speak of Homer's Iliad as a work, our point of reference is not a particular recitation or text of the work, but the intellectual creation that lies behind all the various expressions of the work.     

For example:

work #1 J. S. Bach's The art of the fugue

    
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Work.__init__(self)
		self._initialised = False
		self.shortname = "MusicalWork"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["arranged_in"] = PropertySet("arranged_in","http://purl.org/ontology/mo/arranged_in", Arrangement, False)
		self._props["bpm"] = PropertySet("bpm","http://purl.org/ontology/mo/bpm", None, True)
		self._props["composed_in"] = PropertySet("composed_in","http://purl.org/ontology/mo/composed_in", Composition, False)
		self._props["discogs"] = PropertySet("discogs","http://purl.org/ontology/mo/discogs", Document, False)
		self._props["has_movement"] = PropertySet("has_movement","http://purl.org/ontology/mo/has_movement", Movement, False)
		self._props["image"] = PropertySet("image","http://purl.org/ontology/mo/image", Resource, False)
		self._props["imdb"] = PropertySet("imdb","http://purl.org/ontology/mo/imdb", Document, False)
		self._props["key"] = PropertySet("key","http://purl.org/ontology/mo/key", Key, False)
		self._props["musicbrainz"] = PropertySet("musicbrainz","http://purl.org/ontology/mo/musicbrainz", Document, False)
		self._props["musicmoz"] = PropertySet("musicmoz","http://purl.org/ontology/mo/musicmoz", Document, False)
		self._props["opus"] = PropertySet("opus","http://purl.org/ontology/mo/opus", None, True)
		self._props["performed_in"] = PropertySet("performed_in","http://purl.org/ontology/mo/performed_in", Performance, False)
		self._props["tempo"] = PropertySet("tempo","http://purl.org/ontology/mo/tempo", None, True)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/MusicalWork"


	# Python class properties to wrap the PropertySet objects
	arranged_in = property(fget=lambda x: x._props["arranged_in"].get(), fset=lambda x,y : x._props["arranged_in"].set(y), fdel=None, doc=propDocs["arranged_in"])
	bpm = property(fget=lambda x: x._props["bpm"].get(), fset=lambda x,y : x._props["bpm"].set(y), fdel=None, doc=propDocs["bpm"])
	composed_in = property(fget=lambda x: x._props["composed_in"].get(), fset=lambda x,y : x._props["composed_in"].set(y), fdel=None, doc=propDocs["composed_in"])
	discogs = property(fget=lambda x: x._props["discogs"].get(), fset=lambda x,y : x._props["discogs"].set(y), fdel=None, doc=propDocs["discogs"])
	has_movement = property(fget=lambda x: x._props["has_movement"].get(), fset=lambda x,y : x._props["has_movement"].set(y), fdel=None, doc=propDocs["has_movement"])
	image = property(fget=lambda x: x._props["image"].get(), fset=lambda x,y : x._props["image"].set(y), fdel=None, doc=propDocs["image"])
	imdb = property(fget=lambda x: x._props["imdb"].get(), fset=lambda x,y : x._props["imdb"].set(y), fdel=None, doc=propDocs["imdb"])
	key = property(fget=lambda x: x._props["key"].get(), fset=lambda x,y : x._props["key"].set(y), fdel=None, doc=propDocs["key"])
	musicbrainz = property(fget=lambda x: x._props["musicbrainz"].get(), fset=lambda x,y : x._props["musicbrainz"].set(y), fdel=None, doc=propDocs["musicbrainz"])
	musicmoz = property(fget=lambda x: x._props["musicmoz"].get(), fset=lambda x,y : x._props["musicmoz"].set(y), fdel=None, doc=propDocs["musicmoz"])
	opus = property(fget=lambda x: x._props["opus"].get(), fset=lambda x,y : x._props["opus"].set(y), fdel=None, doc=propDocs["opus"])
	performed_in = property(fget=lambda x: x._props["performed_in"].get(), fset=lambda x,y : x._props["performed_in"].set(y), fdel=None, doc=propDocs["performed_in"])
	tempo = property(fget=lambda x: x._props["tempo"].get(), fset=lambda x,y : x._props["tempo"].set(y), fdel=None, doc=propDocs["tempo"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class SACD(Medium):
	"""
	mo:SACD
	Super Audio Compact Disc used as medium to record a musical manifestation.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Medium.__init__(self)
		self._initialised = False
		self.shortname = "SACD"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/SACD"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class SoundEngineer(Agent):
	"""
	mo:SoundEngineer
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Agent.__init__(self)
		self._initialised = False
		self.shortname = "SoundEngineer"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/SoundEngineer"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Item(Resource):
	"""
	frbr:Item
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "Item"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["amazon_asin"] = PropertySet("amazon_asin","http://purl.org/ontology/mo/amazon_asin", Document, False)
		self._props["wikipedia"] = PropertySet("wikipedia","http://purl.org/ontology/mo/wikipedia", Document, False)
		self._initialised = True
	classURI = "http://purl.org/vocab/frbr/core#Item"


	# Python class properties to wrap the PropertySet objects
	amazon_asin = property(fget=lambda x: x._props["amazon_asin"].get(), fset=lambda x,y : x._props["amazon_asin"].set(y), fdel=None, doc=propDocs["amazon_asin"])
	wikipedia = property(fget=lambda x: x._props["wikipedia"].get(), fset=lambda x,y : x._props["wikipedia"].set(y), fdel=None, doc=propDocs["wikipedia"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class ProperInterval(Interval):
	"""
	time:ProperInterval
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Interval.__init__(self)
		self._initialised = False
		self.shortname = "ProperInterval"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["intervalDuring"] = PropertySet("intervalDuring","http://www.w3.org/2006/time#intervalDuring", (ProperInterval,Interval), False)
		self._initialised = True
	classURI = "http://www.w3.org/2006/time#ProperInterval"


	# Python class properties to wrap the PropertySet objects
	intervalDuring = property(fget=lambda x: x._props["intervalDuring"].get(), fset=lambda x,y : x._props["intervalDuring"].set(y), fdel=None, doc=propDocs["intervalDuring"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class OriginMap(TimeLineMap):
	"""
	timeline:OriginMap
	
		This time line map represents the relation between the physical timeline and a
		continuous time line where the origin is at a given point on the physical timeline
		(eg. "the timeline backing this signal corresponds
		to the physical timeline: point 0 on this timeline corresponds to the
		20th of december at 5pm").
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		TimeLineMap.__init__(self)
		self._initialised = False
		self.shortname = "OriginMap"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["originAt"] = PropertySet("originAt","http://purl.org/NET/c4dm/timeline.owl#originAt", None, True)
		self._initialised = True
	classURI = "http://purl.org/NET/c4dm/timeline.owl#OriginMap"


	# Python class properties to wrap the PropertySet objects
	originAt = property(fget=lambda x: x._props["originAt"].get(), fset=lambda x,y : x._props["originAt"].set(y), fdel=None, doc=propDocs["originAt"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class CD(Medium):
	"""
	mo:CD
	Compact Disc used as medium to record a musical manifestation.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Medium.__init__(self)
		self._initialised = False
		self.shortname = "CD"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/CD"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class DCC(Medium):
	"""
	mo:DCC
	Digital Compact Cassette used as medium to record a musical manifestation.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Medium.__init__(self)
		self._initialised = False
		self.shortname = "DCC"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/DCC"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Instrumentation(Arrangement):
	"""
	mo:Instrumentation
	
		Instrumentation deals with the techniques of writing music for a specific instrument, 
		including the limitations of the instrument, playing techniques and idiomatic handling of the instrument.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Arrangement.__init__(self)
		self._initialised = False
		self.shortname = "Instrumentation"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Instrumentation"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Lyrics(MusicalExpression):
	"""
	mo:Lyrics
	
		Lyrics
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		MusicalExpression.__init__(self)
		self._initialised = False
		self.shortname = "Lyrics"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["published_as"] = PropertySet("published_as","http://purl.org/ontology/mo/published_as", (Record,PublishedScore,PublishedLibretto,PublishedLyrics), False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Lyrics"


	# Python class properties to wrap the PropertySet objects
	published_as = property(fget=lambda x: x._props["published_as"].get(), fset=lambda x,y : x._props["published_as"].set(y), fdel=None, doc=propDocs["published_as"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Movement(MusicalWork):
	"""
	mo:Movement
	A movement is a self-contained part of a musical work. While individual or selected movements from a composition are sometimes performed separately, a performance of the complete work requires all the movements to be performed in succession.

Often a composer attempts to interrelate the movements thematically, or sometimes in more subtle ways, in order that the individual
movements exert a cumulative effect. In some forms, composers sometimes link the movements, or ask for them to be played without a
pause between them.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		MusicalWork.__init__(self)
		self._initialised = False
		self.shortname = "Movement"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["movement_number"] = PropertySet("movement_number","http://purl.org/ontology/mo/movement_number", None, True)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Movement"


	# Python class properties to wrap the PropertySet objects
	movement_number = property(fget=lambda x: x._props["movement_number"].get(), fset=lambda x,y : x._props["movement_number"].set(y), fdel=None, doc=propDocs["movement_number"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Performer(Agent):
	"""
	mo:Performer
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Agent.__init__(self)
		self._initialised = False
		self.shortname = "Performer"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Performer"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Signal(MusicalExpression):
	"""
	mo:Signal
	
		A subclass of MusicalExpression, representing a Signal. Realisation of a MusicalWork through both a Performance and a Recording.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		MusicalExpression.__init__(self)
		self._initialised = False
		self.shortname = "Signal"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["channels"] = PropertySet("channels","http://purl.org/ontology/mo/channels", None, True)
		self._props["contains_sample_from"] = PropertySet("contains_sample_from","http://purl.org/ontology/mo/contains_sample_from", Signal, False)
		self._props["djmix_of"] = PropertySet("djmix_of","http://purl.org/ontology/mo/djmix_of", Signal, False)
		self._props["djmixed_by"] = PropertySet("djmixed_by","http://purl.org/ontology/mo/djmixed_by", MusicArtist, False)
		self._props["isrc"] = PropertySet("isrc","http://purl.org/ontology/mo/isrc", None, True)
		self._props["mashup_of"] = PropertySet("mashup_of","http://purl.org/ontology/mo/mashup_of", Signal, False)
		self._props["medley_of"] = PropertySet("medley_of","http://purl.org/ontology/mo/medley_of", Signal, False)
		self._props["musicbrainz"] = PropertySet("musicbrainz","http://purl.org/ontology/mo/musicbrainz", Document, False)
		self._props["published_as"] = PropertySet("published_as","http://purl.org/ontology/mo/published_as", (Record,PublishedScore,PublishedLibretto,PublishedLyrics), False)
		self._props["puid"] = PropertySet("puid","http://purl.org/ontology/mo/puid", None, True)
		self._props["records"] = PropertySet("records","http://purl.org/ontology/mo/records", Performance, False)
		self._props["remaster_of"] = PropertySet("remaster_of","http://purl.org/ontology/mo/remaster_of", Signal, False)
		self._props["remix_of"] = PropertySet("remix_of","http://purl.org/ontology/mo/remix_of", Signal, False)
		self._props["remixer"] = PropertySet("remixer","http://purl.org/ontology/mo/remixer", MusicArtist, False)
		self._props["sampler"] = PropertySet("sampler","http://purl.org/ontology/mo/sampler", MusicArtist, False)
		self._props["similar_to"] = PropertySet("similar_to","http://purl.org/ontology/mo/similar_to", (Agent,Signal,Genre), False)
		self._props["time"] = PropertySet("time","http://purl.org/ontology/mo/time", TemporalEntity, False)
		self._props["trmid"] = PropertySet("trmid","http://purl.org/ontology/mo/trmid", None, True)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Signal"


	# Python class properties to wrap the PropertySet objects
	channels = property(fget=lambda x: x._props["channels"].get(), fset=lambda x,y : x._props["channels"].set(y), fdel=None, doc=propDocs["channels"])
	contains_sample_from = property(fget=lambda x: x._props["contains_sample_from"].get(), fset=lambda x,y : x._props["contains_sample_from"].set(y), fdel=None, doc=propDocs["contains_sample_from"])
	djmix_of = property(fget=lambda x: x._props["djmix_of"].get(), fset=lambda x,y : x._props["djmix_of"].set(y), fdel=None, doc=propDocs["djmix_of"])
	djmixed_by = property(fget=lambda x: x._props["djmixed_by"].get(), fset=lambda x,y : x._props["djmixed_by"].set(y), fdel=None, doc=propDocs["djmixed_by"])
	isrc = property(fget=lambda x: x._props["isrc"].get(), fset=lambda x,y : x._props["isrc"].set(y), fdel=None, doc=propDocs["isrc"])
	mashup_of = property(fget=lambda x: x._props["mashup_of"].get(), fset=lambda x,y : x._props["mashup_of"].set(y), fdel=None, doc=propDocs["mashup_of"])
	medley_of = property(fget=lambda x: x._props["medley_of"].get(), fset=lambda x,y : x._props["medley_of"].set(y), fdel=None, doc=propDocs["medley_of"])
	musicbrainz = property(fget=lambda x: x._props["musicbrainz"].get(), fset=lambda x,y : x._props["musicbrainz"].set(y), fdel=None, doc=propDocs["musicbrainz"])
	published_as = property(fget=lambda x: x._props["published_as"].get(), fset=lambda x,y : x._props["published_as"].set(y), fdel=None, doc=propDocs["published_as"])
	puid = property(fget=lambda x: x._props["puid"].get(), fset=lambda x,y : x._props["puid"].set(y), fdel=None, doc=propDocs["puid"])
	records = property(fget=lambda x: x._props["records"].get(), fset=lambda x,y : x._props["records"].set(y), fdel=None, doc=propDocs["records"])
	remaster_of = property(fget=lambda x: x._props["remaster_of"].get(), fset=lambda x,y : x._props["remaster_of"].set(y), fdel=None, doc=propDocs["remaster_of"])
	remix_of = property(fget=lambda x: x._props["remix_of"].get(), fset=lambda x,y : x._props["remix_of"].set(y), fdel=None, doc=propDocs["remix_of"])
	remixer = property(fget=lambda x: x._props["remixer"].get(), fset=lambda x,y : x._props["remixer"].set(y), fdel=None, doc=propDocs["remixer"])
	sampler = property(fget=lambda x: x._props["sampler"].get(), fset=lambda x,y : x._props["sampler"].set(y), fdel=None, doc=propDocs["sampler"])
	similar_to = property(fget=lambda x: x._props["similar_to"].get(), fset=lambda x,y : x._props["similar_to"].set(y), fdel=None, doc=propDocs["similar_to"])
	time = property(fget=lambda x: x._props["time"].get(), fset=lambda x,y : x._props["time"].set(y), fdel=None, doc=propDocs["time"])
	trmid = property(fget=lambda x: x._props["trmid"].get(), fset=lambda x,y : x._props["trmid"].set(y), fdel=None, doc=propDocs["trmid"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Torrent(Medium):
	"""
	mo:Torrent
	Something available on the Bittorrent peer-2-peer filesharing network
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Medium.__init__(self)
		self._initialised = False
		self.shortname = "Torrent"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Torrent"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class SpatialThing(Resource):
	"""
	geo:SpatialThing
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Resource.__init__(self)
		self._initialised = False
		self.shortname = "SpatialThing"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["based_near"] = PropertySet("based_near","http://xmlns.com/foaf/0.1/based_near", SpatialThing, False)
		self._initialised = True
	classURI = "http://www.w3.org/2003/01/geo/wgs84_pos#SpatialThing"


	# Python class properties to wrap the PropertySet objects
	based_near = property(fget=lambda x: x._props["based_near"].get(), fset=lambda x,y : x._props["based_near"].set(y), fdel=None, doc=propDocs["based_near"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Organization(Agent):
	"""
	foaf:Organization
	An organization.
	
		An organization.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Agent.__init__(self)
		self._initialised = False
		self.shortname = "Organization"
		self.URI = URI
		self._initialised = True
	classURI = "http://xmlns.com/foaf/0.1/Organization"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class AnalogSignal(Signal):
	"""
	mo:AnalogSignal
	
		An analog signal.
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Signal.__init__(self)
		self._initialised = False
		self.shortname = "AnalogSignal"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["sampled_version"] = PropertySet("sampled_version","http://purl.org/ontology/mo/sampled_version", DigitalSignal, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/AnalogSignal"


	# Python class properties to wrap the PropertySet objects
	sampled_version = property(fget=lambda x: x._props["sampled_version"].get(), fset=lambda x,y : x._props["sampled_version"].set(y), fdel=None, doc=propDocs["sampled_version"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class DigitalSignal(Signal):
	"""
	mo:DigitalSignal
	
		A digital signal
	
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Signal.__init__(self)
		self._initialised = False
		self.shortname = "DigitalSignal"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["bitsPerSample"] = PropertySet("bitsPerSample","http://purl.org/ontology/mo/bitsPerSample", None, True)
		self._props["sample_rate"] = PropertySet("sample_rate","http://purl.org/ontology/mo/sample_rate", None, True)
		self._props["sampled_version_of"] = PropertySet("sampled_version_of","http://purl.org/ontology/mo/sampled_version_of", AnalogSignal, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/DigitalSignal"


	# Python class properties to wrap the PropertySet objects
	bitsPerSample = property(fget=lambda x: x._props["bitsPerSample"].get(), fset=lambda x,y : x._props["bitsPerSample"].set(y), fdel=None, doc=propDocs["bitsPerSample"])
	sample_rate = property(fget=lambda x: x._props["sample_rate"].get(), fset=lambda x,y : x._props["sample_rate"].set(y), fdel=None, doc=propDocs["sample_rate"])
	sampled_version_of = property(fget=lambda x: x._props["sampled_version_of"].get(), fset=lambda x,y : x._props["sampled_version_of"].set(y), fdel=None, doc=propDocs["sampled_version_of"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class MagneticTape(Medium):
	"""
	mo:MagneticTape
	Magnetic analogue tape used as medium to record a musical manifestation.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Medium.__init__(self)
		self._initialised = False
		self.shortname = "MagneticTape"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/MagneticTape"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class DateTimeInterval(ProperInterval):
	"""
	time:DateTimeInterval
	"""
	def __init__(self,URI=None):
		# Initialise parents
		ProperInterval.__init__(self)
		self._initialised = False
		self.shortname = "DateTimeInterval"
		self.URI = URI
		self._initialised = True
	classURI = "http://www.w3.org/2006/time#DateTimeInterval"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class CorporateBody(Organization):
	"""
	mo:CorporateBody
	Organization or group of individuals and/or other organizations involved in the music market.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Organization.__init__(self)
		self._initialised = False
		self.shortname = "CorporateBody"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["discogs"] = PropertySet("discogs","http://purl.org/ontology/mo/discogs", Document, False)
		self._props["download"] = PropertySet("download","http://purl.org/ontology/mo/download", Document, False)
		self._props["free_download"] = PropertySet("free_download","http://purl.org/ontology/mo/free_download", Document, False)
		self._props["imdb"] = PropertySet("imdb","http://purl.org/ontology/mo/imdb", Document, False)
		self._props["mailorder"] = PropertySet("mailorder","http://purl.org/ontology/mo/mailorder", Document, False)
		self._props["paid_download"] = PropertySet("paid_download","http://purl.org/ontology/mo/paid_download", Document, False)
		self._props["preview_download"] = PropertySet("preview_download","http://purl.org/ontology/mo/preview_download", Document, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/CorporateBody"


	# Python class properties to wrap the PropertySet objects
	discogs = property(fget=lambda x: x._props["discogs"].get(), fset=lambda x,y : x._props["discogs"].set(y), fdel=None, doc=propDocs["discogs"])
	download = property(fget=lambda x: x._props["download"].get(), fset=lambda x,y : x._props["download"].set(y), fdel=None, doc=propDocs["download"])
	free_download = property(fget=lambda x: x._props["free_download"].get(), fset=lambda x,y : x._props["free_download"].set(y), fdel=None, doc=propDocs["free_download"])
	imdb = property(fget=lambda x: x._props["imdb"].get(), fset=lambda x,y : x._props["imdb"].set(y), fdel=None, doc=propDocs["imdb"])
	mailorder = property(fget=lambda x: x._props["mailorder"].get(), fset=lambda x,y : x._props["mailorder"].set(y), fdel=None, doc=propDocs["mailorder"])
	paid_download = property(fget=lambda x: x._props["paid_download"].get(), fset=lambda x,y : x._props["paid_download"].set(y), fdel=None, doc=propDocs["paid_download"])
	preview_download = property(fget=lambda x: x._props["preview_download"].get(), fset=lambda x,y : x._props["preview_download"].set(y), fdel=None, doc=propDocs["preview_download"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class MusicalManifestation(Manifestation):
	"""
	mo:MusicalManifestation
	

This entity is related to the edition/production/publication of a musical expression (musical manifestation are closely related with the music industry (their terms, concepts, definitions, methods (production, publication, etc.), etc.)
    
From the FRBR final report: The entity defined as manifestation encompasses a wide range of materials, including manuscripts, books, periodicals, maps, posters, sound recordings, films, video recordings, CD-ROMs, multimedia kits, etc. As an entity, manifestation represents all the physical objects that bear the same characteristics, in respect to both intellectual content and physical form.


Work #1 J. S. Bach's Six suites for unaccompanied cello

    * Expression #1 sound issued during the performance by Janos Starker recorded in 1963 and 1965
          o Manifestation #1 recordings released on 33 1/3 rpm sound discs in 1965 by Mercury
          o Manifestation #2 recordings re-released on compact disc in 1991 by Mercury 
    * Expression #2 sound issued during the performances by Yo-Yo Ma recorded in 1983
          o Manifestation #1 recordings released on 33 1/3 rpm sound discs in 1983 by CBS Records
          o Manifestation #2 recordings re-released on compact disc in 1992 by CBS Records 

          
Changes that occur deliberately or even inadvertently in the production process that affect the copies result, strictly speaking, in a new manifestation. A manifestation resulting from such a change may be identified as a particular "state" or "issue" of the publication.

Changes that occur to an individual copy after the production process is complete (e.g., the loss of a page, rebinding, etc.) are not considered to result in a new manifestation. That copy is simply considered to be an exemplar (or item) of the manifestation that deviates from the copy as produced.

With the entity defined as manifestation we can describe the physical characteristics of a set of items and the characteristics associated with the production and distribution of that set of items that may be important factors in enabling users to choose a manifestation appropriate to their physical needs and constraints, and to identify and acquire a copy of that manifestation.

Defining manifestation as an entity also enables us to draw relationships between specific manifestations of a work. We can use the relationships between manifestations to identify, for example, the specific publication that was used to create a microreproduction.          


	"""
	def __init__(self,URI=None):
		# Initialise parents
		Manifestation.__init__(self)
		self._initialised = False
		self.shortname = "MusicalManifestation"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["available_as"] = PropertySet("available_as","http://purl.org/ontology/mo/available_as", MusicalItem, False)
		self._props["compilation_of"] = PropertySet("compilation_of","http://purl.org/ontology/mo/compilation_of", Signal, False)
		self._props["compiler"] = PropertySet("compiler","http://purl.org/ontology/mo/compiler", MusicArtist, False)
		self._props["discogs"] = PropertySet("discogs","http://purl.org/ontology/mo/discogs", Document, False)
		self._props["image"] = PropertySet("image","http://purl.org/ontology/mo/image", Resource, False)
		self._props["imdb"] = PropertySet("imdb","http://purl.org/ontology/mo/imdb", Document, False)
		self._props["item"] = PropertySet("item","http://purl.org/ontology/mo/item", MusicalItem, False)
		self._props["musicbrainz"] = PropertySet("musicbrainz","http://purl.org/ontology/mo/musicbrainz", Document, False)
		self._props["musicmoz"] = PropertySet("musicmoz","http://purl.org/ontology/mo/musicmoz", Document, False)
		self._props["other_release_of"] = PropertySet("other_release_of","http://purl.org/ontology/mo/other_release_of", MusicalManifestation, False)
		self._props["preview"] = PropertySet("preview","http://purl.org/ontology/mo/preview", MusicalItem, False)
		self._props["producer"] = PropertySet("producer","http://purl.org/ontology/mo/producer", Agent, False)
		self._props["publisher"] = PropertySet("publisher","http://purl.org/ontology/mo/publisher", Agent, False)
		self._props["publishing_location"] = PropertySet("publishing_location","http://purl.org/ontology/mo/publishing_location", SpatialThing, False)
		self._props["release_status"] = PropertySet("release_status","http://purl.org/ontology/mo/release_status", ReleaseStatus, False)
		self._props["release_type"] = PropertySet("release_type","http://purl.org/ontology/mo/release_type", ReleaseType, False)
		self._props["tribute_to"] = PropertySet("tribute_to","http://purl.org/ontology/mo/tribute_to", MusicArtist, False)
		self._props["maker"] = PropertySet("maker","http://xmlns.com/foaf/0.1/maker", Agent, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/MusicalManifestation"


	# Python class properties to wrap the PropertySet objects
	available_as = property(fget=lambda x: x._props["available_as"].get(), fset=lambda x,y : x._props["available_as"].set(y), fdel=None, doc=propDocs["available_as"])
	compilation_of = property(fget=lambda x: x._props["compilation_of"].get(), fset=lambda x,y : x._props["compilation_of"].set(y), fdel=None, doc=propDocs["compilation_of"])
	compiler = property(fget=lambda x: x._props["compiler"].get(), fset=lambda x,y : x._props["compiler"].set(y), fdel=None, doc=propDocs["compiler"])
	discogs = property(fget=lambda x: x._props["discogs"].get(), fset=lambda x,y : x._props["discogs"].set(y), fdel=None, doc=propDocs["discogs"])
	image = property(fget=lambda x: x._props["image"].get(), fset=lambda x,y : x._props["image"].set(y), fdel=None, doc=propDocs["image"])
	imdb = property(fget=lambda x: x._props["imdb"].get(), fset=lambda x,y : x._props["imdb"].set(y), fdel=None, doc=propDocs["imdb"])
	item = property(fget=lambda x: x._props["item"].get(), fset=lambda x,y : x._props["item"].set(y), fdel=None, doc=propDocs["item"])
	musicbrainz = property(fget=lambda x: x._props["musicbrainz"].get(), fset=lambda x,y : x._props["musicbrainz"].set(y), fdel=None, doc=propDocs["musicbrainz"])
	musicmoz = property(fget=lambda x: x._props["musicmoz"].get(), fset=lambda x,y : x._props["musicmoz"].set(y), fdel=None, doc=propDocs["musicmoz"])
	other_release_of = property(fget=lambda x: x._props["other_release_of"].get(), fset=lambda x,y : x._props["other_release_of"].set(y), fdel=None, doc=propDocs["other_release_of"])
	preview = property(fget=lambda x: x._props["preview"].get(), fset=lambda x,y : x._props["preview"].set(y), fdel=None, doc=propDocs["preview"])
	producer = property(fget=lambda x: x._props["producer"].get(), fset=lambda x,y : x._props["producer"].set(y), fdel=None, doc=propDocs["producer"])
	publisher = property(fget=lambda x: x._props["publisher"].get(), fset=lambda x,y : x._props["publisher"].set(y), fdel=None, doc=propDocs["publisher"])
	publishing_location = property(fget=lambda x: x._props["publishing_location"].get(), fset=lambda x,y : x._props["publishing_location"].set(y), fdel=None, doc=propDocs["publishing_location"])
	release_status = property(fget=lambda x: x._props["release_status"].get(), fset=lambda x,y : x._props["release_status"].set(y), fdel=None, doc=propDocs["release_status"])
	release_type = property(fget=lambda x: x._props["release_type"].get(), fset=lambda x,y : x._props["release_type"].set(y), fdel=None, doc=propDocs["release_type"])
	tribute_to = property(fget=lambda x: x._props["tribute_to"].get(), fset=lambda x,y : x._props["tribute_to"].set(y), fdel=None, doc=propDocs["tribute_to"])
	maker = property(fget=lambda x: x._props["maker"].get(), fset=lambda x,y : x._props["maker"].set(y), fdel=None, doc=propDocs["maker"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class PublishedLyrics(MusicalManifestation):
	"""
	mo:PublishedLyrics
	Published lyrics, as a book or as a text file, for example
	"""
	def __init__(self,URI=None):
		# Initialise parents
		MusicalManifestation.__init__(self)
		self._initialised = False
		self.shortname = "PublishedLyrics"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["publication_of"] = PropertySet("publication_of","http://purl.org/ontology/mo/publication_of", (Signal,Score,Libretto,Lyrics), False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/PublishedLyrics"


	# Python class properties to wrap the PropertySet objects
	publication_of = property(fget=lambda x: x._props["publication_of"].get(), fset=lambda x,y : x._props["publication_of"].set(y), fdel=None, doc=propDocs["publication_of"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Record(MusicalManifestation):
	"""
	mo:Record
	A published record (manifestation which first aim is to render the product of a recording)
	"""
	def __init__(self,URI=None):
		# Initialise parents
		MusicalManifestation.__init__(self)
		self._initialised = False
		self.shortname = "Record"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["publication_of"] = PropertySet("publication_of","http://purl.org/ontology/mo/publication_of", (Signal,Score,Libretto,Lyrics), False)
		self._props["track"] = PropertySet("track","http://purl.org/ontology/mo/track", Track, False)
		self._props["maker"] = PropertySet("maker","http://xmlns.com/foaf/0.1/maker", Agent, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Record"


	# Python class properties to wrap the PropertySet objects
	publication_of = property(fget=lambda x: x._props["publication_of"].get(), fset=lambda x,y : x._props["publication_of"].set(y), fdel=None, doc=propDocs["publication_of"])
	track = property(fget=lambda x: x._props["track"].get(), fset=lambda x,y : x._props["track"].set(y), fdel=None, doc=propDocs["track"])
	maker = property(fget=lambda x: x._props["maker"].get(), fset=lambda x,y : x._props["maker"].set(y), fdel=None, doc=propDocs["maker"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Track(Record):
	"""
	mo:Track
	A track on a particular record
	"""
	def __init__(self,URI=None):
		# Initialise parents
		Record.__init__(self)
		self._initialised = False
		self.shortname = "Track"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["olga"] = PropertySet("olga","http://purl.org/ontology/mo/olga", Document, False)
		self._props["track_number"] = PropertySet("track_number","http://purl.org/ontology/mo/track_number", None, True)
		self._props["maker"] = PropertySet("maker","http://xmlns.com/foaf/0.1/maker", Agent, False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Track"


	# Python class properties to wrap the PropertySet objects
	olga = property(fget=lambda x: x._props["olga"].get(), fset=lambda x,y : x._props["olga"].set(y), fdel=None, doc=propDocs["olga"])
	track_number = property(fget=lambda x: x._props["track_number"].get(), fset=lambda x,y : x._props["track_number"].set(y), fdel=None, doc=propDocs["track_number"])
	maker = property(fget=lambda x: x._props["maker"].get(), fset=lambda x,y : x._props["maker"].set(y), fdel=None, doc=propDocs["maker"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Label(CorporateBody):
	"""
	mo:Label
	Trade name of a company that produces musical works or expression of musical works.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		CorporateBody.__init__(self)
		self._initialised = False
		self.shortname = "Label"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/Label"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class PublishedScore(MusicalManifestation):
	"""
	mo:PublishedScore
	A published score (subclass of MusicalManifestation)
	"""
	def __init__(self,URI=None):
		# Initialise parents
		MusicalManifestation.__init__(self)
		self._initialised = False
		self.shortname = "PublishedScore"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["publication_of"] = PropertySet("publication_of","http://purl.org/ontology/mo/publication_of", (Signal,Score,Libretto,Lyrics), False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/PublishedScore"


	# Python class properties to wrap the PropertySet objects
	publication_of = property(fget=lambda x: x._props["publication_of"].get(), fset=lambda x,y : x._props["publication_of"].set(y), fdel=None, doc=propDocs["publication_of"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class Person(Agent, SpatialThing):
	"""
	foaf:Person
	
		A person.
	
	A person.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		SpatialThing.__init__(self)
		Agent.__init__(self)
		self._initialised = False
		self.shortname = "Person"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["member_of"] = PropertySet("member_of","http://purl.org/ontology/mo/member_of", Group, False)
		self._props["currentProject"] = PropertySet("currentProject","http://xmlns.com/foaf/0.1/currentProject", Thing, False)
		self._props["family_name"] = PropertySet("family_name","http://xmlns.com/foaf/0.1/family_name", None, True)
		self._props["firstName"] = PropertySet("firstName","http://xmlns.com/foaf/0.1/firstName", None, True)
		self._props["geekcode"] = PropertySet("geekcode","http://xmlns.com/foaf/0.1/geekcode", None, True)
		self._props["img"] = PropertySet("img","http://xmlns.com/foaf/0.1/img", Image, False)
		self._props["interest"] = PropertySet("interest","http://xmlns.com/foaf/0.1/interest", Document, False)
		self._props["knows"] = PropertySet("knows","http://xmlns.com/foaf/0.1/knows", Person, False)
		self._props["myersBriggs"] = PropertySet("myersBriggs","http://xmlns.com/foaf/0.1/myersBriggs", None, True)
		self._props["pastProject"] = PropertySet("pastProject","http://xmlns.com/foaf/0.1/pastProject", Thing, False)
		self._props["plan"] = PropertySet("plan","http://xmlns.com/foaf/0.1/plan", None, True)
		self._props["publications"] = PropertySet("publications","http://xmlns.com/foaf/0.1/publications", Document, False)
		self._props["schoolHomepage"] = PropertySet("schoolHomepage","http://xmlns.com/foaf/0.1/schoolHomepage", Document, False)
		self._props["surname"] = PropertySet("surname","http://xmlns.com/foaf/0.1/surname", None, True)
		self._props["topic_interest"] = PropertySet("topic_interest","http://xmlns.com/foaf/0.1/topic_interest", Thing, False)
		self._props["workInfoHomepage"] = PropertySet("workInfoHomepage","http://xmlns.com/foaf/0.1/workInfoHomepage", Document, False)
		self._props["workplaceHomepage"] = PropertySet("workplaceHomepage","http://xmlns.com/foaf/0.1/workplaceHomepage", Document, False)
		self._initialised = True
	classURI = "http://xmlns.com/foaf/0.1/Person"


	# Python class properties to wrap the PropertySet objects
	member_of = property(fget=lambda x: x._props["member_of"].get(), fset=lambda x,y : x._props["member_of"].set(y), fdel=None, doc=propDocs["member_of"])
	currentProject = property(fget=lambda x: x._props["currentProject"].get(), fset=lambda x,y : x._props["currentProject"].set(y), fdel=None, doc=propDocs["currentProject"])
	family_name = property(fget=lambda x: x._props["family_name"].get(), fset=lambda x,y : x._props["family_name"].set(y), fdel=None, doc=propDocs["family_name"])
	firstName = property(fget=lambda x: x._props["firstName"].get(), fset=lambda x,y : x._props["firstName"].set(y), fdel=None, doc=propDocs["firstName"])
	geekcode = property(fget=lambda x: x._props["geekcode"].get(), fset=lambda x,y : x._props["geekcode"].set(y), fdel=None, doc=propDocs["geekcode"])
	img = property(fget=lambda x: x._props["img"].get(), fset=lambda x,y : x._props["img"].set(y), fdel=None, doc=propDocs["img"])
	interest = property(fget=lambda x: x._props["interest"].get(), fset=lambda x,y : x._props["interest"].set(y), fdel=None, doc=propDocs["interest"])
	knows = property(fget=lambda x: x._props["knows"].get(), fset=lambda x,y : x._props["knows"].set(y), fdel=None, doc=propDocs["knows"])
	myersBriggs = property(fget=lambda x: x._props["myersBriggs"].get(), fset=lambda x,y : x._props["myersBriggs"].set(y), fdel=None, doc=propDocs["myersBriggs"])
	pastProject = property(fget=lambda x: x._props["pastProject"].get(), fset=lambda x,y : x._props["pastProject"].set(y), fdel=None, doc=propDocs["pastProject"])
	plan = property(fget=lambda x: x._props["plan"].get(), fset=lambda x,y : x._props["plan"].set(y), fdel=None, doc=propDocs["plan"])
	publications = property(fget=lambda x: x._props["publications"].get(), fset=lambda x,y : x._props["publications"].set(y), fdel=None, doc=propDocs["publications"])
	schoolHomepage = property(fget=lambda x: x._props["schoolHomepage"].get(), fset=lambda x,y : x._props["schoolHomepage"].set(y), fdel=None, doc=propDocs["schoolHomepage"])
	surname = property(fget=lambda x: x._props["surname"].get(), fset=lambda x,y : x._props["surname"].set(y), fdel=None, doc=propDocs["surname"])
	topic_interest = property(fget=lambda x: x._props["topic_interest"].get(), fset=lambda x,y : x._props["topic_interest"].set(y), fdel=None, doc=propDocs["topic_interest"])
	workInfoHomepage = property(fget=lambda x: x._props["workInfoHomepage"].get(), fset=lambda x,y : x._props["workInfoHomepage"].set(y), fdel=None, doc=propDocs["workInfoHomepage"])
	workplaceHomepage = property(fget=lambda x: x._props["workplaceHomepage"].get(), fset=lambda x,y : x._props["workplaceHomepage"].set(y), fdel=None, doc=propDocs["workplaceHomepage"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class PublishedLibretto(MusicalManifestation):
	"""
	mo:PublishedLibretto
	A published libretto
	"""
	def __init__(self,URI=None):
		# Initialise parents
		MusicalManifestation.__init__(self)
		self._initialised = False
		self.shortname = "PublishedLibretto"
		self.URI = URI
		self._props = getattr(self,"_props",{}) # Initialise if a parent class hasn't already
		self._props["publication_of"] = PropertySet("publication_of","http://purl.org/ontology/mo/publication_of", (Signal,Score,Libretto,Lyrics), False)
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/PublishedLibretto"


	# Python class properties to wrap the PropertySet objects
	publication_of = property(fget=lambda x: x._props["publication_of"].get(), fset=lambda x,y : x._props["publication_of"].set(y), fdel=None, doc=propDocs["publication_of"])

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr

class SoloMusicArtist(MusicArtist, Person):
	"""
	mo:SoloMusicArtist
	Single person whose musical creative work shows sensitivity and imagination.
	"""
	def __init__(self,URI=None):
		# Initialise parents
		MusicArtist.__init__(self)
		Person.__init__(self)
		self._initialised = False
		self.shortname = "SoloMusicArtist"
		self.URI = URI
		self._initialised = True
	classURI = "http://purl.org/ontology/mo/SoloMusicArtist"

	# Utility methods
	__setattr__ = protector
	__str__ = objToStr


# ======================= Instance Definitions ======================= 

universaltimeline = PhysicalTimeLine("http://purl.org/NET/c4dm/timeline.owl#universaltimeline")
universaltimeline.description = \
"""The "canonical" physical time-line, on which points/intervals are addressed through UTC.
		(Remember: we do here the amalgam between timelines and coordinate systems, as we 
		choose one canonical one per timeline)."""

bootleg = ReleaseStatus("http://purl.org/ontology/mo/bootleg")
bootleg.description = \
"""An unofficial/underground musical work or the expression of a musical work that was not sanctioned by the artist and/or the corporate body."""
official = ReleaseStatus("http://purl.org/ontology/mo/official")
official.description = \
"""Any musical work or the expression of a musical work officially sanctioned by the artist and/or their corporate body."""
promotion = ReleaseStatus("http://purl.org/ontology/mo/promotion")
promotion.description = \
"""A giveaway musical work or the expression of a musical work intended to promote an upcoming official musical work or the expression of a musical work."""

audiobook = ReleaseType("http://purl.org/ontology/mo/audiobook")
audiobook.description = \
"""Book read by a narrator without music.
		This is a type of MusicalManifestation defined by the musical industry."""
compilation = ReleaseType("http://purl.org/ontology/mo/compilation")
compilation.description = \
"""Collection of previously released manifestations of a musical expression by one or more artists.
		This is a type of MusicalManifestation defined by the musical industry."""
interview = ReleaseType("http://purl.org/ontology/mo/interview")
interview.description = \
"""Recording of the questioning of a person.
		This is a type of MusicalManifestation defined by the musical industry."""
ep = ReleaseType("http://purl.org/ontology/mo/ep")
ep.description = \
"""An EP"""
soundtrack = ReleaseType("http://purl.org/ontology/mo/soundtrack")
soundtrack.description = \
"""Sound recording on a narrow strip of a motion picture film.
		This is a type of MusicalManifestation defined by the musical industry."""
album = ReleaseType("http://purl.org/ontology/mo/album")
album.description = \
"""One or more track issued together.
    		This is a type of MusicalManifestation defined by the musical industry."""
remix = ReleaseType("http://purl.org/ontology/mo/remix")
remix.description = \
"""Musical manifestation that primarily contains remixed material. 
		This is a type of MusicalManifestation defined by the musical industry."""
spokenword = ReleaseType("http://purl.org/ontology/mo/spokenword")
spokenword.description = \
"""Spoken word is a form of music or artistic performance in which lyrics, poetry, or stories are spoken rather than sung. 
		Spoken-word is often done with a musical background, but emphasis is kept on the speaker.
		This is a type of MusicalManifestation defined by the musical industry."""
live = ReleaseType("http://purl.org/ontology/mo/live")
live.description = \
"""A musical manifestation that was recorded live.
		This is a type of MusicalManifestation defined by the musical industry."""

namespaceBindings = {"xml":"http://www.w3.org/XML/1998/namespace","owl":"http://www.w3.org/2002/07/owl#","key":"http://purl.org/NET/c4dm/keys.owl#","rdfs":"http://www.w3.org/2000/01/rdf-schema#","timeline":"http://purl.org/NET/c4dm/timeline.owl#","mo":"http://purl.org/ontology/mo/","vs":"http://www.w3.org/2003/06/sw-vocab-status/ns#","dc":"http://purl.org/dc/elements/1.1/","frbr":"http://purl.org/vocab/frbr/core#","rdf":"http://www.w3.org/1999/02/22-rdf-syntax-ns#","foaf":"http://xmlns.com/foaf/0.1/","bio":"http://vocab.org/bio/0.1/","time":"http://www.w3.org/2006/time#","wot":"http://xmlns.com/wot/0.1/","geo":"http://www.w3.org/2003/01/geo/wgs84_pos#","event":"http://purl.org/NET/c4dm/event.owl#"}



# =======================       Clean Up       ======================= 
del objToStr, propDocs
