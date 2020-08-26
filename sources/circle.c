/**
    @file
    circle.c - same as line modulo 360.

 Etienne Démoulin - 2020.

    this object has 1 inlet and 2 outlets
    it responds to ints and floats in inlet. output a line in the first outlet / bang when done in the second outlet.

    @ingroup    demoul
*/

#include "ext.h"            // you must include this - it contains the external object's link to available Max functions
#include "ext_obex.h"        // this is required for all objects using the newer style for writing objects.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef __APPLE__
#include <Carbon/Carbon.h>
#include <unistd.h>
#endif

#ifdef WIN32
#ifndef NAN
    static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
    #define NAN (*(const float *) __nan)
#endif
#endif


typedef struct _plussz {    // defines our object's internal variables for each instance in a patch
    t_object p_ob;            // object header - ALL objects MUST begin with this...
    double p_time;
    double p_dep;
    double p_inc;
    double p_arr;
    long p_step;
    long i;
    //PNPP
    double p_cur;
    
    void *p_outlet;            // 2nd output. Bang when done
    void *p_outlet2; //1rst output. Line out.
    
    void *m_clock;
    
} t_plussz;


// these are prototypes for the methods that are defined below

void plussz_assist(t_plussz *x, void *b, long m, long a, char *s); //help function
void plussz_free(t_plussz *x);  //destructor
void *plussz_new(t_symbol *s, long argc, t_atom *argv); //squelette
void plussz_float(t_plussz *x, double f); //when float called
void plussz_task(t_plussz *x); //clocker.
void plussz_int(t_plussz *x, long n); //when int called

t_class *plussz_class;        // global pointer to the object class - so max can reference the object


//--------------------------------------------------------------------------

void ext_main(void *r)
{
    t_class *c;

    c = class_new("circle", (method)plussz_new, (method)plussz_free, (long)sizeof(t_plussz), 0L, A_GIMME, 0);
    // class_new() loads our external's class into Max's memory so it can be used in a patch
    // plussz_new = object creation method defined below


    class_addmethod(c, (method)plussz_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)plussz_int, "int", A_LONG, 0);
    class_addmethod(c, (method)plussz_assist,    "assist",    A_CANT, 0);    // (optional) assistance method needs to be declared like this
    
    class_register(CLASS_BOX, c);
    plussz_class = c;

}


//--------------------------------------------------------------------------


void *plussz_new(t_symbol *s, long argc, t_atom *argv)        // argc = number of arguments // argv list of arguments.
{
    
    //Verifications.
    t_plussz *x = NULL;
    long time;
    if (argc != 1) //check there is 1 argument.
    {
        post("circle <time to reach>");
        return NULL;
    }
    time = atom_getfloat(argv);
    //check argument > 0.
    if (time < 0)
    {
        post("circle <time to reach>");
        post("<time to reach> > 0");
        return NULL;
    }

    x = (t_plussz *)object_alloc(plussz_class); // create a new instance of this object

    x->p_outlet = floatout(x);    // Create 2nd outlet.
    x->p_outlet2 = floatout(x); // Create 1rst outlet.
    
    //Define time and steps.
    x->p_time = time;
    x->p_step = x->p_time/20;
    //Define loop variables.
    x->p_cur = 0;
    x->i = 0;
    
    x->m_clock = clock_new((t_plussz *)x, (method)plussz_task); //define clock.

    return(x);                    // return a reference to the object instance
}


//--------------------------------------------------------------------------

void plussz_assist(t_plussz *x, void *b, long m, long a, char *s) // 4 final arguments are always the same for the assistance method
{
    if (m == ASSIST_INLET) {    // Inlets
        switch (a) {
        case 0: sprintf(s, "(number) destination to reach"); break;
        }
    }
    else {                        // Outlets
        switch (a) {
        case 0: sprintf(s, "(float) Line out"); break;
        case 1: sprintf(s, "(bang) when done"); break;
        }
    }
}

void plussz_task(t_plussz *x) //clocker
{
    x->p_cur+=x->p_inc;
    outlet_float(x->p_outlet2, fmod(x->p_cur,360));
    if (x->i < x->p_step)
    {
        x->i+=1;
        clock_fdelay(x->m_clock, 20.);
    }
    else
    {
        outlet_float(x->p_outlet2, x->p_arr);
        outlet_bang(x->p_outlet);
        x->p_dep = x->p_arr;
        clock_unset(x->m_clock);
        //post("la clock est stop");
    }
}

void plussz_free(t_plussz *x) //destructor
{
    object_free(x->m_clock);
}

void plussz_float(t_plussz *x, double f) //when float called.
{
    if (f <= 360 & f >= 0) //check incoming value.
    {


    x->i=0;
    x->p_arr = f;
    x->p_dep = x->p_cur;
    
    if (fabs(x->p_arr-x->p_dep) < 180) //case 1. distance < 180.
    {
    x->p_inc = (x->p_arr-x->p_dep)/x->p_step;
    x->p_cur = x->p_dep;
    clock_fdelay(x->m_clock, 0L);
    //post("step %.2f",n);
    }
    else if (x->p_arr-x->p_dep < 0) //case 2. distance > 180 and positive increment.
    {
        x->p_inc = ((x->p_arr-x->p_dep)+360)/x->p_step;
        x->p_cur = x->p_dep;
        clock_fdelay(x->m_clock, 0L);
    }
    else //case 3. Distance > 180 and negative increment.
    {
        x->p_inc = ((x->p_arr-x->p_dep)-360)/x->p_step;
        x->p_cur = x->p_dep+360;
        clock_fdelay(x->m_clock, 0L);
    }
    }
    else
    {
    post("FORBIDDEN, you should stay on range [0;360]");
    outlet_bang(x->p_outlet);
    }
}

void plussz_int(t_plussz *x, long n) //when inc called, convert to float.
{
    double f = n;
    plussz_float(x,f);
}
