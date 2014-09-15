#!/usr/bin/env python
import os
import xml.etree.ElementTree as ET
#from termcolor import colored

def getLFN( file_name ) :
  # parse xml file
#!/usr/bin/env python
import os
import xml.etree.ElementTree as ET
#from termcolor import colored

def getLFN( file_name ) :
  # parse xml file
  if ( os.path.getsize( file_name) ==0 ) :
    print "Oops, this file is empty : %s"%(file_name)
    return ""
  doc = ET.parse(file_name)

  # get root node
  root = doc.getroot()
  c = root.find("File")
  if ( c is None ) :
    print "Error! Key 'file' can not be available on %s"%file_name
    return ''
  else :
    d = c.find("LFN")
    if ( d is None) :
      print "Error! Key 'LFN' can not be available on %s"%file_name
      return ''
    else :
      LFN = d.text
      if ( LFN is None) :
        print "Error! text attribute is None on %s"%file_name
        return ''
      else :
        LFN_filename = d.text.strip()
        return LFN_filename

list = os.listdir('.')
dirs = []
for dir in list :
  if ( os.access(dir+'/res',os.X_OK) ) :
    dirs.append(dir)
print "Hello, I found ", len(dirs)," crab subdirectories."

fjr_files =[]
for i,dir in enumerate(dirs) :
  print "[%d/%d]"%(i+1,len(dirs))+"Let's go to "+dir
  output = open("filelist_"+dir,"w")
  files = os.listdir(dir+'/res')
  for file in files :
    if ( file.find("crab_fjr") != -1)  :
      filename = dir+'/res/'+file
      out = getLFN(filename)
      output.write( out+'\n')
