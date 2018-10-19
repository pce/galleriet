/*
 * galleriet - a xsltgallery
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,  59 Temple Place - Suite 330, Cambridge, MA 02139, USA.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/DOCBparser.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <glob.h>
#include <libexif/exif-data.h>
#include <MagickWand/MagickWand.h>

#define JPEG_QUALITY 95
#define DEST_MAX_WIDTH 	800
#define DEST_MAX_HEIGHT 800


#ifdef DEBUG
#define TRACE(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define TRACE(...)
#endif

static void usage(const char *name)
{
    printf("Usage: %s [images-directory] [ -o index.html ]\n", name);
    printf("      -x (switch) skip generate xml and use ./gallery.xml for xslt\n");
    printf("      -t <gallerytitle> set title\n");
    printf("      -m (switch) generate HTML and resize Images\n");
    printf("      -o <filename> of HTML output\n");
    printf("      -h shows this usage message\n");
    printf("\nExamples:\n");
    printf("             %s /home/user/Pictures \n", name);
    printf("Set Title:   %s -t \"07/2011\" /home/user/Pictures/2011-07 -o /home/user/Pictures/2011-07/index.html\n", name);
    printf("Skip XML:    %s -x -o /home/user/Pictures/2011-07/index.html\n", name);
}


static char *basname(char *path)
{
    char *base = strrchr(path, '/');
    return base ? base+1 : path;
}

static int fileExists(char *filename)
{
    FILE *fh;
    if ((fh = fopen(filename, "r")) == NULL) {
        return 0;
    }
    fclose(fh);
    return 1;
}

static void progressBar(int x, int n, int r, int w)
{
    if ((r !=0) && (x % (n/r) != 0)) return;
    // Calculuate the ratio of complete-to-incomplete.
    float rat = x/(float)n;
    int   c     = rat * w;
    // Show the percentage complete.
    printf("%3d%% [", (int)(rat*100) );
    for (x=0; x<c; x++)
        printf("=");
    for (x=c; x<w; x++)
        printf(" ");
    printf("]\n\33[1A\33[2K");
}


static short getOrientation(ExifData *d, ExifIfd ifd)
{
    ExifEntry *entry = exif_content_get_entry(d->ifd[ifd], EXIF_TAG_ORIENTATION);
    if (entry) {
        return exif_get_short(entry->data, exif_data_get_byte_order(d));
    }
    return 1;
}

static void throwWandException(MagickWand *wand)
{
    char *description;

    ExceptionType severity;

    description=MagickGetException(wand,&severity);
    (void) fprintf(stderr,"%s %s %lu %s\n", GetMagickModule(), description);
    description=(char *) MagickRelinquishMemory(description);
}


static void createTransformedImage(const char* filename, const char* outfilename, int w, int h, short orientation)
{
    MagickWandGenesis();
    MagickBooleanType status;
    MagickWand *magick_wand = NULL;

    magick_wand = NewMagickWand();
    status = MagickReadImage(magick_wand, filename);
    if (status == MagickFalse) {
        TRACE("%s", "Exception MagickReadImage\n");
    }
    
    // resize with aspect ratio
    int width = MagickGetImageWidth(magick_wand);
    int height = MagickGetImageHeight(magick_wand);
    // TRACE("w:%d, h:%d\n", width, height);
    double mf1;
    double mf2;
    double magnificationFactor;

    mf1 = w/width;
    mf2 = h/height;

    magnificationFactor = fmin(mf1, mf2);
    w = (int)(w * magnificationFactor);
    h = (int)(h * magnificationFactor);


    /*
    available Filters:
    Bessel   
    Blackman   
    Box
    Catrom   
    CubicGaussian
    Hanning  
    Hermite    
    Lanczos (enlarge)
    Mitchell (shrink)
    PointQuandratic
    Sinc     
    Triangle
    */

    status = MagickResizeImage(magick_wand, w, h, MitchellFilter);
    if (status == MagickFalse) {
        TRACE("%s", "Exception MagickResizeImage\n");
    }
   
    // workarround: some JPG's need to be rotated ...
    double degrees = 0;
    if (orientation == 8) {
        degrees = 270.0;
    } else if (orientation == 6) {
        degrees =  90.0;
    }
    if (degrees) {
        TRACE("rotate %f°\n", degrees);
        status=MagickRotateImage(magick_wand, NewPixelWand(), degrees);
        if (status == MagickFalse) {
            TRACE("%s", "Exception MagickRotateImage\n");
            throwWandException(magick_wand);
        }
    }
    status=MagickWriteImages(magick_wand, outfilename, MagickTrue);
    if (status == MagickFalse) {
        TRACE("%s", "Exception MagickWriteImages\n");
        throwWandException(magick_wand);
    }

    magick_wand = DestroyMagickWand(magick_wand);
    MagickWandTerminus();
}


