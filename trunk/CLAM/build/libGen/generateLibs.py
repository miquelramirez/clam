#!/usr/bin/python

import CLAMcore
import CLAMProcessing
import CLAMIO
import CLAMSMS
import CLAMVM
import SettingsGen
import os

def main() :
    outPath = "../Libs"

    CLAMcore.makeAutoconf( outPath )
    CLAMProcessing.makeAutoconf( outPath )
    CLAMIO.makeAutoconf( outPath )
    CLAMSMS.makeAutoconf( outPath )
    CLAMVM.makeAutoconf( outPath )
    
    SettingsGen.resetHeaderDB()
  
    CLAMcore.makeSettings(outPath)
    CLAMProcessing.makeSettings(outPath)
    CLAMIO.makeSettings(outPath)
    CLAMSMS.makeSettings(outPath)
    CLAMVM.makeSettings(outPath)

if __name__ == "__main__" :
    main()
else :
    print "This is an executable script"
    sys.exit(2)
