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

     $Id: ssgBranch.cxx,v 1.26 2004/02/16 20:51:12 stromberg Exp $
*/


#include "ssgLocal.h"

void ssgBranch::copy_from ( ssgBranch *src, int clone_flags )
{
  ssgEntity::copy_from ( src, clone_flags ) ;

  for ( int i = 0 ; i < src -> getNumKids () ; i++ )
  {
    ssgEntity *k = src -> getKid ( i ) ;

    if ( k != NULL && ( clone_flags & SSG_CLONE_RECURSIVE ) )
      addKid ( (ssgEntity *)( k -> clone ( clone_flags )) ) ;
    else
      addKid ( k ) ;
  }
}

ssgBase *ssgBranch::clone ( int clone_flags )
{
  ssgBranch *b = new ssgBranch ;
  b -> copy_from ( this, clone_flags ) ;
  return b ;
}



ssgBranch::ssgBranch (void)
{
  type = ssgTypeBranch () ;
}


ssgBranch::~ssgBranch (void)
{
  removeAllKids () ;
}


void ssgBranch::zeroSpareRecursive ()
{
  zeroSpare () ;

  for ( ssgEntity *k = getKid ( 0 ) ; k != NULL ; k = getNextKid () )
    k -> zeroSpareRecursive () ;
}


void ssgBranch::recalcBSphere (void)
{
  emptyBSphere () ;

  for ( ssgEntity *k = getKid ( 0 ) ; k != NULL ; k = getNextKid () )
    extendBSphere ( k -> getBSphere () ) ;

  bsphere_is_invalid = FALSE ;
}


void ssgBranch::addKid ( ssgEntity *entity )
{
  kids. addEntity ( entity ) ;
  entity -> addParent ( this ) ;
  dirtyBSphere () ;
}


void ssgBranch::removeKid  ( int n )
{
  ssgEntity *k = kids.getEntity ( n ) ;

  if ( k != NULL ) {
    k -> removeParent ( this ) ;
    kids.removeEntity ( n ) ;
    dirtyBSphere () ;
  }
}


void ssgBranch::removeKid ( ssgEntity *entity )
{
  removeKid( searchForKid( entity ) );
}


void ssgBranch::removeAllKids (void)
{
  for ( int k = getNumKids() - 1; k >= 0; --k ) {
    removeKid ( k ) ;
  }
}


void ssgBranch::replaceKid ( int n, ssgEntity *new_entity )
{
  if ( n >= 0 && n < getNumKids () )
  {
    getKid ( n ) -> removeParent ( this ) ;
    kids.replaceEntity ( n, new_entity ) ;
    new_entity -> addParent ( this ) ;
    dirtyBSphere () ;
  }
}

void ssgBranch::replaceKid ( ssgEntity *old_entity, ssgEntity *new_entity )
{
  replaceKid ( searchForKid( old_entity ), new_entity ) ;
}


void ssgBranch::print ( FILE *fd, char *indent, int how_much )
{
  ssgEntity::print ( fd, indent, how_much ) ;
  fprintf ( fd, "%s  Num Kids=%d\n", indent, getNumKids() ) ;

  if ( getNumParents() != getRef() )
    ulSetError ( UL_WARNING, "Ref count doesn't tally with parent count" ) ;

	if ( how_much > 1 )
  {	if ( bsphere.isEmpty() )
			fprintf ( fd, "%s  BSphere is Empty.\n", indent ) ;
		else
			fprintf ( fd, "%s  BSphere  R=%g, C=(%g,%g,%g)\n", indent,
				bsphere.getRadius(), bsphere.getCenter()[0], bsphere.getCenter()[1], bsphere.getCenter()[2] ) ;
	}

  char in [ 100 ] ;
  sprintf ( in, "%s  ", indent ) ;

  for ( ssgEntity *e = getKid ( 0 ) ; e != NULL ; e = getNextKid() )
    e -> print ( fd, in, how_much ) ;
}