static void generateAndSaveImage(const char* filename, const char* suffix, int max_w, int max_h)
{
    char outfilename[FILENAME_MAX];
    short orientation = 0;
    snprintf(outfilename, sizeof(outfilename), "%s.%s", filename, suffix);

    // portrait or landscape?
    ExifData *ed = exif_data_new_from_file(filename);
    if (!ed) {
        TRACE("no EXIF data in file %s\n", filename);
    } else {
        orientation = getOrientation(ed, EXIF_IFD_0);
        TRACE("orientation %d\n", orientation);
    }
    createTransformedImage(filename, outfilename, max_w, max_h, orientation);
}

static void startXml(FILE* fp, char* name)
{
    fprintf (fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?><gallery><name>%s</name><images>\n", name);
}

static void closeXml(FILE* fp)
{
    fprintf (fp, "</images></gallery>\n");
    fclose(fp);
}

static void addImageXml(FILE* fp, char* name)
{
    if (1) name = basname(name);
    fprintf (fp, "<image><filename>%s</filename><title><![CDATA[ %s ]]></title></image>\n", name, name);
}

static void generateXML(char* filename, char* pattern, char* title, char* suffix)
{
    int i = 0;
    glob_t globbuf;
    size_t name_len;
    char name[FILENAME_MAX];
    FILE *fp = NULL;
    fp = fopen(filename, "w+");
    if(fp == NULL) {
        printf("Error opening %s for writing.", filename);
        abort();
    }
    startXml(fp, title);
    glob(pattern, GLOB_BRACE, NULL, &globbuf);
    for (i = 0; i < globbuf.gl_pathc; i++) {
        TRACE("progress %d/%zd\n",i, globbuf.gl_pathc);
        progressBar(i, globbuf.gl_pathc, 0, 50);
        name_len = sizeof name;
        snprintf(name, sizeof(name), "%s", globbuf.gl_pathv[i]);
        if (strlen(suffix) > 1) {
            name_len += 2;
            snprintf(name, name_len, "%s.%s", globbuf.gl_pathv[i], suffix);
            generateAndSaveImage(globbuf.gl_pathv[i], suffix, DEST_MAX_WIDTH, DEST_MAX_HEIGHT);
        }
        addImageXml(fp, name);
    }
    globfree(&globbuf);
    closeXml(fp);
}

int main(int argc, char **argv)
{
    int nbparams = 0;
    bool hasxml = false;
    bool hasout = false;
    bool hastitle = false;
    int result = 0;
    char pattern[FILENAME_MAX];
    char filename[FILENAME_MAX];
    char xsltfilename[FILENAME_MAX];
    char htmlfilename[FILENAME_MAX];
    char title[FILENAME_MAX];
    char suffix[FILENAME_MAX];
    char thumbnail[FILENAME_MAX];
    FILE *out = NULL;
    const char *params[16 + 1];
    params[nbparams] = NULL;
    xsltStylesheetPtr cur = NULL;
    xmlDocPtr doc, res;
    char *xsltfile = "./gallery.xsl";
    const char *xmlfile = "./gallery.xml";

    char* default_xslfile = "/usr/share/galleriet/gallery.xsl";


    while (argc > 1) {
        if (argv[1][0] == '-') {
            if (argv[1][1] == 'h' || (strcmp(argv[1],"--help")==0)) {
                usage(argv[0]);
                exit(0);
            } else if (argv[1][1] == 'p') {
                // sprintf(pattern,"%s",argv[2]);
            } else if (argv[1][1] == 'o') {
                hasout = true;
                snprintf(htmlfilename, sizeof(htmlfilename), "%s", argv[2]);
                TRACE("htmlfilename:%s\n", htmlfilename);
                argv++;
                argc--;
            }
            else if (argv[1][1] == 't') {
                hastitle = 1;
                // assume valid title
                snprintf(title, sizeof(title), "%s", argv[2]);
                argv++;
                argc--;
            } else if (argv[1][1] == 'n') {
                snprintf(thumbnail, sizeof(thumbnail), "%s", argv[2]);
                // assume valid filename
                generateAndSaveImage(thumbnail, "m.jpg", 320, 240);
                argv++;
                argc--;
            } else if (argv[1][1] == 'm') {
                snprintf(suffix, sizeof(suffix), "%s", "m.jpg");
            } else if (argv[1][1] == 'x') {
                TRACE("using ./gallery.xml");
                hasxml = true;
            }
            argc--;
            argv++;
        } else {
            break;
        }
    }

    if (argc > 1) {
        snprintf(pattern, sizeof(pattern), "%s", argv[1]);
    }
    else if (getcwd(pattern, sizeof(pattern)) != NULL) {
        ;
    }
    strcat(pattern, "/*.{CR2,jpeg,jpg,JPG}");
    TRACE("pattern: %s\n", pattern);
    if (argc > 2) {
        snprintf(filename, sizeof(filename), "%s", argv[2]);
    }
    else {
        snprintf(filename, sizeof(filename), "%s", xmlfile);
    }
    if (argc > 3) {
        snprintf(xsltfilename, sizeof(xsltfilename), "%s", argv[3]);
    }
    else {
        snprintf(xsltfilename, sizeof(xsltfilename), "%s", xsltfile);
    }

    if (!hastitle) snprintf(title, sizeof(title), "%s", "Photos Hydra");
    // TRACE("filename:%s\n", filename);
    if (hasout) {
        TRACE("out: %s\n", htmlfilename);
        if (fileExists(htmlfilename))
            printf("overwrite file %s\n", htmlfilename);
        out = fopen(htmlfilename, "w+");
        if(out == NULL) {
            printf("Error opening %s for writing.\n", filename);
            abort();
        }
    } else {
        out = stdout;
    }

    if (!hasxml) {
        TRACE("generateXML(%s)\n", filename);
        generateXML(filename, pattern, title, suffix);
    }

    if (!fileExists(xsltfilename)) {
        printf("`%s' file not found\n", xsltfilename);
        snprintf(xsltfilename, sizeof(xsltfilename), "%s", default_xslfile);
        printf("using `%s'\n", xsltfilename);
    }

    // printf("%s %s", xsltfile, xmlfile);
    xmlSubstituteEntitiesDefault(1);
    // xmlLoadExtDtdDefaultValue = 1;
    cur = xsltParseStylesheetFile((const xmlChar *)xsltfilename);
    doc = xmlParseFile(xmlfile);
    res = xsltApplyStylesheet(cur, doc, params);
    xsltSaveResultToFile(out, res, cur);
    if (hasout) {
        fclose(out);
        if (result == -1) {
            fprintf(stderr, "Error: Result not saved in HTML-file\n");
        }
    }
    xsltFreeStylesheet(cur);
    xmlFreeDoc(res);
    xmlFreeDoc(doc);
    xsltCleanupGlobals();
    xmlCleanupParser();
    return(0);
}

