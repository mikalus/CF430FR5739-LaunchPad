#coding=utf-8
'''
Created on 12.02.2012

@author: christopherkalus
'''
import os
import glob
import codecs

path=str(os.getcwd())
outfile=codecs.open("Glossary.txt","w","utf-8")
for file in glob.glob(os.path.join(path, '*.s43')):
    infile=codecs.open(file)
    (pfad, filename)=os.path.split(file)
    print >> outfile, "\n"+str(filename) 
    for zeile in infile:
        if ";C" in zeile or";X" in zeile or";Z" in zeile or";U" in zeile or";A" in zeile or "===" in zeile:
            zeile = str(zeile).strip().split("  ")
            zeile2=""
            for element in range(len(zeile)):
                if zeile[element]=='':
                    continue
                else:
                    zeile2= zeile2 + (zeile[element]) +"  "
            zeile2=zeile2.split("  ")          
            if len(zeile2)>=3:
                print >> outfile, zeile2[0].strip().ljust(16) + zeile2[1].strip(" ").ljust(30) + zeile2[2].strip(),
            else: 
                for element in zeile2: 
                    if "===" in element:      
                        print >> outfile, element.ljust(80,"="),
                    else:
                        print >> outfile, element,
            print >> outfile, ""
    infile.close()
outfile.close()
