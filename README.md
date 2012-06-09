galleriet
=========

..generates a simple single-file html image gallery of a folder.



BUILD
-----

cd to  galleriet/gallery
```
$ make
```

Make compiles executable binaries into the bin/Debug and bin/Release directory.
The xsl file has to be in the same path as the binary.
You can link to the gallery.xsl file of the source dir:

```
cd bin/Debug
ln -s ../../gallery.xsl .
# generate
./gallery -m -o "/home/username/Pictures/folder-name/index.html" "/home/pce/Pictures/folder-name"
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
./gallery -m -o "/home/username/Pictures/folder-name/index.html" "/home/pce/Pictures/folder-name"

# view in browser
xdg-open  /home/username/Pictures/picfolder/index.html
```


Code::blocks 
------------

gallery.cbp is a [Code::Blocks](http://codeblocks.org/) project file.

Project build options -> Linker settings:
```
`pkg-config gtk+-2.0 --libs`
`xslt-config --libs`
```


![Screenshot](https://github.com/pce/galleriet/raw/master/xslgallery.jpg)


Limitations
-----------

* no exif rotation
* only jpg's are supported


TODO
----

* gd: Portrait: extract image EXIF (like http://libexif.sourceforge.net/ ) infos and rotate images
* gd: generate thumbs for overview
* save rotate property
* addImageXml - use_relpath? name = basename(name) 
* option: define xmlfile for xslt
* option: supported fileformats - current pattern: *.jpg
* fix warning "DOCBparser deprecated" with -lxml2


Credits
-------

* [Libxslt](https://xmlsoft.org/xslt/) - the XSLT C library developed for the GNOME project
* [GD](http://www.boutell.com/gd/) - open source code library for the dynamic creation of images  
* progressbar: rosshemsley's blogpost



*Patches are welcome!*




 