void ssgBranch::getStats ( int *num_branches, int *num_leaves, int *num_tris, int *num_verts )
{
  int nb, nl, nt, nv ;

  *num_branches = 1 ;   /* this! */
  *num_leaves   = 0 ;
  *num_tris     = 0 ;
  *num_verts    = 0 ;

  for ( int i = 0 ; i < getNumKids () ; i++ )
  {
    ssgEntity *e = getKid ( i ) ;

    e -> getStats ( & nb, & nl, & nt, & nv ) ;
    *num_branches += nb ;
    *num_leaves   += nl ;
    *num_tris     += nt ;
    *num_verts    += nv ;
  }
}


ssgEntity *ssgBranch::getByName ( char *match )
{
  if ( getName() != NULL && strcmp ( getName(), match ) == 0 )
    return this ;

  /* Otherwise check the kids for a match */

  for ( ssgEntity* k = getKid ( 0 ) ; k != NULL ; k = getNextKid() )
  {
    ssgEntity *e = k -> getByName ( match ) ;

    if ( e != NULL )
      return e ;
  }

  return NULL ;
}


ssgEntity *ssgBranch::getByPath ( char *path )
{
  /* Ignore leading '/' */

  if ( *path == '/' )
    ++path ;

  char *n = getName () ;

  /*
    If this node has no name then pass the request down the tree
  */

  if ( n == NULL )
  {
    for ( ssgEntity* k = getKid ( 0 ) ; k != NULL ; k = getNextKid () )
    {
      ssgEntity *e = k -> getByPath ( path ) ;

      if ( e != NULL )
        return e ;
    }

    return NULL ;
  }

  /*
    If this node does have a name - but it doesn't match the
    next part of the path then punt.
  */

  unsigned int l = strlen ( n ) ;

  if ( strlen ( path ) < l || strncmp ( n, path, l ) != 0 )
    return NULL ;

  /*
    If the first part of the path is this ssgBranch, we
    may have a winner.
  */

  char c = path [ l ] ;

  /* If we reached the end of the path - we win! */

  if ( c == '\0' )
    return this ;

  if ( c == '/' )
  {
    /* If the path continues, try to follow the path to the kids */

    for ( ssgEntity* k = getKid ( 0 ) ; k != NULL ; k = getNextKid () )
    {
      ssgEntity *e = k -> getByPath ( path + l ) ;

      if ( e != NULL )
        return e ;
    }
  }

  return NULL ;
}
 


void ssgBranch::cull ( sgFrustum *f, sgMat4 m, int test_needed )
{
  if ( ! preTravTests ( &test_needed, SSGTRAV_CULL ) )
    return ;

  int cull_result = cull_test ( f, m, test_needed ) ;

  if ( cull_result == SSG_OUTSIDE )
    return ;

  for ( ssgEntity *e = getKid ( 0 ) ; e != NULL ; e = getNextKid() )
    e -> cull ( f, m, cull_result != SSG_INSIDE ) ;

  postTravTests ( SSGTRAV_CULL ) ; 
}



void ssgBranch::hot ( sgVec3 s, sgMat4 m, int test_needed )
{
  if ( ! preTravTests ( &test_needed, SSGTRAV_HOT ) )
    return ;

  int hot_result = hot_test ( s, m, test_needed ) ;

  if ( hot_result == SSG_OUTSIDE )
    return ;

  _ssgPushPath ( this ) ;

  for ( ssgEntity *e = getKid ( 0 ) ; e != NULL ; e = getNextKid() )
    e -> hot ( s, m, hot_result != SSG_INSIDE ) ;

  _ssgPopPath () ;

  postTravTests ( SSGTRAV_HOT ) ;
}



void ssgBranch::los ( sgVec3 s, sgMat4 m, int test_needed )
{
  if ( ! preTravTests ( &test_needed, SSGTRAV_LOS ) )
    return ;

  int los_result = los_test ( s, m, test_needed ) ;

  if ( los_result == SSG_OUTSIDE )
    return ;

  _ssgPushPath ( this ) ;

  for ( ssgEntity *e = getKid ( 0 ) ; e != NULL ; e = getNextKid() )
    e -> los ( s, m, los_result != SSG_INSIDE ) ;

  _ssgPopPath () ;

  postTravTests ( SSGTRAV_LOS) ;
}


