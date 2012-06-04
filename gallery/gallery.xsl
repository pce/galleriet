<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="image">
images.push({filename:"<xsl:value-of select="filename"/>",title:"<xsl:apply-templates select="title"/>"});
</xsl:template>

<xsl:template match="gallery">
    <html>
      <head>
        <title>
          <xsl:value-of select="name"/>
        </title>
        <style type="text/css" media="all">
        /**
	@section colors
		#cccc00  mustard
		#00cccc  mint
		#cc00cc  fuchsia
        */
        /** @section  basic */
        html, body {
            background: #444;
            color:#00cccc;
            font-family: Sans-Serif;
        }

        /** @section  gallery */

        .btn {
            width: 130px;
            cursor: pointer;
            padding: 10px;
            background: #cccc00;
            color: #cc00cc;
            text-decoration: none;
            font-weight: normal;
        }

        .btn:hover {
            cursor: pointer;
            background: #cc00cc;
            color: #cccc00;
            text-decoration: none;
        }

        .btn img { border:0px; vertical-align:middle; }

        #btnback { float:left;}
        #btnnext { float:left;clear:left; margin-top:20px}

        #gallery  { padding: 10px; font-size:11pt; font-family:Sans-Serif; font-weight: bold; }
        #subtitle { margin-left:160px; margin-top:10px;font-size: 11pt; font-family: Sans-Serif;}
        </style>
        </head>
        <body>
        <div id="gallery">

            <a class="btn" id="btnback">previous</a>
            <a class="btn" id="btnnext">next</a>
            <img id="pic" src="" />
            <div id="subtitle"></div>

            <div id="gallerynav" >
                <a class="btn" id="btnplay">play</a>
                <a class="btn" id="btnstop">stop</a>
            </div>

        </div>
        <script>
       	var images = [];
	<xsl:apply-templates select="images/image"/>
        // ---------------------------------------------------
        // -- Class ImageGallery
        // ---------------------------------------------------

        var ImageGallery = function() {};
        ImageGallery.prototype = {
            imgs : [],
            ptr : 0,
            preloadImgs : {},
            setImages : function(images) {
                this.imgs = images;
                return this;
            },
            pager : function(go) {
                  if (go) {
                    (this.ptr == this.imgs.length-1) ? this.ptr = 0 : this.ptr++;
                  } else {
                    (this.ptr == 0) ? this.ptr = this.imgs.length-1 : this.ptr--;
                  }
                  this.load();
                },
                isPlaying : false,
                ID : 0,
                animate : function(go) {
                        if(!this.isPlaying &amp;&amp; go) {
                          this.animId = setInterval(
                                    (function(self) {
                                        return function() {self.pager(1); } }
                                    )(this),
                                    5000
                                );
                          this.isPlaying = true;
                          $('btnplay').style.backgroundColor = "#fff";
                          $('btnstop').style.backgroundColor = "rgb(252, 206, 101)";
                        } else {
                          clearInterval(this.animId);
                          this.isPlaying = false;
                          $('btnstop').style.backgroundColor = "#fff";
                          $('btnplay').style.backgroundColor = "rgb(252, 206, 101)";
                        }
                },
                initialize : function() {
                    this.margin = 140 + (40 * 2); // buttonwidth padding
                    this.load();
                },
                load : function() {
                    $('pic').src = this.imgs[this.ptr]["filename"];
                    // @todo http://www.quirksmode.org/dom/w3c_cssom.html
                    // $('pic').width = window.innerWidth - this.margin;
                    $('subtitle').innerHTML = this.imgs[this.ptr]["title"];
                    // this.preLoad();
                },
                preLoad:function() {
                    var prevPtr = this.ptr++;
                    if (typeof this.preloadImgs[prevPtr] == "undefined"
                        &amp;&amp; typeof this.imgs[prevPtr] != "undefined") {
                        this.preloadImgs[prevPtr] = new Image();
                        this.preloadImgs[prevPtr].src = this.imgs[this.ptr]["filename"];
                    }

                }
            };

        if (typeof $ == "undefined")
            var $ = function(e) { if (e) return document.getElementById(e); };

        window.onload = function () {
            // $('gallerynav').style.display = 'none';
            var ig = new ImageGallery();
            ig.setImages(images);
            $('btnback').onclick = function() { ig.pager(false);};
            $('btnnext').onclick = function() { ig.pager(true);};
            $('btnplay').onclick = function() { ig.animate(true);};
            $('btnstop').onclick = function() { ig.animate(false);};
            var handleOnKeyUp = function (evt) {
                evt = (evt) ? evt : ((window.event) ? event : null);
                if (evt) {
                    switch (evt.keyCode) {
            		case 37:
                	// left
                	$('btnback').click();
                    break;
            		case 39:
                        // right
                        $('btnnext').click();
                        break;
                    }
                }
            };
            document.onkeyup = handleOnKeyUp;
            ig.initialize();
            ig.animate(1);
        }
        </script>
        </body>
    </html>
</xsl:template>

</xsl:stylesheet>
