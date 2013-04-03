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
			background: url("chrome://global/skin/media/imagedoc-darknoise.png") repeat scroll 0 0 #444444;
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
        
.rotate0 { 
    -webkit-transform: rotate(0deg); 
    -moz-transform: rotate(0deg); 
    -o-transform: rotate(0deg);
    -ms-transform: rotate(0deg);
    rotation: 0deg;     
    filter: progid:DXImageTransform.Microsoft.BasicImage(rotation=0); 
}
        
.rotate90 { 
    -webkit-transform: rotate(90deg); 
    -moz-transform: rotate(90deg); 
    -o-transform: rotate(90deg);
    -ms-transform: rotate(90deg);
    rotation: 90deg;     
    filter: progid:DXImageTransform.Microsoft.BasicImage(rotation=1); 
}

.rotate180 {
    -webkit-transform: rotate(180deg);
    -moz-transform: rotate(180deg);
    -o-transform: rotate(180deg);
    -ms-transform: rotate(180deg);
    rotation: 180deg;     
    filter: progid:DXImageTransform.Microsoft.BasicImage(rotation=2);
}

.rotate270 {
    -webkit-transform: rotate(270deg);
    -moz-transform: rotate(270deg);
    -o-transform: rotate(270deg);
    -ms-transform: rotate(270deg);
    rotation: 270deg;     
    filter: progid:DXImageTransform.Microsoft.BasicImage(rotation=3);
}
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
				<a class="btn" id="btnrotate">o</a>
            </div>

        </div>
<script>
"use strict";

var images = [];
<xsl:apply-templates select="images/image"/>


// ---------------------------------------------------
// -- Class ImageGallery
// ---------------------------------------------------
var ImageGallery = function (picId) {
        this.picId = picId || "pic"
};
ImageGallery.prototype = {
    imgs: [],
    ptr: 0,
    degree: 0,
    preloadImgs: {},
    setImages: function (images) {
        this.imgs = images;
        return this;
    },
    onKeyDownEvent: function (event) {
        // trace(event);
        event = event || window.event;
        var key = event.keyCode;
        switch (key) {
        case 6:
        case 8:
        case 9:
            break;
        case 13:
            // enter
            break;
        case 16:
            // shift
            break;
        case 17:
            // ctrl
            break;
        case 18:
            // alt
            break;
        case 27:
            //  escape
            break;
        case 32:
            break;
        case 37:
            // left arrow
            this.pager(false);
            break;
        case 38:
            // (up arrow)
            this.rotate();
            break;
        case 40:
            // (down arrow)
            break;
        case 39:
            // right arrow
            this.pager(true);
            break;
        case 46:
            // del
            break;
        default:
            break;
        }
        // this.update();
    },
    init:function() {
        document.onkeydown=this.onKeyDownEvent.bind(this);
		// document.onkeypress=this.onKeyPressEvent.bind(this);
        this.margin = 140 + (40 * 2); // buttonwidth padding
        this.load();
    },
    pager: function (go) {
        if (go) {
            (this.ptr == this.imgs.length - 1) ? this.ptr = 0 : this.ptr++;
        } else {
            (this.ptr == 0) ? this.ptr = this.imgs.length - 1 : this.ptr--;
        }
        this.load();
    },
    isPlaying: false,
    ID: 0,
    animate: function (go) {
        if (!this.isPlaying &amp;&amp; go) {
            this.animId = setInterval(
            (function (self) {
                return function () {
                    self.pager(1);
                }
            })(this), 5000);
            this.isPlaying = true;
            id('btnplay').style.backgroundColor = "#fff";
            id('btnstop').style.backgroundColor = "rgb(252, 206, 101)";
        } else {
            clearInterval(this.animId);
            this.isPlaying = false;
            id('btnstop').style.backgroundColor = "#fff";
            id('btnplay').style.backgroundColor = "rgb(252, 206, 101)";
        }
    },
    load: function () {
        id(this.picId).className = "rotate0";              
        this.degree=0;
        id(this.picId).src = this.imgs[this.ptr]["filename"];
        // @todo http://www.quirksmode.org/dom/w3c_cssom.html
        // id('pic').width = window.innerWidth - this.margin;
        id('subtitle').innerHTML = this.imgs[this.ptr]["title"];
        // this.preLoad();
    },
    preLoad: function () {
        var prevPtr = this.ptr++;
        if (typeof this.preloadImgs[prevPtr] == "undefined" &amp;&amp; typeof this.imgs[prevPtr] != "undefined") {
            this.preloadImgs[prevPtr] = new Image();
            this.preloadImgs[prevPtr].src = this.imgs[this.ptr]["filename"];
        }

    },
    rotate: function () {
        var rotateclass;
        switch (this.degree) {
        case 90:
            rotateclass = "rotate180";
            this.degree=180;
            break;
        case 180:
            rotateclass = "rotate270";
            this.degree=270;
            break;
        case 270:
            rotateclass = "rotate0";
            this.degree=0;
            break;
        case 360:
        case 0:
            rotateclass = "rotate90";
            this.degree=90;
            break;
        default:
            rotateclass = "rotate0";            
            this.degree=0;
        }
        id(this.picId).className = rotateclass;
    }
};

// ----------------------------------

if (typeof id == "undefined") var id = function (e) {
        if (e) return document.getElementById(e);
};

window.onload = function () {
    var kioskmode = false;    
    // id('gallerynav').style.display = 'none';
    var ig = new ImageGallery("pic");
    ig.setImages(images);
    id('btnback').onclick = function () {
        ig.pager(false);
    };
    id('btnnext').onclick = function () {
        ig.pager(true);
    };
    id('btnrotate').onclick = function () {
        ig.rotate();
    };
    ig.init();
    // start animation
    if (kioskmode) {
        id('btnplay').style.display="";
        id('btnstop').style.display="";
        id('btnplay').onclick = function () {
            ig.animate(true);
        };
        id('btnstop').onclick = function () {
            ig.animate(false);
        };
        ig.animate(1);
    }
};
</script>
        </body>
    </html>
</xsl:template>

</xsl:stylesheet>
