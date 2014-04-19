
#!/usr/bin/env python

import os
import sys


# Setup environ
os.environ['DJANGO_SETTINGS_MODULE'] = "research.settings"

from quickstart.models import TimeSeries, Person

from datetime import datetime

# f = file('wstawanie1.txt', 'r')

import glob


try:
    Person.objects.create(id=1,name="Dziadek",age=98, gender="male")
except:
    pass


files = glob.glob('./*.txt')

for f in files:
    f = file(f, 'r')


    for line in f:
        # split the line into a list of column values
        columns = line.split(' ')
        # clean any whitespace off the items
        columns = [col.strip() for col in columns]

        # ensure the column has at least one value before printing
        print len(columns)
        if columns:
            print "xvalue", columns[1]
            print "yvalue", columns[2]
            print "zvalue", columns[3]

            xvalue = columns[1]
            yvalue = columns[2]
            zvalue = columns[3]


            now = columns[0]
            #tag = f.name.strip(".txt")

            ps = Person.objects.get(id=1)

            ts = TimeSeries(person=ps,xvalue=xvalue,yvalue=yvalue,zvalue=zvalue,qvalue=0,wvalue=0,evalue=0,when=now,device="nano")
            ts.save()
            #ts.tags.add(tag)
            #ts.save()


