/******************************************************************************
 * Project:  PROJ.4
 * Purpose:  Implementation of pj_log() function.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 2010, Frank Warmerdam
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *****************************************************************************/

#include "proj.h"
#include <projects.h>
#include <string.h>
#include <stdarg.h>

/************************************************************************/
/*                          pj_stderr_logger()                          */
/************************************************************************/

void pj_stderr_logger( void *app_data, int level, const char *msg )

{
    (void) app_data;
    (void) level;
    fprintf( stderr, "%s\n", msg );
}

/************************************************************************/
/*                               pj_vlog()                              */
/************************************************************************/

/* Workhorse for the log functions - relates to pj_log as vsprintf relates to sprintf */
static void pj_vlog( projCtx ctx, int level, const char *fmt, va_list args )

{
    char *msg_buf;

    if( level > ctx->debug_level )
        return;

    msg_buf = (char *) malloc(100000);
    if( msg_buf == NULL )
        return;

    /* we should use vsnprintf where available once we add configure detect.*/
    vsprintf( msg_buf, fmt, args );

    ctx->logger( ctx->app_data, level, msg_buf );

    free( msg_buf );
}


/************************************************************************/
/*                               pj_log()                               */
/************************************************************************/

void pj_log( projCtx ctx, int level, const char *fmt, ... )

{
    va_list args;

    if( level > ctx->debug_level )
        return;

    va_start( args, fmt );
    pj_vlog( ctx, level, fmt, args );
    va_end( args );
}



/************************************************************************/
/*             logging functions for the proj.h/obs_api                 */
/************************************************************************/

void pj_log_error (PJ *P, const char *fmt, ...)

{
    va_list args;
    va_start( args, fmt );
    pj_vlog (pj_get_ctx (P), PJ_LOG_ERROR , fmt, args);
    va_end( args );
}


void pj_log_debug (PJ *P, const char *fmt, ...)

{
    va_list args;
    va_start( args, fmt );
    pj_vlog (pj_get_ctx (P), PJ_LOG_DEBUG_MAJOR , fmt, args);
    va_end( args );
}


void pj_log_trace (PJ *P, const char *fmt, ...)

{
    va_list args;
    va_start( args, fmt );
    pj_vlog (pj_get_ctx (P), PJ_LOG_DEBUG_MINOR , fmt, args);
    va_end( args );
}