void ssgBranch::isect ( sgSphere *s, sgMat4 m, int test_needed )
{
  if ( ! preTravTests ( &test_needed, SSGTRAV_ISECT ) )
    return ;

  int isect_result = isect_test ( s, m, test_needed ) ;

  if ( isect_result == SSG_OUTSIDE )
    return ;

  _ssgPushPath ( this ) ;

  for ( ssgEntity *e = getKid ( 0 ) ; e != NULL ; e = getNextKid() )
    e -> isect ( s, m, isect_result != SSG_INSIDE ) ;

  _ssgPopPath () ;

  postTravTests ( SSGTRAV_ISECT ) ; 
}


int ssgBranch::load ( FILE *fd )
{
  int nkids ;

  _ssgReadInt ( fd, & nkids ) ;

  if ( ! ssgEntity::load ( fd ) )
    return FALSE ;

  for ( int i = 0 ; i < nkids ; i++ )
  {
    ssgEntity *kid ;

    if ( ! _ssgLoadObject ( fd, (ssgBase **) &kid, ssgTypeEntity () ) )
      return FALSE ;

    addKid ( kid ) ;
  }

  return TRUE ;
}


int ssgBranch::save ( FILE *fd )
{
  _ssgWriteInt ( fd, getNumKids() ) ;

  if ( ! ssgEntity::save ( fd ) )
    return FALSE ;

  for ( int i = 0 ; i < getNumKids() ; i++ )
  {
    if ( ! _ssgSaveObject ( fd, getKid ( i ) ) )
       return FALSE ;
  }

  return TRUE ;
}

// ----------------------- code by W.K for merging hierarchy nodes ------------------------------


// #define VERBOSE
int maxTriangles = 100; // Thats a nice value for handling MDL files for BoB. 
                        // You might want to assign another value, possibly -1 before calling mergeHNodes
int maxVertices = 195; // see maxTriangles



static int noOfMergedNodes; // number of merged nodes

void AddLeafToTriangles(ssgVtxArray *pSrc , ssgVtxArray *pDest)
// pDest must be of type GL_TRIANGLES
// So, this function adds the contents of a leaf to a GL_TRIANGLES node.
{	
	// Add vertices and keep track of old to new indexes
	if(pSrc->getNumTriangles()==0)		return; // wk 14.7.2002
	int * aiOld2NewIndex = new int[pSrc->getNumVertices()];
	int iSrc; // iDest;
	for(iSrc = 0; iSrc < pSrc->getNumVertices(); iSrc++)
	{ float * pfSrc = pSrc->getVertex(iSrc);
	  int bFound = FALSE;
	  /* test
		for(iDest = 0; iDest < pDest->getNumVertices(); iDest++)
		{ float * pfDest = pDest->getVertex(iDest);
#define MYABS(x) ((x>0)?(x):(-(x)))
			if (MYABS(pfSrc[0]-pfDest[0])+MYABS(pfSrc[1]-pfDest[1])+
				  MYABS(pfSrc[2]-pfDest[2]) < 0.0001)
			{ bFound = TRUE;
			  aiOld2NewIndex[iSrc] = iDest;
			}
		}*/
		if(!bFound)
		{	aiOld2NewIndex[iSrc] = pDest->getNumVertices();
			pDest->vertices->add(pfSrc);
			float * f = pSrc->getNormal(iSrc);
			if (f)
				pDest->normals->add(f);
			else
			{	
				pDest->normals->add(_ssgNormalUp);
			}
			f = pSrc->getTexCoord(iSrc);
			if (f)
				pDest->texcoords->add(f);
			else
			{
				pDest->texcoords->add(_ssgTexCoord00);
			}
	//	assert(pSrc->colours->getNum()==0);
		}
	
	}
	// Add faces
	pDest->addIndex(aiOld2NewIndex[*pSrc->getIndex(0)]);
	pDest->addIndex(aiOld2NewIndex[*pSrc->getIndex(1)]);
	pDest->addIndex(aiOld2NewIndex[*pSrc->getIndex(2)]);
  if(pSrc->getGLtype()==GL_TRIANGLE_FAN)
	{
		for(iSrc=1;iSrc<pSrc->getNumTriangles();iSrc++)
		{ // add 0, i+1, i+2
			pDest->addIndex(aiOld2NewIndex[*pSrc->getIndex(0)]);
			pDest->addIndex(aiOld2NewIndex[*pSrc->getIndex(iSrc+1)]);
			pDest->addIndex(aiOld2NewIndex[*pSrc->getIndex(iSrc+2)]);
		
		}
	}
	else
	{ assert(pSrc->getGLtype()==GL_TRIANGLES);
		for(iSrc=1;iSrc<pSrc->getNumTriangles();iSrc++)
		{ // add 0, i+1, i+2
			pDest->addIndex(aiOld2NewIndex[*pSrc->getIndex(iSrc*3)]);
			pDest->addIndex(aiOld2NewIndex[*pSrc->getIndex(iSrc*3+1)]);
			pDest->addIndex(aiOld2NewIndex[*pSrc->getIndex(iSrc*3+2)]);
		
		}
	}

	// State
	pDest->setState(pSrc ->getState());
	delete aiOld2NewIndex ;
}



