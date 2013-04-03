galleriet
=========

..generates a simple single-file html image gallery of a folder.



BUILD
-----

Libraries  
`apt-get install libxslt1-dev libexif-dev libgd2-xpm-dev`


cd to  galleriet/src  
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



![Screenshot](https://github.com/pce/galleriet/raw/master/xslgalleriet.jpg)


Limitations
-----------

* only jpg's are supported


TODO
----

* gd: rotate images [gdImageCopyResampled](http://www.boutell.com/gd/manual2.0.28.html#gdImageCopyResampled)
* gd: generate thumbs for overview
* js: save rotate property in cookie
* addImageXml - use_relpath? name = basename(name) 
* option: define xmlfile for xslt
* option: supported fileformats - current pattern: *.jpg
* fix warning "DOCBparser deprecated" with -lxml2


Credits
-------

* [Libxslt](https://xmlsoft.org/xslt/) - the XSLT C library developed for the GNOME project
* [GD](http://www.boutell.com/gd/) - open source code library for the dynamic creation of images  
* [libexif](http://libexif.sourceforge.net/)
* progressbar: rosshemsley's blogpost



*Patches are welcome!*




 
