#include <math.h>
#include "ext.h"
#include "ext_obex.h"

// declare object
typedef struct _pitchbend {
	t_object ob;
	long p_inlet0;
	long p_inlet1;
	void *p_outlet0;
	void *p_outlet1;
} t_pitchbend;

// function prototypes
void *pitchbend_new(void);
void pitchbend_assist(t_pitchbend *x, void *b, long m, long a, char *s);
void pitchbend_bang(t_pitchbend *x);
void pitchbend_in0(t_pitchbend *x, long n);
void pitchbend_in1(t_pitchbend *x, long n);

// global class pointer
void *pitchbend_class;

// object intialised
void ext_main(void *r) {
	t_class *c;
	c = class_new("pitchbend", (method)pitchbend_new, (method)NULL, sizeof(t_pitchbend), 0L, A_DEFLONG, 0);
	class_addmethod(c, (method)pitchbend_assist, "assist", A_CANT, 0);
	class_addmethod(c, (method)stdinletinfo, "inletinfo", A_CANT, 0);
	class_addmethod(c, (method)pitchbend_bang, "bang", 0);
	class_addmethod(c, (method)pitchbend_in0, "int", A_LONG, 0);
	class_addmethod(c, (method)pitchbend_in1, "in1", A_LONG, 0);
	class_register(CLASS_BOX, c);
	pitchbend_class = c;
}

// object created
void *pitchbend_new(void) {
	t_pitchbend *x;
    x = (t_pitchbend *)object_alloc(pitchbend_class);
	intin(x, 1);
	x->p_outlet1 = floatout((t_pitchbend *)x);
    x->p_outlet0 = floatout((t_pitchbend *)x);
	x->p_inlet1	= 0;
	x->p_inlet0	= 69;
	return(x);
}

// hover over inlets
void pitchbend_assist(t_pitchbend *x, void *b, long m, long a, char *s) {
	if (m == ASSIST_OUTLET)
		switch (a) {
		case 0:
			sprintf(s,"Outlet %ld: Hertz output", a);
			break;
		case 1:
			sprintf(s,"Outlet %ld: Cents output", a);
			break;
		}
	else {
		switch (a) {
		case 0:
			sprintf(s,"Inlet %ld: Midi Note number 0 - 127", a);
			break;
		case 1:
			sprintf(s,"Inlet %ld: Pitch bend value -8191 - 8191", a);
			break;
		}
	}
}

// all the methods
void pitchbend_in0(t_pitchbend *x, long n) {
	x->p_inlet0 = n;
	pitchbend_bang(x);
}

void pitchbend_in1(t_pitchbend *x, long n) {
	x->p_inlet1 = n;
}

void pitchbend_bang(t_pitchbend *x) {
    double detune;
    double pb;
    double hz;

    // calculate detune amount (essentially the scale object)
    detune = ((((double)x->p_inlet1 + 8191.) / (8191. + 8191.)) * (1. + 1.)) -1;
    pb = ((double)x->p_inlet0 + detune) - 69.;
    hz = 440.0 * pow(2, (pb / 12.));

    outlet_float(x->p_outlet1, pb);
    outlet_float(x->p_outlet0, hz);
}
