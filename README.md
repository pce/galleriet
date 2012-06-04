galleriet
=========

..gernerates a simple single-file html image gallery of a folder


![Screenshot](:http://github.com/pce/galleriet/raw/master/xslgallery.jpg) 



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
./gallery /home/username/Pictures/picfolder -m -o /home/username/Pictures/picfolder/index.html
# view in browser
xdg-open  /home/username/Pictures/picfolder/index.html
```


Codeblocks 
----------

Project build options -> Linker settings:
```
`pkg-config gtk+-2.0 --libs`
`xslt-config --libs`
```

The xsl file has to be in the same path as the binary, 
or you may link to the gallery.xsl file

```
cd bin/Debug
ln -s ../../gallery.xsl .
# generate
./gallery -m -o "/home/username/Pictures/folder-name/index.html" "/home/pce/Pictures/folder-name"

```




TODO
----

* gd: Portrait: extract image EXIF (like http://libexif.sourceforge.net/ ) infos and rotate images
* css rotate button (and save property): 
  .rotate90 {
      rotation: 90deg;
      -webkit-transform: rotate(90deg);
      -moz-transform: rotate(90deg);
      filter: progid:DXImageTransform.Microsoft.BasicImage(rotation=1);
  }
* gd: generate thumbs for overview
* option: define xmlfile for xslt
* option: supported fileformats - current pattern: *.jpg
* fix warning "DOCBparser deprecated" with -lxml2
 