void recursiveMergeHNodes  ( ssgEntity *root, int delta )
// This is the recursive part of "mergeHNodes".
// Walks the scene graph and looks for nodes to merge.
// Uses AddLeafToTriangles for the actual merging
{
  if ( root == NULL )
    return ;

  if ( root -> isAKindOf ( ssgTypeBranch() ) )
  { // see whether we can merge some of the kids
    ssgBranch *b = (ssgBranch *) root ;
		
    int i1, // loop var.
			oldnk=b -> getNumKids (), oldi1=-1 ; // for debug purpose only
		i1 = 0 ; 
		while(i1 < b -> getNumKids () )
		{ 
			// check that each loop increases i1 or decreases b -> getNumKids ()
      if(!((i1>oldi1) || (oldnk>b ->getNumKids ())))
				break;
			assert((i1>oldi1) || (oldnk>b ->getNumKids ()));
			oldi1=i1;
			oldnk=b ->getNumKids ();

			ssgEntity * pThis = b -> getKid ( i1 ) ;
			if ( pThis -> isAKindOf ( ssgTypeBranch() ) )
			{	recursiveMergeHNodes ( pThis, delta ); // recursively go deeper into the scene graph
				i1++;
			}
			else if(( pThis -> isAKindOf ( ssgTypeLeaf() ) ) && (i1+delta < b -> getNumKids ()))
			{ ssgEntity * pOther = b -> getKid ( i1+delta );
				if( pOther -> isAKindOf ( ssgTypeLeaf() ) )
				{ ssgLeaf *l1 = (ssgLeaf *)pThis, *l2 = (ssgLeaf *)pOther;
					// now: l1 = kid at i1, l2 = kid at i1+delta		
					if ( (l1->getState() == l2->getState()) && 
						   ((maxTriangles < 0) || (l1->getNumTriangles()+l2->getNumTriangles()<maxTriangles )) &&
							 ((maxVertices < 0) || (l1->getNumVertices()+l2->getNumVertices()<maxVertices )) &&
							 (0 == strcmp(l1->getPrintableName(), l2->getPrintableName())) 
							) 
					{
						if	(((l1->getGLtype() == GL_TRIANGLE_FAN) || (l1->getGLtype() == GL_TRIANGLES)) &&
							((l2->getGLtype() == GL_TRIANGLE_FAN) || (l2->getGLtype() == GL_TRIANGLES)))
						{ // For using MDL files, these are all the types that we need ...
							
							if (l1->isA(ssgTypeVtxTable()))
							{	l1 = ((ssgVtxTable *)l1)->getAs_ssgVtxArray(); // Kludge: mem leak
							  b->replaceKid    ( i1, l1 ) ;
							}
							if (l2->isA(ssgTypeVtxTable()))
							{	l2 = ((ssgVtxTable *)l2)->getAs_ssgVtxArray(); // Kludge: mem leak
							  b->replaceKid    ( i1+delta, l2 ) ;
							}
							assert(l1 -> isAKindOf ( ssgTypeVtxArray() ));
							assert(l2 -> isAKindOf ( ssgTypeVtxArray() ));
							if((l1->getNumTriangles()+l2->getNumTriangles())<1300) // ??? wk kludge
							{	
#ifdef VERBOSE
								printf("I do sthg :-) %d\n", noOfMergedNodes++);
#else
								noOfMergedNodes++;
#endif
								if(l1->getGLtype() == GL_TRIANGLE_FAN) // convert to GL_TRIANGLES
								{ ssgVtxArray * pNewArray = new ssgVtxArray(GL_TRIANGLES, 
											 new ssgVertexArray(), 
											 new ssgNormalArray(), new ssgTexCoordArray(), 
											 new ssgColourArray(), new ssgIndexArray());
									AddLeafToTriangles((ssgVtxArray *)l1, pNewArray);
									pNewArray->setName(l1->getPrintableName());
									assert(l1==b->getKid(i1));
									b->removeKid(i1);
									b->addKid(pNewArray);
									pNewArray->dirtyBSphere  ();
									l1 = pNewArray;
								}
								// l1 is of type GL_TRIANGLES now.
								{ //ssgEntity *t= b->getKid(i1+delta);
									//assert(l2==t);
								}
								AddLeafToTriangles((ssgVtxArray *)l2, (ssgVtxArray *)l1);
								l1->dirtyBSphere  ();
								l2->dirtyBSphere  (); // unnessesary ?
								b->removeKid(l2); // kid(i1+delta) changed because of remove!?!       i1+delta);
							}
						}
						else
						{	i1++;
						  printf("wrong types: %ld, %ld, num Trias: %ld, %ld\n", 
								 (long)l1->getGLtype(), (long)l1->getGLtype(),
								 (long)l1->getNumTriangles(), (long)l2->getNumTriangles());
						}
					}
					else
						i1++;
				}
				else 
					i1++;
			}
			else
				i1++;
		}
		


/*
		int i1=0;
		for ( i = 0 ; i < b -> getNumKids () ; i++ )
    { ssgEntity * pThis = b -> getKid ( i1 ) ;
			if ( pThis -> isAKindOf ( ssgTypeBranch() ) )
			{	recursiveMergeHNodes  ( pThis );
				i1++;
			}
			else
			{ assert( pThis -> isAKindOf ( ssgTypeLeaf() ) );
			  ssgLeaf *l1 = (ssgLeaf *)pThis;
				assert(l1 -> isAKindOf ( ssgTypeVtxArray() ));
				static int noOfMergedNodes=0;
				printf("I could do sthg :-) %d\n", noOfMergedNodes++);
				assert(l1->getGLtype() == GL_TRIANGLE_FAN); // convert to GL_TRIANGLES
				ssgVtxArray * pNewArray = new ssgVtxArray(GL_TRIANGLES, 
							 new ssgVertexArray(), new ssgNormalArray(), 
							 new ssgTexCoordArray(), new ssgColourArray(), 
							 new ssgIndexArray());
				AddLeafToTriangles((ssgVtxArray *)l1, pNewArray);
				b->removeKid(i1);
				pNewArray->dirtyBSphere  ();
				b->addKid(pNewArray);
				
				pNewArray->setName("meins!");
			}
			
		}
		*/
  }
}


