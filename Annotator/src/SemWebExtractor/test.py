#!/usr/bin/python

from SemWebCrawler import *
import sys
import os

#sc = SemWebCrawler('C:\Jun\data\Simple Exercice.mp3')
#sc = SemWebCrawler('C:\Jun\data\Crash Into June.mp3')
sc = SemWebCrawler('C:\Jun\data\mud.mp3')
metaData = sc.spider()
##print metaData

