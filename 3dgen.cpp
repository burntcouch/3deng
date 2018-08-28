// 
//3dgen.cpp
//

// utility functions for generating and modifiying objects
//
//  8/26/18 - started file, conversion of PartObj3D to new c++/SDL engine
//



#ifndef __DVECTOR
#include "vectoro.cpp"
#endif

#ifndef __3DBASE
#include "3dbase.cpp"    // XYCrd, Colora, Trig3D objects
#endif

#ifndef __3DOBJ
#include "3dobj.cpp"
#endif

#ifndef __3DGEN
#define __3DGEN 1

//
// Partition an object into 4 faces/per face
//
int PartObj3D(pObj3D pobj, int plvl)  {
	// plvl 1 = faces * 4, 2 -> 16, 3 -> 64, 4 -> 256, 5 -> 1024

	unsigned int fcnt;
	unsigned int i, k;
	pFace3D newface, fcurr, foldtail, ftmp;
	pVert3D v1, v2, vtemp, vdone;
	pVert3D newvert[3];
	pSeg3D s1, s2, sdummy, ns;

	DVector center = pobj->center;

	for (i = 0; i < plvl; i++) {   // Once for each level...

	  foldtail = pobj->ftail;  // Save old faces position, so's we can delete them after we are done
	  fcurr = pobj->fhead;     // start at the top of the face list
	  fcnt = 0;

	  while (1) {  // And once for each face
			//
			// first get the new vertices, three on each segment
			// if the new vert will be common to another face, and possibly
			// already built...don't do it again, just find the right one.
			//
			for (k = 0; k < 3; k++) {
				v1 = fcurr->v[k];
				v2 = fcurr->v[ (k + 1) % 3 ];   // stepping thru verts paired (0,1),(1,2),(3,0)
					// does seg not exist?
				sdummy = v1->addseg(v2);  // this builds the segs if they don't already exist
				s1 = v1->seg;
				s2 = v2->seg;

				// next we have to verify vert isn't already built, and which segs in v1, v2 it is on
				vdone = NULL;
				while (s1 != NULL) {  // hopefully there ARE some segs in v1
					if (s1->v == v2) {
						if (s1->vf != NULL) {  // did we already do this segment?
							vdone = s1->vf;  // this is the new vert here
						}
						break;  // found it
					}
					else s1 = s1->next;
				}

				while (s2 != NULL) {  // also, hope there ARE segs in v2
					if (s2->v == v1) break;  // found it
					else s2 = s2->next;
				}

				// Okay.  s1 and s2 now pointed at the right seg, so let's make the new vert if it's not there

				if (vdone == NULL) {  // if we haven't already done the new vert
						vtemp = new Vert3D(DV_midpt(v1->v, v2->v, center));
						pobj->vtail = vtemp;
						vtemp->next = NULL;
						newvert[k] = vtemp;
//
//  this wasn't working?
//					 	newvert[k] = pobj->addvert( DV_midpt(v1->v,	v2->v, center) );  // new vert
//
					 	s1->vf = newvert[k];  // save it in seg structure for later
					 	s2->vf = newvert[k];  //  on BOTH segs
				}
				else newvert[k] = vdone;
			}  		// end of k-loop (vertices for face j)
			// all new verts made/found for this face, so...
			//
			// Now make the four new faces...

			newface = pobj->addface();   // other info for new faces should be set, here...
		  newface->rgb = fcurr->rgb;
			newface->curv = fcurr->curv;
		  newface->v[0] = fcurr->v[0];
		  newface->v[1] = newvert[0];
		  newface->v[2] = newvert[2]; 
			fcnt++;

			newface = pobj->addface();
		  newface->rgb = fcurr->rgb;
			newface->curv = fcurr->curv;
		  newface->v[0] = newvert[0];
		  newface->v[1] = fcurr->v[1];
		  newface->v[2] = newvert[1]; 
			fcnt++;

			newface = pobj->addface();
		  newface->rgb = fcurr->rgb;
			newface->curv = fcurr->curv;
		  newface->v[0] = newvert[1];
		  newface->v[1] = fcurr->v[2];
		  newface->v[2] = newvert[2]; 
			fcnt++;

			newface = pobj->addface();
		  newface->rgb = fcurr->rgb;
			newface->curv = fcurr->curv;
		  newface->v[0] = newvert[0];
		  newface->v[1] = newvert[1];
		  newface->v[2] = newvert[2]; 
			fcnt++;

		  fcurr = fcurr->next;
		  if (fcurr == foldtail->next) break;   // reach end of face list
	  }  		// faces for level i

		// remove old faces now

		fcurr = pobj->fhead;
		while (1) {
			ftmp = fcurr->next;
			delete fcurr;
			if (ftmp == foldtail) break;
			fcurr = ftmp;
		}

		// wipe out old seg list
		v1 = pobj->vhead;
		while (v1 != NULL) {
			s1 = v1->seg;
			while (s1 != NULL) {
				s2 = s1->next;
				delete s1;
				s1 = s2;
			}
			v1->seg = NULL;
			v1 = v1->next;
		}
		pobj->fhead = foldtail->next;	
		delete foldtail;
	} 	// end of i-loop (levels)

	return (fcnt); // return new count of faces
}

#endif
// end of 3dgen.cpp