void ssgBranch::mergeHNodes()
// merges hierarchy nodes if (not iff :-/):
// a) Both have the same state 
// b) the same name
// c) the result would not have more than maxTriangles triangles (if maxTriangles is negative, this is not checked).
// d) the result would not have more than maxVertices vertices (if maxVertices is negative, this is not checked).
// e) both are of type GL_TRIANGLE_FAN or GL_TRIANGLES
{


int deltas[] = { 1 ,2 ,1 ,3 ,2 ,1 ,2 ,4 ,2 ,1 ,5 ,1 ,2 ,6 ,2 ,1 ,2 ,7 ,2 ,1 ,2 ,1 ,8 ,1 ,2 ,1 ,2 ,9 ,2 ,1 ,2 ,1, 
              10 ,1 ,2 ,1 ,2 ,11 ,1 ,12 ,1 ,2 ,13 ,1 ,2 ,14 ,2 ,1 ,1 ,15 ,1 ,16 ,1 ,17 ,2 ,1 ,18 ,1 ,22 ,2 ,25 ,
							2 ,1 ,30 ,2 ,1 ,2 ,1 ,13 ,2 ,1 ,2 ,1 };
	noOfMergedNodes = 0;
	for(unsigned i=0; i < sizeof(deltas)/sizeof(int); i++)
	{
		recursiveMergeHNodes ( this, deltas[i] );
#ifdef VERBOSE
		printf("############################################### ^%d\n". deltas[i]);
#endif
	}
	printf("%d nodes were merged!\n", noOfMergedNodes);
}

