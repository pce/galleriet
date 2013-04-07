galleriet
=========

..generates a simple single-file html image gallery of a folder.

"galleriet" is swedish for "the gallery", 
not that the definite article (et) is a suffix.


BUILD
-----

Libraries 
`apt-get install libxslt1-dev libexif-dev libmagickwand-dev`

  
cd to  galleriet/src  
```
$ make
```

Make compiles executable binaries into the bin/Debug and bin/Release directory.
The xsl file has to be in the same path as the binary.
You can link to the gallery.xsl file of the source dir:

```
$ bin/Release/galleriet -m -o  "/home/username/Pictures/folder/index.html" "/home/username/Pictures/folder"
```


OPTIONS
-------

```
-x  skip generate xml and use ./gallery.xml for xslt
-m  generate HTML and resize Images,
-t  <gallerytitle> set title
-o  <outfile> HTML-File (default:stdout)
```


EXAMPLES
--------

generating HTML (xslt)
```
# generate html and medium sized images (keeps original files)
$ bin/Release/galleriet -m -o  "/home/username/Pictures/picfolder/index.html" "/home/username/Pictures/picfolder"

# view in browser
xdg-open  /home/username/Pictures/picfolder/index.html
```


![Screenshot](https://github.com/pce/galleriet/raw/master/xslgalleriet.jpg)


**Apache**  

Generate a password protected Directory  
```
$ htpasswd -b -c /var/www/files/passwd/passwords admin admin101
```
In /var/www/protected/.htaccess, or in httpd.conf inside a <Directory /var/www/protected> section.
```
AuthType Basic
AuthName "Restricted Files"
# (Following line optional)
AuthBasicProvider file
AuthUserFile /var/www/files/passwd/passwords
Require user admin 
```


Limitations
-----------

* only jpg's are supported


TODO
----

* make install: #ifdef RESOURCES_DIR and exists use it 
* option: define xmlfile for xslt
* generate xml in pwd?
* generate thumbs for overview (index)
* js: save rotate property in cookie? 
* js: url-hash history?
* addImageXml - use_relpath? name = basename(name) 
* option: supported fileformats - current pattern: *.jpg
* fix warning "DOCBparser deprecated" with -lxml2


Credits
-------

* [Libxslt](https://xmlsoft.org/xslt/) - the XSLT C library developed for the GNOME project
* [GD](http://www.boutell.com/gd/) - open source code library for the dynamic creation of images  
* [libexif](http://libexif.sourceforge.net/)
* progressbar: rosshemsley's blogpost



*Patches are welcome!*

