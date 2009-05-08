#!/usr/bin/env python
# -*- coding: utf-8 -*-
# CMPC2G04 Game Coursework
# George Kettleborough <g.kettleborough@uea.ac.uk>
# UEA registration number: 3316173
# 
# outputs XML for the worldfile from raw tile data

import sys
from optparse import OptionParser

# optparse
parser = OptionParser()

(options, args) = parser.parse_args()
if len(args) != 2:
    print 'Usage: maptiles2xml TEXFILE TANGFILE'
    sys.exit(1)

texfile = open(args[0], 'r')
tanfile = open(args[1], 'r')

# read each character
texchar = texfile.read(1)
tanchar = tanfile.read(1)
while texchar != "" and tanchar != "":
    if texchar != "\n" and tanchar != "\n":
        # print the xml element
        print "<t id=\""+texchar+"\" x=\""+tanchar+"\" />"
    texchar = texfile.read(1)
    tanchar = tanfile.read(1)

