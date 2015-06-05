/*
 * Copyright (c) 2001-2003, David Janssens
 * Copyright (c) 2002-2003, Yannick Verschueren
 * Copyright (c) 2002-2003,  Communications and remote sensing Laboratory, Universite catholique de Louvain, Belgium
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS `AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <openjpeg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifndef DONT_HAVE_GETOPT
#include <getopt.h>
#else
#include "compat/getopt.h"
#endif
#include "convert.h"


JPWL_cp_t jpwl_cp;
extern info_image info_IM;
int use_index;

void help_display()
{
  printf("HELP\n----\n\n");
  printf("- the option -help displays the readme.txt file on screen\n\n");


  printf("List of parameters for the coder JPEG 2000 :\n");
  printf("\n");
  printf
    ("- The markers COD and QCD are writed both of two in the main_header and never appear in the tile_header.  The markers in the main header are : SOC SIZ COD QCD COM.\n");
  printf("\n");
  printf
    ("- This coder can encode mega image, a test was made on a 24000x24000 pixels color image.  You need enough disk space memory (twice the original) to encode the image. (i.e. for a 1.5 Gb image you need a minimum of 3Gb of disk memory)\n");
  printf("\n");
  printf("REMARKS :\n");
  printf("---------\n");
  printf("\n");
  printf
    ("* the value of rate enter in the code line is the compression factor !\n");
  printf("exemple :\n");
  printf("\n");
  printf
    ("-r 20,10,1 means quality 1 : compress 20x, quality 2 : compress 10x and quality 3 : compress 1x = lossless\n");
  printf("\n");
  printf("By default :\n");
  printf("------------\n");
  printf("\n");
  printf(" * lossless\n");
  printf(" * 1 tile\n");
  printf(" * size of precinct 2^15 x 2^15 (means 1 precinct)\n");
  printf(" * size of code-block 64 x 64\n");
  printf(" * Number of resolution : 6\n");
  printf(" * No SOP marker in the codestream\n");
  printf(" * No EPH marker in the codestream\n");
  printf(" * No sub-sampling in x and y direction\n");
  printf(" * No mode switch activated\n");
  printf(" * progression order : LRCP\n");
  printf(" * No index file\n");
  printf(" * No ROI upshifted\n");
  printf(" * No offset of the origin of the image\n");
  printf(" * No offset of the origin of the tiles\n");
  printf(" * Reversible DWT 5-3\n");
  printf("\n");
  printf("Parameters :\n");
  printf("------------\n");
  printf("\n");
  printf
    ("-i             : source file  (-i source.pnm also *.pgm, *.ppm) required\n");
  printf("\n");
  printf("-o             : destination file (-o dest.j2k or .jp2) required\n");
  printf("\n");
  printf("-help          : Display the help information optional\n ");
  printf("\n");
  printf("-r             : different rates (-r 20,10,5) optional\n ");
  printf("\n");
  printf("-n             : Number of resolution (-n 3) optional\n");
  printf("\n");
  printf("-b             : size of code block (-b 32,32) optional\n");
  printf("\n");
  printf("-c             : size of precinct (-c 128,128) optional\n");
  printf("\n");
  printf("-t             : size of tile (-t 512,512) optional\n");
  printf("\n");
  printf
    ("-p             : progression order (-p LRCP) [LRCP, RLCP, RPCL, PCRL, CPRL] optional\n");
  printf("\n");
  printf
    ("-s             : subsampling factor (-s 2,2) [-s X,Y] optional\n");
  printf("\n");
  printf
    ("-SOP           : write SOP marker before each packet optional\n");
  printf("\n");
  printf
    ("-EPH           : write EPH marker after each header packet optional\n");
  printf("\n");
  printf
    ("-M             : mode switch (-M 3) [1=BYPASS(LAZY) 2=RESET 4=RESTART(TERMALL) 8=VSC 16=ERTERM(SEGTERM) 32=SEGMARK(SEGSYM)] optional\n");
  printf
    ("                    for several mode switch you have to add the value of each mode you want\n");
  printf
    ("                    ex : RESTART(4) + RESET(2) + SEGMARK(32) = -M 38\n");
  printf("\n");
  printf
    ("-x             : Create an index file *.Idx (-x index_name.Idx) optional\n");
  printf("\n");
  printf
    ("-ROI:c=%%d,U=%%d : quantization indices upshifted for component c=%%d [%%d = 0,1,2]\n");
  printf
    ("                 with a value of U=%%d [0 <= %%d <= 37] (i.e. -ROI:c=0,U=25) optional\n");
  printf("\n");
  printf
    ("-d             : offset of the origin of the image (-d 150,300) optional\n");
  printf("\n");
  printf
    ("-T             : offset of the origin of the tiles (-T 100,75) optional\n");
  printf("\n");
  printf("-I             : Use the irreversible DWT 9-7 (-I) optional\n");
  printf("\n");
  printf("IMPORTANT :\n");
  printf("-----------\n");
  printf("\n");
  printf("* subsampling bigger than 2 can produce error\n");
  printf("\n");
  printf("The index file respect the structure below :\n");
  printf("---------------------------------------------\n");
  printf("\n");
  printf("Image_height Image_width\n");
  printf("progression order\n");
  printf("Tiles_size_X Tiles_size_Y\n");
  printf("Components_nb\n");
  printf("Layers_nb\n");
  printf("decomposition_levels\n");
  printf("Precincts_size_X Precincts_size_Y\n");
  printf("Main_header_end_position\n");
  printf("Codestream_size\n");
  printf("Tile0 start_pos end_Theader end_pos\n");
  printf("Tile1  ''           ''        ''\n");
  printf("...\n");
  printf("TileN  ''           ''        ''\n");
  printf("Tpacket_0 Tile layer res. comp. prec. start_pos end_pos\n");
  printf("...\n");
  printf("Tpacket_M  ''    ''   ''   ''    ''       ''       ''\n");
}

int give_progression(char progression[4])
{
  if (progression[0] == 'L' && progression[1] == 'R'
      && progression[2] == 'C' && progression[3] == 'P') {
    return 0;
  } else {
    if (progression[0] == 'R' && progression[1] == 'L'
	&& progression[2] == 'C' && progression[3] == 'P') {
      return 1;
    } else {
      if (progression[0] == 'R' && progression[1] == 'P'
	  && progression[2] == 'C' && progression[3] == 'L') {
	return 2;
      } else {
	if (progression[0] == 'P' && progression[1] == 'C'
	    && progression[2] == 'R' && progression[3] == 'L') {
	  return 3;
	} else {
	  if (progression[0] == 'C' && progression[1] == 'P'
	      && progression[2] == 'R' && progression[3] == 'L') {
	    return 4;
	  } else {
	    return -1;
	  }
	}
      }
    }
  }
}

double dwt_norms_97[4][10] = {
  {1.000, 1.965, 4.177, 8.403, 16.90, 33.84, 67.69, 135.3, 270.6, 540.9},
  {2.022, 3.989, 8.355, 17.04, 34.27, 68.63, 137.3, 274.6, 549.0},
  {2.022, 3.989, 8.355, 17.04, 34.27, 68.63, 137.3, 274.6, 549.0},
  {2.080, 3.865, 8.307, 17.18, 34.71, 69.59, 139.3, 278.6, 557.2}
};

int floorlog2(int a)
{
  int l;
  for (l = 0; a > 1; l++) {
    a >>= 1;
  }
  return l;
}

void encode_stepsize(int stepsize, int numbps, int *expn, int *mant)
{
  int p, n;
  p = floorlog2(stepsize) - 13;
  n = 11 - floorlog2(stepsize);
  *mant = (n < 0 ? stepsize >> -n : stepsize << n) & 0x7ff;
  *expn = numbps - p;
}

void calc_explicit_stepsizes(j2k_tccp_t * tccp, int prec)
{
  int numbands, bandno;
  numbands = 3 * tccp->numresolutions - 2;
  for (bandno = 0; bandno < numbands; bandno++) {
    double stepsize;

    int resno, level, orient, gain;
    resno = bandno == 0 ? 0 : (bandno - 1) / 3 + 1;
    orient = bandno == 0 ? 0 : (bandno - 1) % 3 + 1;
    level = tccp->numresolutions - 1 - resno;
    gain =
      tccp->qmfbid == 0 ? 0 : (orient ==
			       0 ? 0 : (orient == 1
					|| orient == 2 ? 1 : 2));
    if (tccp->qntsty == J2K_CCP_QNTSTY_NOQNT) {
      stepsize = 1.0;
    } else {
      double norm = dwt_norms_97[orient][level];
      stepsize = (1 << (gain + 1)) / norm;
    }
    encode_stepsize((int) floor(stepsize * 8192.0), prec + gain,
		    &tccp->stepsizes[bandno].expn,
		    &tccp->stepsizes[bandno].mant);
  }
}

int main(int argc, char **argv)
{
  int len;
  int NumResolution, numD_min;	/*   NumResolution : number of resolution                     */
  int Tile_arg;			/*   Tile_arg = 0 (not in argument) ou = 1 (in argument)      */
  int CSty;			/*   CSty : coding style                                      */
  int Prog_order;		/*   progression order (default LRCP)                         */
  char progression[4];
  int numpocs, numpocs_tile;	/*   Number of progression order change (POC) default 0       */
  int prcw_init[J2K_MAXRLVLS];	/*   Initialisation Precinct width                            */
  int prch_init[J2K_MAXRLVLS];	/*   Initialisation Precinct height                           */
  //int prcw_init, prch_init;                     /*   Initialisation precincts' size                           */
  int cblockw_init, cblockh_init;	/*   Initialisation codeblocks' size                          */
  int mode, value;		/*   Mode switch (cblk_style)                                 */
  int subsampling_dx, subsampling_dy;	/* subsampling value for dx and dy                    */
  int ROI_compno, ROI_shift;	/*   region of interrest                                      */
  int Dim[2];			/*   portion of the image coded                               */
  int TX0, TY0;			/*   tile off-set                                             */
  j2k_image_t img;
  j2k_cp_t cp, cp_init;		/*   cp_init is used to initialise in multiple tiles          */
  j2k_tcp_t *tcp, *tcp_init;	/*   tcp_init is used to initialise in multiple tile          */
  j2k_poc_t POC[32];		/*   POC : used in case of Progression order change           */
  j2k_poc_t *tcp_poc;
  j2k_tccp_t *tccp;
  
  int i, tileno, j;
  char *infile = 0;
  char *outfile = 0;
  char *index = 0;
  char *s, S1, S2, S3;
  int ir = 0;
  int res_spec = 0;		/*   For various precinct sizes specification                 */
  char sep;
  char *outbuf, *out;
  FILE *f;


  /* default value */
  /* ------------- */
  NumResolution = 6;
  CSty = 0;
  cblockw_init = 64;
  cblockh_init = 64;
  cp.tw = 1;
  cp.th = 1;
  cp.index_on = 0;
  Prog_order = 0;
  numpocs = 0;
  mode = 0;
  subsampling_dx = 1;
  subsampling_dy = 1;
  ROI_compno = -1;		/* no ROI */
  ROI_shift = 0;
  Dim[0] = 0;
  Dim[1] = 0;
  TX0 = 0;
  TY0 = 0;
  cp.comment = "Created by OpenJPEG version 0.9";
  cp.disto_alloc = 0;
  cp.fixed_alloc = 0;
  cp.fixed_quality = 0;		//add fixed_quality
  /* img.PPT=0; */

  Tile_arg = 0;
  cp_init.tcps = (j2k_tcp_t *) malloc(sizeof(j2k_tcp_t));	/* initialisation if only one tile */
  tcp_init = &cp_init.tcps[0];
  tcp_init->numlayers = 0;
  jpwl_cp_init(&jpwl_cp);

  cp.intermed_file=0;
  use_index=0;

  while (1) {
    int c = getopt(argc, argv,
		"i:o:r:q:f:t:n:c:b:x:p:s:d:h:P:S:E:M:R:T:C:I:W,F");
    if (c == -1)
      break;
    switch (c) {
    case 'i':			/* IN fill */
      infile = optarg;
      s = optarg;
      while (*s) {
	s++;
      }
      s--;
      S3 = *s;
      s--;
      S2 = *s;
      s--;
      S1 = *s;

      if ((S1 == 'p' && S2 == 'g' && S3 == 'x')
	  || (S1 == 'P' && S2 == 'G' && S3 == 'X')) {
	cp.image_type = 0;
	break;
      }

      if ((S1 == 'p' && S2 == 'n' && S3 == 'm')
	  || (S1 == 'P' && S2 == 'N' && S3 == 'M')
	  || (S1 == 'p' && S2 == 'g' && S3 == 'm') || (S1 == 'P'
						       && S2 == 'G'
						       && S3 == 'M')
	  || (S1 == 'P' && S2 == 'P' && S3 == 'M') || (S1 == 'p'
						       && S2 == 'p'
						       && S3 == 'm')) {
	cp.image_type = 1;
	break;
      }

      if ((S1 == 'b' && S2 == 'm' && S3 == 'p')
	  || (S1 == 'B' && S2 == 'M' && S3 == 'P')) {
	cp.image_type = 2;
	break;
      }
      fprintf(stderr,
	      "!! Unrecognized format for infile : %c%c%c [accept only *.pnm, *.pgm, *.ppm, *.pgx or *.bmp] !!\n\n",
	      S1, S2, S3);
      return 1;
      break;
      /* ----------------------------------------------------- */
    case 'o':			/* OUT fill */
      outfile = optarg;
      while (*outfile) {
	outfile++;
      }
      outfile--;
      S3 = *outfile;
      outfile--;
      S2 = *outfile;
      outfile--;
      S1 = *outfile;
      
      outfile = optarg;
      
      if ((S1 == 'j' && S2 == '2' && S3 == 'k') || (S1 == 'J' && S2 == '2' && S3 == 'K'))
	cp.JPEG2000_format=0;
      else if ((S1 == 'j' && S2 == 'p' && S3 == '2') || (S1 == 'J' && S2 == 'P' && S3 == '2'))
	cp.JPEG2000_format=1;
      else    {
	fprintf(stderr,"Unknown output format image *.%c%c%c [only *.j2k, *.jp2]!! \n",S1,S2,S3);
	return 1;
      }
      
      
      
      break;
      /* ----------------------------------------------------- */
    case 'r':			/* rates rates/distorsion */
      s = optarg;
      while (sscanf(s, "%d", &tcp_init->rates[tcp_init->numlayers])
	     == 1) {
	tcp_init->numlayers++;
	while (*s && *s != ',') {
	  s++;
	}
	if (!*s)
	  break;
	s++;
      }
      cp.disto_alloc = 1;
      cp.matrice = NULL;
      break;
      /* ----------------------------------------------------- */
    case 'q':			/* add fixed_quality */
      s = optarg;
      while (sscanf(s, "%f", &tcp_init->distoratio[tcp_init->numlayers]) ==
	     1) {
	tcp_init->numlayers++;
	while (*s && *s != ',') {
	  s++;
	}
	if (!*s)
	  break;
	s++;
      }
      cp.fixed_quality = 1;
      cp.matrice = NULL;
      break;
      /* dda */
      /* ----------------------------------------------------- */
    case 'f':			/* mod fixed_quality (before : -q) */
      s = optarg;
      sscanf(s, "%d", &tcp_init->numlayers);
      s++;
      if (tcp_init->numlayers > 9)
	s++;
      cp.matrice =
	(int *) malloc(tcp_init->numlayers * NumResolution * 3 *
		       sizeof(int));
      s = s + 2;
      for (i = 0; i < tcp_init->numlayers; i++) {
	tcp_init->rates[i] = 1;
	sscanf(s, "%d,", &cp.matrice[i * NumResolution * 3]);
	s += 2;
	if (cp.matrice[i * NumResolution * 3] > 9)
	  s++;
	cp.matrice[i * NumResolution * 3 + 1] = 0;
	cp.matrice[i * NumResolution * 3 + 2] = 0;
	for (j = 1; j < NumResolution; j++) {
	  sscanf(s, "%d,%d,%d",
		 &cp.matrice[i * NumResolution * 3 + j * 3 + 0],
		 &cp.matrice[i * NumResolution * 3 + j * 3 + 1],
		 &cp.matrice[i * NumResolution * 3 + j * 3 + 2]);
	  s += 6;
	  if (cp.matrice[i * NumResolution * 3 + j * 3] > 9)
	    s++;
	  if (cp.matrice[i * NumResolution * 3 + j * 3 + 1] > 9)
	    s++;
	  if (cp.matrice[i * NumResolution * 3 + j * 3 + 2] > 9)
	    s++;
	}
	if (i < tcp_init->numlayers - 1)
	  s++;
      }
      cp.fixed_alloc = 1;
      break;
      /* ----------------------------------------------------- */
    case 't':			/* tiles */
      sscanf(optarg, "%d,%d", &cp.tdx, &cp.tdy);
      Tile_arg = 1;
      break;
      /* ----------------------------------------------------- */
    case 'n':			/* resolution */
      sscanf(optarg, "%d", &NumResolution);
      break;
      /* ----------------------------------------------------- */
    case 'c':			/* precinct dimension */
      s = optarg;
      do {
	sep = 0;
	sscanf(s, "[%d,%d]%c", &prcw_init[res_spec],
	       &prch_init[res_spec], &sep);
	CSty |= 0x01;
	res_spec++;
	s = strpbrk(s, "]") + 2;
      } while (sep == ',');
      break;
      /* ----------------------------------------------------- */
    case 'b':			/* code-block dimension */
      sscanf(optarg, "%d,%d", &cblockw_init, &cblockh_init);
      if (cblockw_init * cblockh_init > 4096 || cblockw_init > 1024
	  || cblockw_init < 4 || cblockh_init > 1024 || cblockh_init < 4) {
	fprintf(stderr,
		"!! Size of code_block error (option -b) !!\n\nRestriction :\n    * width*height<=4096\n    * 4<=width,height<= 1024\n\n");
	return 1;
      }
      break;
      /* ----------------------------------------------------- */
    case 'x':			/* creation of index file */
      index = optarg;
      cp.index_on = 1;
	  use_index = 1;
      break;
      /* ----------------------------------------------------- */
    case 'p':			/* progression order */
      s = optarg;
      for (i = 0; i < 4; i++) {
	progression[i] = *s;
	s++;
      }
      Prog_order = give_progression(progression);

      if (Prog_order == -1) {
	fprintf(stderr,
		"Unrecognized progression order [LRCP, RLCP, RPCL, PCRL, CPRL] !!\n");
	return 1;
      }
      break;
      /* ----------------------------------------------------- */
    case 's':			/* subsampling factor */
      if (sscanf(optarg, "%d,%d", &subsampling_dx, &subsampling_dy)
	  != 2) {
	fprintf(stderr,
		"'-s' sub-sampling argument error !  [-s dx,dy]\n");
	return 1;
      }
      break;
      /* ----------------------------------------------------- */
    case 'd':			/* coordonnate of the reference grid */
      if (sscanf(optarg, "%d,%d", &Dim[0], &Dim[1]) != 2) {
	fprintf(stderr,
		"-d 'coordonnate of the reference grid' argument error !! [-d x0,y0]\n");
	return 1;
      }
      break;
      /* ----------------------------------------------------- */
    case 'h':			/* Display an help description */
      help_display();
      return 0;
      break;
      /* ----------------------------------------------------- */
    case 'P':			/* POC */
      fprintf(stderr, "/----------------------------------\\\n");
      fprintf(stderr, "|  POC option not fully tested !!  |\n");
      fprintf(stderr, "\\----------------------------------/\n");

      s = optarg;
      while (sscanf(s, "T%d=%d,%d,%d,%d,%d,%s", &POC[numpocs].tile,
		    &POC[numpocs].resno0, &POC[numpocs].compno0,
		    &POC[numpocs].layno1, &POC[numpocs].resno1,
		    &POC[numpocs].compno1, POC[numpocs].progorder) == 7) {
	POC[numpocs].prg = give_progression(POC[numpocs].progorder);
	/* POC[numpocs].tile; */
	numpocs++;
	while (*s && *s != '/') {
	  s++;
	}
	if (!*s)
	  break;
	s++;
      }
      break;
      /* ------------------------------------------------------ */
    case 'S':			/* SOP marker */
      CSty |= 0x02;
      break;
      /* ------------------------------------------------------ */
    case 'E':			/* EPH marker */
      CSty |= 0x04;
      break;
      /* ------------------------------------------------------ */
    case 'M':			/* Mode switch pas tous au point !! */
      if (sscanf(optarg, "%d", &value) == 1) {
	for (i = 0; i <= 5; i++) {
	  int cache = value & (1 << i);
	  if (cache)
	    mode |= (1 << i);
	}
      }
      break;
      /* ------------------------------------------------------ */
    case 'R':			/* ROI */
      if (sscanf(optarg, "OI:c=%d,U=%d", &ROI_compno, &ROI_shift) != 2) {
	fprintf(stderr, "ROI error !! [-ROI:c='compno',U='shift']\n");
	return 1;
      }
      break;
      /* ------------------------------------------------------ */
    case 'T':			/* Tile offset */
      if (sscanf(optarg, "%d,%d", &TX0, &TY0) != 2) {
	fprintf(stderr, "-T 'tile offset' argument error !! [-T X0,Y0]");
	return 1;
      }
      break;
      /* ------------------------------------------------------ */
    case 'C':			/* Add a comment */
      cp.comment = optarg;
      break;
      /* ------------------------------------------------------ */
    case 'I':			/* reversible or not */
      ir = 1;
      break;
	   /* ------------------------------------------------------ */
	case 'W':			/* version 0.2 enables only EPC on main header and epb in fixed way*/
	  jpwl_cp.JPWL_on = 1;
	  break;
	   /* ------------------------------------------------------ */
	case 'F':			/* use intermed files*/
	  cp.intermed_file=1;
	  break;
	   /* ------------------------------------------------------ */
    default:
      return 1;
    }
  }

  cp.tx0 = TX0;
  cp.ty0 = TY0;

  // inserici i parametri jpwl
  
  if(jpwl_cp.JPWL_on)
	get_jpwl_cp(& cp);


  


  /* Error messages */
  /* -------------- */
  if (!infile || !outfile) {
    fprintf(stderr,
	    "usage: image_to_j2k -i image-file -o j2k/jp2-file (+ options)\n");
    return 1;
  }

  if ((cp.disto_alloc || cp.fixed_alloc || cp.fixed_quality)&&(!(cp.disto_alloc ^ cp.fixed_alloc ^ cp.fixed_quality))) {
    fprintf(stderr,
	    "Error: options -r -q and -f can not be used together !!\n");
    return 1;
  } // mod fixed_quality

  /* if no rate entered, lossless by default */
  if (tcp_init->numlayers == 0) {
    tcp_init->rates[tcp_init->numlayers] = 1;
    tcp_init->numlayers++;
    cp.disto_alloc = 1;
  }

  if (TX0 > Dim[0] || TY0 > Dim[1]) {
    fprintf(stderr,
	    "Error: Tile offset dimension is unnappropriate --> TX0(%d)<=IMG_X0(%d) TYO(%d)<=IMG_Y0(%d) \n",
	    TX0, Dim[0], TY0, Dim[1]);
    return 1;
  }

  for (i = 0; i < numpocs; i++) {
    if (POC[i].prg == -1) {
      fprintf(stderr,
	      "Unrecognized progression order in option -P (POC n %d) [LRCP, RLCP, RPCL, PCRL, CPRL] !!\n",
	      i + 1);
    }
  }

  switch (cp.image_type) {
  case 0:
    if (Tile_arg) {
      if (!pgxtoimage
	  (infile, &img, cp.tdy, subsampling_dx, subsampling_dy, Dim,
	   cp)) {
	fprintf(stderr, "not a pgx file\n");
	return 1;
      }
    } else {
      if (!pgxtoimage
	  (infile, &img, -1, subsampling_dx, subsampling_dy, Dim, cp)) {
	fprintf(stderr, " not a pgx file\n");
	return 1;
      }
    }
    break;

  case 1:
    if (!pnmtoimage(infile, &img, subsampling_dx, subsampling_dy, Dim)) {
      fprintf(stderr, " not a pnm file\n");
      return 1;
    }
    break;

  case 2:
    if (!bmptoimage(infile, &img, subsampling_dx, subsampling_dy, Dim)) {
      fprintf(stderr, " not a bmp file\n");
      return 1;
    }
    break;
  }
  /* to respect profile - 0 */
  /* ---------------------- */
  numD_min = 0;
  /*   while (int_ceildiv(img.x1,(1<<numD_min))-int_ceildiv(img.x0,(1<<numD_min))>120 || int_ceildiv(img.y1,(1<<numD_min))-int_ceildiv(img.y0,(1<<numD_min))>160) numD_min++;
     if ((numD_min+1)>NumResolution)
     {
     fprintf(stderr,"\n********************************************************************************\n\n");
     fprintf(stderr,  "In view to respect Profile-0, the number of resolution used is %d in place of %d\n\n",numD_min+1,NumResolution);
     fprintf(stderr,  "********************************************************************************\n\n");
     NumResolution=numD_min+1;
     } */

  if (Tile_arg == 1) {
    cp.tw = int_ceildiv(img.x1 - cp.tx0, cp.tdx);
    cp.th = int_ceildiv(img.y1 - cp.ty0, cp.tdy);
  } else {
    cp.tdx = img.x1 - cp.tx0;
    cp.tdy = img.y1 - cp.ty0;
  }
 
  /* Initialization for PPM marker */
  cp.ppm = 0;
  cp.ppm_data = NULL;
  cp.ppm_previous = 0;
  cp.ppm_store = 0;

  /* Init the mutiple tiles */
  /* ---------------------- */
  cp.tcps = (j2k_tcp_t *) malloc(cp.tw * cp.th * sizeof(j2k_tcp_t));

  for (tileno = 0; tileno < cp.tw * cp.th; tileno++) {
    tcp = &cp.tcps[tileno];
    tcp->numlayers = tcp_init->numlayers;
    for (j = 0; j < tcp->numlayers; j++) {
      if (cp.fixed_quality)   // add fixed_quality
	tcp->distoratio[j] = tcp_init->distoratio[j];
      else
	tcp->rates[j] = tcp_init->rates[j];
    }
    tcp->csty = CSty;
    tcp->prg = Prog_order;
    tcp->mct = img.numcomps == 3 ? 1 : 0;
    tcp->ppt = 0;
    tcp->ppt_data = NULL;
    tcp->ppt_store = 0;

    numpocs_tile = 0;
    tcp->POC = 0;
    if (numpocs) {
      /* intialisation of POC */
      tcp->POC = 1;
      for (i = 0; i < numpocs; i++) {
	if (tileno == POC[i].tile - 1 || POC[i].tile == -1) {
	  tcp_poc = &tcp->pocs[numpocs_tile];
	  tcp_poc->resno0 = POC[numpocs_tile].resno0;
	  tcp_poc->compno0 = POC[numpocs_tile].compno0;
	  tcp_poc->layno1 = POC[numpocs_tile].layno1;
	  tcp_poc->resno1 = POC[numpocs_tile].resno1;
	  tcp_poc->compno1 = POC[numpocs_tile].compno1;
	  tcp_poc->prg = POC[numpocs_tile].prg;
	  tcp_poc->tile = POC[numpocs_tile].tile;
	  numpocs_tile++;
	}
      }
    }
    tcp->numpocs = numpocs_tile;
    tcp->tccps = (j2k_tccp_t *) malloc(img.numcomps * sizeof(j2k_tccp_t));

    for (i = 0; i < img.numcomps; i++) {
      tccp = &tcp->tccps[i];
      tccp->csty = CSty & 0x01;	/* 0 => one precinct || 1 => custom precinct  */
      tccp->numresolutions = NumResolution;
      tccp->cblkw = int_floorlog2(cblockw_init);
      tccp->cblkh = int_floorlog2(cblockh_init);
      tccp->cblksty = mode;
      tccp->qmfbid = ir ? 0 : 1;
      tccp->qntsty = ir ? J2K_CCP_QNTSTY_SEQNT : J2K_CCP_QNTSTY_NOQNT;
      tccp->numgbits = 2;
      if (i == ROI_compno)
	tccp->roishift = ROI_shift;
      else
	tccp->roishift = 0;
      if (CSty & J2K_CCP_CSTY_PRT) {
	int p = 0;
	for (j = tccp->numresolutions - 1; j >= 0; j--) {
	  if (p < res_spec) {
	    if (prcw_init[p] < 1)
	      tccp->prcw[j] = 1;
	    else
	      tccp->prcw[j] = int_floorlog2(prcw_init[p]);

	    if (prch_init[p] < 1)
	      tccp->prch[j] = 1;
	    else
	      tccp->prch[j] = int_floorlog2(prch_init[p]);
	  } else {
	    int size_prcw, size_prch;
	    size_prcw = prcw_init[res_spec - 1] >> (p - (res_spec - 1));
	    size_prch = prch_init[res_spec - 1] >> (p - (res_spec - 1));
	    if (size_prcw < 1)
	      tccp->prcw[j] = 1;
	    else
	      tccp->prcw[j] = int_floorlog2(size_prcw);
	    if (size_prch < 1)
	      tccp->prch[j] = 1;
	    else
	      tccp->prch[j] = int_floorlog2(size_prch);
	  }
	  p++;
	  /*printf("\nsize precinct pour level %d : %d,%d\n", j,
		 tccp->prcw[j], tccp->prch[j]);*/
	}
      } else {
	for (j = 0; j < tccp->numresolutions; j++) {
	  tccp->prcw[j] = 15;
	  tccp->prch[j] = 15;
	}
      }
      calc_explicit_stepsizes(tccp, img.comps[i].prec);
    }
  }

  if (cp.JPEG2000_format==0) {	      /* J2K format output */
    if (cp.intermed_file==1) {	      /* After the encoding of each tile, j2k_encode 
					 stores the data in the file*/
      len = j2k_encode(&img, &cp, outfile, cp.tdx * cp.tdy * 10, index);
      if (len == 0) {
	fprintf(stderr, "failed to encode image\n");
	return 1;
      }
    }
	else {
      outbuf = (char *) malloc( cp.tdx * cp.tdy * cp.tw * cp.th * 4*sizeof(char)); /* Allocate memory for all tiles */
      cio_init(outbuf, cp.tdx * cp.tdy * cp.tw * cp.th * 4);							 
      len = j2k_encode(&img, &cp, outbuf, cp.tdx * cp.tdy * cp.tw * cp.th * 4, index); 
      if (len == 0) {
			fprintf(stderr, "failed to encode image\n");
			return 1;
      }
	// non uso l'intermed file
	  if (jpwl_cp.JPWL_on){
		out= (char *) malloc(len*4*sizeof(char));
		len=jpwl_encode(outbuf,out,len);
		if (len == 0) {
		  fprintf(stderr, "failed to encode image\n");
		  return 1;
		}
		
		if (cp.index_on && use_index){
			//printf("num: %d\n",info_IM.num);
			write_index(index, len);
		}
		
		f = fopen(outfile, "wb");	
		if (!f) {					
			fprintf(stderr, "failed to open %s for writing\n", outfile);	
			return 1;				
		}   
		fwrite(out, 1, len, f);
		free(outbuf);
		free(out);
		fclose(f);
		if(cp.index_on)
			j2k_free_info();
	

		}
	  else {
		  f = fopen(outfile, "wb");	
		  if (!f) {					
			fprintf(stderr, "failed to open %s for writing\n", outfile);	
			return 1;				
		  }   
		  fwrite(outbuf, 1, len, f);
		  free(outbuf);
		  fclose(f);
		  if(cp.index_on)
			j2k_free_info();
		
	  
	  
	  }
	}
  }
  else			       /* JP2 format output */
  {
    jp2_struct_t * jp2_struct;
    jp2_struct = (jp2_struct_t *) malloc(sizeof(jp2_struct_t));
    jp2_struct->image = &img;
        
    /* Initialising the standard JP2 box content*/
    /* If you wish to modify those boxes, you have to modify the jp2_struct content*/ 
    if (jp2_init_stdjp2(jp2_struct, &img))
    {
      fprintf(stderr,"Error with jp2 initialization");
      return 1;
    };

    if (cp.intermed_file==1) {	      
      /*For the moment, JP2 format does not use intermediary files for each tile*/
      cp.intermed_file=0;
    }
    outbuf = (char *) malloc( cp.tdx * cp.tdy * cp.tw * cp.th * 10 *sizeof(char));
    cio_init(outbuf, cp.tdx * cp.tdy * cp.tw * cp.th * 10);    
    len = jp2_encode(jp2_struct, &cp, outbuf, index);
    if (len == 0) {
      fprintf(stderr, "failed to encode image\n");
      return 1;
    }
    f = fopen(outfile, "wb");	
    if (!f) {					
      fprintf(stderr, "failed to open %s for writing\n", outfile);	
      return 1;				
    }
    fwrite(outbuf, 1, len, f);
    free(outbuf);
    fclose(f);
  }

  /* Remove the temporary files */
  /* -------------------------- */
  if (cp.image_type) {		/* PNM PGM PPM */
    for (i = 0; i < img.numcomps; i++) {
      char tmp[7];										// risolto il bug finale (stack corrupted)!!!
      sprintf(tmp, "Compo%d", i);
      if (remove(tmp) == -1) {
	fprintf(stderr, "failed to kill %s file !\n", tmp);
      }
    }
  } else {			/* PGX */
    for (i = 0; i < cp.th; i++) {
      char tmp[10];
      sprintf(tmp, "bandtile%d", i + 1);

      if (remove(tmp) == -1) {
	fprintf(stderr, "failed to kill %s file !\n", tmp);
      }
    }
  }

  /* Free memory */
  free(img.comps);
  free(cp_init.tcps);
  if (tcp_init->numlayers > 9) free(cp.matrice);
  for (tileno = 0; tileno < cp.tw * cp.th; tileno++)
    free(cp.tcps[tileno].tccps);
  free(cp.tcps);

  system("pause");
  return 0;
}
