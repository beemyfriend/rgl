##
## R source file
## This file is part of rgl
##
## $Id: enum.R,v 1.1 2003/03/25 00:13:21 dadler Exp $
##


##
## ===[ SECTION: enumerations ]===============================================
##

##
## FUNCTIONS
##   rgl.enum
##
## utility function translating literals to ids
##

rgl.enum   <- function ( name, ... )
{
  choices <- list( ... )
  names   <- attr(choices,"names")

  pos <- pmatch( name, names )

  if ( is.na(pos) )
    stop("symbolic value must be of ", list(names) )

  id  <- choices[[pos]]
  
  return( id )
}

##
## ENUM FUNCTIONS
##

rgl.enum.nodetype <- function (type)
  return ( rgl.enum( type, shapes=1, lights=2, bboxdeco=3 ) )

rgl.enum.pixfmt <- function (fmt)
  return ( rgl.enum( fmt, png=0 ) )

rgl.enum.polymode <- function (mode)
  return ( rgl.enum( mode, filled=1, lines=2, points=3, culled=4) )

rgl.enum.textype <- function (textype)
  return ( rgl.enum( textype, alpha=1, luminance=2, luminance.alpha=3, rgb=4, rgba=5 ) )

rgl.enum.fogtype <- function (fogtype)
  return ( rgl.enum (fogtype, none=1, linear=2, exp=3, exp2=4) )

rgl.enum.primtype <- function (primtype)
  return ( rgl.enum( primtype, points=1, lines=2, triangles=3, quadrangles=4 ) )

rgl.enum.halign <- function( halign)
  return ( rgl.enum (halign, left=-1, center=0, right=1 ) )

