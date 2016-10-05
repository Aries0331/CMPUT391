complie and execution:
for complie, type "gcc -o <filename> <filename>.c sqlite3.c -lpthread -ldl" and then "./<filename> as1.db" 
for q4 and q5, please first do 
 * Linux:
   gcc -fPIC -lm -shared extension-functions.c -o libsqlitefunctions.so
 * Mac OS X:
   gcc -fno-common -dynamiclib extension-functions.c -o libsqlitefunctions.dylib
 (You may need to add flags
  -I /opt/local/include/ -L/opt/local/lib -lsqlite3
  if your sqlite3 is installed from Mac ports, or
  -I /sw/include/ -L/sw/lib -lsqlite3
  if installed with Fink.)
 to load the extension
 **** somehow fail to run the extension on lab machine, but definitely can run my Mac OS X laptop, if needed, I can show you the result on my laptop. Thank you!


librarlies:
<studio.h>
<stdlib.h>
<string.h>
<sqlite3.h>
and in q4 and q5 import a sqlite extension c file

collaborations:
collaborate with Zhaorui Chen, Hang zhu

link for some extra information:
1.calculate the distance between Latitude/Longitude points
 http://www.movable-type.co.uk/scripts/latlong.html
2.extension for some math functions in sqlites
 https://www.sqlite.org/contrib

