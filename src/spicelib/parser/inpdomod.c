/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/

#include "ngspice.h"
#include <stdio.h>
#include "iferrmsg.h"
#include "inpdefs.h"
#include "inp.h"

/*--------------------------------------------------------------
 * This fcn takes the model card & examines it.  Depending upon
 * model type, it parses the model line, and then calls
 * INPmakeMod to stick the model name into the model list.
 * Note that multi-line models are handled in the calling fcn
 * (INPpas1).
 *-------------------------------------------------------------*/
char *INPdomodel(void *ckt, card * image, INPtables * tab)
{

    char *modname;
    int type = -1;
    int lev;
    char *typename;
    char *err = (char *) NULL;
    char *line;

    line = image->line;
    
    /* debug statement */
    /*    printf("In INPdomodel, examining line %s . . . \n", line); */

    INPgetTok(&line, &modname, 1);	/* throw away '.model' */
    INPgetTok(&line, &modname, 1);      /* get model name */
    INPinsert(&modname, tab);           /* stick model name into table */
    INPgetTok(&line, &typename, 1);     /* get model type */

    /*  -----  Check if model is a BJT --------- */
    if ((strcmp(typename, "npn") == 0) || (strcmp(typename, "pnp") == 0)) {
	err = INPfindLev(line,&lev);
	switch(lev) {
		case 0:
		case 1:
			type = INPtypelook("BJT");
			if (type < 0) {
	  		  err = INPmkTemp(
	  		          "Device type BJT not available in this binary\n");
			}
			break;
		case 2:
			 type = INPtypelook("BJT2");
			 if(type < 0) {
			   err = INPmkTemp(
                            "Device type BJT2 not available in this binary\n");
			 }
			 break;
		default: /* placeholder; use level 3 for the next model */
		         err = INPmkTemp(
		            "Only BJT levels 1 and 2 are supported in this binary\n");
		       break;

	}	
	INPmakeMod(modname, type, image);
    } /* end if ((strcmp(typename, "npn") == 0) || (strcmp(typename, "pnp") == 0)) */

    /*  --------  Check if model is a diode --------- */
    else if (strcmp(typename, "d") == 0) {
	type = INPtypelook("Diode");
	if (type < 0) {
	    err =
		INPmkTemp
		("Device type Diode not available in this binary\n");
	}
	INPmakeMod(modname, type, image);
    }  /*   else if (strcmp(typename, "d") == 0) {  */

    /*  --------  Check if model is a jfet --------- */
    else if ((strcmp(typename, "njf") == 0)
	     || (strcmp(typename, "pjf") == 0)) {
        err = INPfindLev(line, &lev);
	switch (lev) {
	case 0:
	case 1:
	    type = INPtypelook("JFET");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type JFET not available in this binary\n");
	    }
	    break;
	case 2:
	    type = INPtypelook("JFET2");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type JFET2 not available in this binary\n");
	    }
	    break;
	default:		/* placeholder; use level 3 for the next model */
	    err =
		INPmkTemp
		("Only JFET device levels 1-2 are supported in this binary\n");
	    break;
	}
	INPmakeMod(modname, type, image);
    }   /*   end  else if ((strcmp(typename, "njf") == 0) */

    /*  --------  Check if model is a ???? --------- */
    else if ((strcmp(typename, "nmf")   == 0)
	       || (strcmp(typename, "pmf")   == 0)
	       || (strcmp(typename, "nhfet") == 0)
	       || (strcmp(typename, "phfet") == 0)) {
	  err = INPfindLev( line, &lev );
	  switch ( lev ) 
	  {	
		case 0:
        	case 1:
        		type = INPtypelook("MES");
        		if (type < 0)
        		{
        			err = INPmkTemp("Device type MES not available\n");
        		}
        		break;
        	case 2:
        		type = INPtypelook("MESA");
        		if (type < 0)
        		{
        			err = INPmkTemp("Device type MES2 not availabe\n");
        		}
        		break;
			case 3:
        		type = INPtypelook("MESA");
        		if (type < 0)
        		{
        			err = INPmkTemp("Device type MES2 not availabe\n");
        		}
        		break;
        	case 4:
        		type = INPtypelook("MESA");
        		if ( type < 0)
        		{
        			err = INPmkTemp(" Device type MESA not available\n");
        		}
        		break;
        	case 5:
        		type = INPtypelook("HFET1");
        		if ( type < 0)
        		{
        			err = INPmkTemp(" Device type HFET1 not available\n");
        		}
        		break;
        	case 6:
        		type = INPtypelook("HFET2");
        		if ( type < 0)
        		{
        			err = INPmkTemp(" Device type HFET2 not available in this binary\n");
        		}
        		break;

        	default:
        		err = INPmkTemp("only mesfet device level 1  and hfet level 5-6 supported\n");
        		break;
        }
	INPmakeMod(modname, type, image);
    } 

    /*  --------  Check if model is a ???? --------- */
    else if (strcmp(typename, "urc") == 0) {
	type = INPtypelook("URC");
	if (type < 0) {
	    err =
		INPmkTemp
		("Device type URC not available in this binary\n");
	}
	INPmakeMod(modname, type, image);
    } 

    /*  --------  Check if model is a MOSFET --------- */
    else if ((strcmp(typename, "nmos") == 0)
	       || (strcmp(typename, "pmos") == 0)
	       || (strcmp(typename, "nsoi") == 0)
	       || (strcmp(typename, "psoi") == 0)) {
	err = INPfindLev(line, &lev);
	switch (lev) {
	case 0:
	case 1:
	    type = INPtypelook("Mos1");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type MOS1 not available in this binary\n");
	    }
	    break;
	case 2:
	    type = INPtypelook("Mos2");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type MOS2 not available in this binary\n");
	    }
	    break;
	case 3:
	    type = INPtypelook("Mos3");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type MOS3 not available in this binary\n");
	    }
	    break;
	case 4:
	    type = INPtypelook("BSIM1");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type BSIM1 not available in this binary\n");
	    }
	    break;
	case 5:
	    type = INPtypelook("BSIM2");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type BSIM2 not available in this binary\n");
	    }
	    break;
	case 6:
	    type = INPtypelook("Mos6");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type MOS6 not available in this binary\n");
	    }
	    break;
	case 7:
	    type = INPtypelook("MOS7");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type MOS7 not available in this binary\n");
	    }
	    break;
	case 8:
	    type = INPtypelook("BSIM3");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type BSIM3 not available in this binary\n");
	    }
	    break;
	    
	case  9:
	    type = INPtypelook("Mos9");
         if(type < 0) {
          err = INPmkTemp(
              "Device type MOS9 not available in this binary\n");
	    }
	    break;
	case 10:
	    type = INPtypelook("B3SOIPD");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type B3SOIPD not available in this binary\n");
	    }
	    break;
	case 11:
	    type = INPtypelook("B3SOIFD");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type B3SOIFD not available in this binary\n");
	    }
	    break;
	case 12:
	    type = INPtypelook("B3SOIDD");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type B3SOIDD not available in this binary\n");
	    }
	    break;        
	case 14:
	    type = INPtypelook("BSIM4");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type BSIM4 not available in this binary\n");}
	    break;
	case 15:
	    type = INPtypelook("BSIM5");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Placeholder: Device type BSIM5 not available in this binary\n");
	    }
	    break;
	case 16:
	    type = INPtypelook("BSIM6");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Placeholder: Device type BSIM6 not available in this binary\n");
	    }
	    break;
	case 44:
		type = INPtypelook("EKV");
		if (type < 0) {
		err =
		    INPmkTemp
		    ("Placeholder for EKV model: look at http://legwww.epfl.ch for info on EKV\n");
	    }
	    break;    
	case 49:
	    type = INPtypelook("BSIM3V1");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type BSIM3V1 not available in this binary\n");
	    }
	    break;
	case 50:
	    type = INPtypelook("BSIM3V2");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type BSIM3V2 not available in this binary\n");
	    }
	    break;	    
	case 60:
	    type = INPtypelook("SOI");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type SOI not available in this binary (internal STAG release)\n");
	    }
	    break;	 
	case 61:
	    type = INPtypelook("SOI2");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type SOI2 not available in this binary (internal STAG release)\n");
	    }
	    break;    
	
	case 62:
	    type = INPtypelook("SOI3");
	    if (type < 0) {
		err =
		    INPmkTemp
		    ("Device type SOI3 not available in this binary (internal STAG release)\n");
	    }
	    break;
	default:		/* placeholder; use level xxx for the next model */
	    err =
		INPmkTemp
		("Only MOS device levels 1-6,8,14,44,49-50,62 are supported in this binary\n");
	    break;
	}
	INPmakeMod(modname, type, image);
    } 


    /*  --------  Check if model is a resistor --------- */
    else if (strcmp(typename, "r") == 0) {
	type = INPtypelook("Resistor");
	if (type < 0) {
	    err =
		INPmkTemp
		("Device type Resistor not available in this binary\n");
	}
	INPmakeMod(modname, type, image);
    } 


    /*  --------  Check if model is a transmission line of some sort --------- */
    else if(strcmp(typename,"txl") == 0) {
      char *val;
      double rval=0, lval=0;
      INPgetTok(&line,&val,1);
      while (*line != '\0') {
	if (*val == 'R' || *val == 'r') {
	  INPgetTok(&line,&val,1);
	  rval = atof(val);
	}
	if ((strcmp(val,"L") == 0)  || (strcmp(val,"l") == 0)) {
	  INPgetTok(&line,&val,1);
	  lval = atof(val);
	}
	INPgetTok(&line,&val,1);
      }
      if(lval)
	rval = rval/lval;
      if (rval > 1.6e10) {
	type = INPtypelook("TransLine");
	INPmakeMod(modname,type,image);
      }
      if (rval > 1.6e9) {
	type = INPtypelook("CplLines");
	INPmakeMod(modname,type,image);
      }
      else {
	type = INPtypelook("TransLine");
	INPmakeMod(modname,type,image);
      }
      if(type < 0) {
	err = INPmkTemp(
			"Device type TransLine not available in this binary\n");
      }
      
    } 

    /*  --------  Check if model is a ???? --------- */
    else if(strcmp(typename,"cpl") == 0) {
      type = INPtypelook("CplLines");
      if(type < 0) {
	err = INPmkTemp(
			"Device type CplLines not available in this binary\n");
      }
      INPmakeMod(modname,type,image);

    } 


    /*  --------  Check if model is a cap --------- */
    else if (strcmp(typename, "c") == 0) {
        type = INPtypelook("Capacitor");
	if (type < 0) {
	    err =
		INPmkTemp
		("Device type Capacitor not available in this binary\n");
	}
	INPmakeMod(modname, type, image);
    } 


    /*  --------  Check if model is a switch --------- */
    else if (strcmp(typename, "sw") == 0) {
	type = INPtypelook("Switch");
	if (type < 0) {
	    err =
		INPmkTemp
		("Device type Switch not available in this binary\n");
	}
	INPmakeMod(modname, type, image);
    } 


    /*  --------  Check if model is a ???? --------- */
    else if (strcmp(typename, "csw") == 0) {
	type = INPtypelook("CSwitch");
	if (type < 0) {
	    err =
		INPmkTemp
		("Device type CSwitch not available in this binary\n");
	}
	INPmakeMod(modname, type, image);
    } 


    /*  --------  Check if model is a ???? --------- */
    else if (strcmp(typename, "ltra") == 0) {
	type = INPtypelook("LTRA");
	if (type < 0) {
	    err =
		INPmkTemp
		("Device type LTRA not available in this binary\n");
	}
	INPmakeMod(modname, type, image);
    } 


    /*  --------  Default action  --------- */
    else {
#ifndef XSPICE
      type = -1;
      err = (char *) MALLOC(35 + strlen(typename));
      (void) sprintf(err, "unknown model type %s - ignored\n", typename);
#else
      /* gtri - modify - wbk - 10/23/90 - modify to look for code models */
      
      /* add new code */
      
      /* look for this model type and put it in the table of models */
      type = INPtypelook(typename);
      if(type < 0) {
	err = (char *) MALLOC(35 + strlen(typename));
	sprintf(err,"Unknown model type %s - ignored\n",typename);
      }
      else
	INPmakeMod(modname,type,image);
      
      /* gtri - end - wbk - 10/23/90 */
#endif
      
    }
    return (err);
}
