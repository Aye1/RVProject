/*
     PLIB - A Suite of Portable Game Libraries
     Copyright (C) 1998,2002  Steve Baker
 
     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Library General Public
     License as published by the Free Software Foundation; either
     version 2 of the License, or (at your option) any later version.
 
     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Library General Public License for more details.
 
     You should have received a copy of the GNU Library General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 
     For further information visit http://plib.sourceforge.net

*/

//
// ASC (ascii files, used for example by 3DS R4 (the DOS version)) export for SSG/PLIB
// Written by Wolfram Kuss in Mar-2003
//

#include "ssgLocal.h"
#include "ssgLoaderWriterStuff.h"

static FILE *save_fd ;
static ssgSimpleStateArray gSSL;



static void save_vtx_table ( ssgVtxTable *vt )
{
  GLenum mode = vt -> getPrimitiveType () ;
  if ( mode != GL_TRIANGLES &&
    mode != GL_TRIANGLE_FAN &&
    mode != GL_TRIANGLE_STRIP )
  {
    //only triangle export
    return;
  }

/*
  Named object:
*/
  int j ;

 
  int istate = gSSL.findIndex ( (ssgSimpleState*)vt->getState () ) ;
  int num_vert = vt -> getNumVertices () ;
  int num_face = vt -> getNumTriangles () ;

  ssgSimpleState* st = ( istate != -1 )? gSSL.get( istate ): 0;
	bool mapped =  ( st && st -> isEnabled ( GL_TEXTURE_2D ) &&
    vt -> getNumTexCoords () == num_vert );

  fprintf ( save_fd, "Named object: \"%s\"\n", vt->getPrintableName());
  fprintf ( save_fd, "Tri-mesh, Vertices: %d     Faces: %d\n", num_vert, num_face );
	if(mapped)	
		fprintf ( save_fd, "Mapped\n"); 
  
/*
  Vertex list:
*/
  fprintf ( save_fd, "Vertex list:\n"); 
	const char *material = "PALGREY27";
	ssgState *s = vt->getState();
	if (s)
		if(s->isAKindOf(ssgTypeSimpleState()))
		{
	
			ssgTexture * t = ((ssgSimpleState *)s)->getTexture();
			if(t)
			{
				char *fn = t->getFilename();
				if (fn)
				{
				
					char *tsA = new char [strlen(fn)+1];
					strcpy(tsA, fn);
					char *t = strrchr(tsA, '.');
					if (t)
					{
						// special handling for *.0af .. *.9af to be able to differntiate between them:
						if((t[2]=='a') && (t[3]=='f'))
							*t='_';
						else
							*t=0;
					}

					t = strrchr(tsA, '\\');
					if (!t)
						t = tsA;
					else
						t++;
					

						
					material = t;
				}
			}
		}
	for ( j = 0; j < num_vert; j++ )
  {
    sgVec3 v;
    sgCopyVec3 ( v, vt->getVertex ( j ) ) ;
    fprintf ( save_fd, "Vertex %d: X:%f Y:%f Z:%f", j, 100.0*v[0], 100.0*v[1], 100.0*v[2] ); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if(mapped)
	  {
			sgVec2 tv ;
      sgCopyVec2 ( tv, vt->getTexCoord ( j ) ) ;
			// ***************************************************
			// WK: Originally I did not have the "U:" and "V:" !!!
			// Strange !! Maybe there are two formats??
			// ***************************************************
      fprintf ( save_fd, " U:%f V:%f\n", tv[0], tv[1]); // 1.0f - tv[1]);
		}
 		else
		  fprintf ( save_fd, "\n"); 
  }
  
/*
  Face list:
*/
	if(num_face > 0)
		fprintf ( save_fd, "Face list:\n"); 
	for ( j = 0; j < num_face; j++ )
  {
    short i1,i2,i3;
    vt -> getTriangle ( j, &i1, &i2, &i3 ) ;

    fprintf ( save_fd, "Face %d: A:%d B:%d C:%d AB:1 BC:1 CA:1\n", j, i1, i2, i3 ); 
		fprintf ( save_fd, "Material:\"%s\"\n", material ); 
  }
  fprintf ( save_fd, "\n");  // empty line after the named object
  
/*
  Item MESH_NUMTVERTEX.
*/
  
  fflush ( save_fd ) ;
/*
  Item NUMCVERTEX.
*/
  //fprintf ( save_fd, "    *MESH_NUMCVERTEX 0\n" );

  //if ( st )    fprintf ( save_fd, "  *MATERIAL_REF %d\n", gSSL.findIndex ( st ) );

}

// wk: Kludge !!!
// if the "calling app" like PPE sets this, then only nodes with spare ==1 are saved !!
int bUseSpare=FALSE; 

static void save_geom ( ssgEntity *e )
{
  if ( e -> isAKindOf ( ssgTypeBranch() ) )
  {
    ssgBranch *br = (ssgBranch *) e ;

    if ( br -> isAKindOf ( ssgTypeSelector() ) )
    {
      save_geom ( br -> getKid ( 0 ) ) ;
    }
    else
    {
      for ( int i = 0 ; i < br -> getNumKids () ; i++ )
        save_geom ( br -> getKid ( i ) ) ;
    }
  }
  else
  if ( e -> isAKindOf ( ssgTypeVtxTable() ) )
	{ int bSaveIt = TRUE;
	  if(bUseSpare)
			if( 1 != e->getSpare() )
				bSaveIt = FALSE;

		if ( bSaveIt )
		{
			ssgVtxTable *vt = (ssgVtxTable *) e ;
			save_vtx_table ( vt ) ;
		}
	}
}


int ssgSaveASC ( FILE* fileout, ssgEntity *ent )
{
  save_fd = fileout ;
  
/*
  Write the header.
*/
  fprintf ( save_fd, "Ambient light color: Red=0.039216 Green=0.039216 Blue=0.039216\n" );
  fprintf ( save_fd, "Solid background color: Red=0.372549 Green=0.454902 Blue=0.639216\n" );
  
  
  gSSL.collect ( ent ) ;
  //save_states () ;
  save_geom ( ent ) ;  
	gSSL.removeAll();  

  fflush ( save_fd ) ;
  
  return TRUE ;
}


int ssgSaveASC ( const char *filename, ssgEntity *ent )
{
  save_fd = fopen ( filename, "wa" ) ;

  if ( save_fd == NULL )
  {
    ulSetError ( UL_WARNING, "ssgSaveASC: Failed to open '%s' for writing", filename ) ;
    return FALSE ;
  }
  
  int result = ssgSaveASC ( save_fd, ent ) ;

  fclose ( save_fd ) ;
  
  return result ;
}
