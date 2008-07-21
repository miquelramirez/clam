from Pool import *
from Schema import *
from Aggregator import *
import shelve
import os



class FileMetadataSource :
	class SongNotFoundException :
		def __init__(self, id) :
			self.id = id
		def __str__(self) :
			return "No descriptors found for song id '%s'." % self.id

	class InvalidPathException :
		def __init__(self, path) :
			self.path = path
		def __str__(self) :
			return "No schema 'schema.sc' found at '%s'." % self.path

	class InvalidExtractorPathException :
		def __init__(self, path) :
			self.path = path
		def __str__(self) :
			return "The extractor '%s' cannot be executed. Check its existence and permissions." % self.path

	def __init__(self, path, extractor=None) :
		self.path = path
		self.idsToRecalculate = []
		if extractor and not os.access(extractor, os.X_OK) :
			raise FileMetadataSource.InvalidExtractorPathException(extractor)
		self.extractor = extractor
		if extractor and not os.access(self.path + "schema.sc", os.R_OK) :
			os.system("%s -s %s"%(self.extractor, os.path.join(self.path,"schema.sc")))
		try :
			self.schema = Schema(file(self.path + "schema.sc"))
		except IOError, e :
			raise FileMetadataSource.InvalidPathException(path)

	def QueryDescriptors(self, id, ignoreCache=False, computeIfNotCached=False, keepCache=True) :
		if not ignoreCache:
			try :
				return Pool(file(self._poolPath(id)))
			except IOError, e : pass # Not found
		path=id
		linkName = os.path.join(self.path, id)
		try :
			os.symlink(path, linkName)
		except: pass
		command = "%s -f .pool %s"%(self.extractor, linkName)
		os.popen(command, 'r').read()
		try :
			return Pool(file(self._poolPath(id)))
		except IOError, e : pass # Not found
		raise FileMetadataSource.SongNotFoundException(id)

	def QuerySchema(self) :
		return self.schema

	def UpdateDescriptors(self, id, pool) :
		if str(id) not in self.idsToRecalculate:
			self.idsToRecalculate.append(id)
		# TODO: Merge when data already exists
		pool.Dump(file(self._poolPath(id),"w"))

	def CheckMissingDescriptor(self, descriptor, idlist):
		available = shelve.open( self.path + "/available.dict", writeback=True )
		result = []

		for id in idlist:
			id = str(id)

			if id in self.idsToRecalculate:
				try:
					del available[descriptor][id]
					available[descriptor][id]=None
				except:
					pass
				try:
					songpool = self.QueryDescriptors( id )
				except FileMetadataSource.SongNotFoundException:
					available[descriptor][id]=False
					result.append( id )
					continue

				if descriptor not in songpool.PresentAttributes():
					result.append( id )
					available[descriptor][id]=False
				else:
					available[descriptor][id]=True

				continue

			if descriptor in available.keys():
				if id in available[descriptor].keys():
					#Everything existed! next id, please
					if not available[descriptor][id]:
						result.append( id )
					continue
				else:
					available[descriptor][id]=None
			else:
				available[descriptor]={}
				available[descriptor][id]=None

			#Calculate phase: see available and store it
			try:
				songpool = self.QueryDescriptors( id )
			except FileMetadataSource.SongNotFoundException:
				available[descriptor][id]=False
				result.append( id )
				continue

			if descriptor not in songpool.PresentAttributes():
				result.append( id )
				available[descriptor][id]=False
			else:
				available[descriptor][id]=True

		available.close()
			
		return result
	
	def _poolPath(self, id) :
		return self.path + id + ".pool";
