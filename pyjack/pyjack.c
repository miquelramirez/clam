/**
  * pyjackc - C module implementation for pyjack
  *
  * Copyright 2003 Andrew W. Schmeder <andy@a2hd.com>
  *
  * This source code is released under the terms of the GNU Public License.
  * See LICENSE for the full text of these terms.
  */

// Python includes
#include "Python.h"
#include "numpy/arrayobject.h"

// Jack
#include <jack/jack.h>
#include <jack/transport.h> //hack by AkhIL

// C standard
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

// Global shared data for jack

#define PYJACK_MAX_PORTS 256

typedef struct {
    jack_client_t* pjc;                             // Client handle
    int            buffer_size;                     // Buffer size
    int            num_inputs;                      // Number of input ports registered
    int            num_outputs;                     // Number of output ports registered
    jack_port_t*   input_ports[PYJACK_MAX_PORTS];   // Input ports
    jack_port_t*   output_ports[PYJACK_MAX_PORTS];  // Output ports
    fd_set         input_rfd;                       // fdlist for select
    fd_set         output_rfd;                      // fdlist for select
    int            input_pipe[2];                   // socket pair for input port data
    int            output_pipe[2];                  // socket pair for output port data
    float*         input_buffer_0;                  // buffer used to transmit audio via slink...
    float*         output_buffer_0;                 // buffer used to send audio via slink...
    float*         input_buffer_1;                  // buffer used to transmit audio via slink...
    float*         output_buffer_1;                 // buffer used to send audio via slink...
    int            input_buffer_size;               // buffer_size * num_inputs * sizeof(sample_t)
    int            output_buffer_size;              // buffer_size * num_outputs * sizeof(sample_t)
    int            iosync;                          // true when the python side synchronizing properly...
    int            event_graph_ordering;            // true when a graph ordering event has occured
    int            event_port_registration;         // true when a port registration event has occured
    int            event_sample_rate;               // true when a sample rate change has occured
    int            event_shutdown;                  // true when the jack server is shutdown
    int            event_hangup;                    // true when client got hangup signal
    int            active;                          // indicates if the client is currently process-enabled
} pyjack_client_t;

pyjack_client_t global_client;



// Initialize global data
void pyjack_init() {
    global_client.pjc = NULL;
    global_client.active = 0;
    global_client.iosync = 0;
    global_client.num_inputs = 0;
    global_client.num_outputs = 0;
    global_client.input_pipe[0] = 0;
    global_client.input_pipe[1] = 0;
    global_client.output_pipe[0] = 0;
    global_client.output_pipe[1] = 0;
    
    // Initialize unamed, raw datagram-type sockets...
    if (socketpair(PF_UNIX, SOCK_DGRAM, 0, global_client.input_pipe) == -1) {
        printf("ERROR: Failed to create socketpair input_pipe!!\n");
    }
    if (socketpair(PF_UNIX, SOCK_DGRAM, 0, global_client.output_pipe) == -1) {
        printf("ERROR: Failed to create socketpair output_pipe!!\n");
    }

    // Convention is that pipe[1] is the "write" end of the pipe, which is always non-blocking.
    fcntl(global_client.input_pipe[1], F_SETFL, O_NONBLOCK);
    fcntl(global_client.output_pipe[1], F_SETFL, O_NONBLOCK);
    fcntl(global_client.output_pipe[0], F_SETFL, O_NONBLOCK);
    
    // The read end, pipe[0], is blocking, but we use a select() call to make sure that data is really there.
    FD_ZERO(&global_client.input_rfd);
    FD_ZERO(&global_client.output_rfd);
    FD_SET(global_client.input_pipe[0], &global_client.input_rfd);
    FD_SET(global_client.output_pipe[0], &global_client.output_rfd);
    
    // Init buffers to null...
    global_client.input_buffer_size = 0;
    global_client.output_buffer_size = 0;
    global_client.input_buffer_0 = NULL;
    global_client.output_buffer_0 = NULL;
    global_client.input_buffer_1 = NULL;
    global_client.output_buffer_1 = NULL;
}

// Finalize global data
void pyjack_final() {
    global_client.pjc = NULL;
    // Free buffers...
    // Close socket...
    global_client.num_inputs = 0;
    global_client.num_outputs = 0;
}

// (Re)initialize socketpair buffers
void init_pipe_buffers(pyjack_client_t  * client) {
    // allocate buffers for send and recv
    if(global_client.input_buffer_size != client->num_inputs * client->buffer_size * sizeof(float)) {
        client->input_buffer_size = client->num_inputs * client->buffer_size * sizeof(float);
        client->input_buffer_0 = realloc(client->input_buffer_0, client->input_buffer_size);
        client->input_buffer_1 = realloc(client->input_buffer_1, client->input_buffer_size);
        //printf("Input buffer size %d bytes\n", input_buffer_size);
    }
    if(client->output_buffer_size != client->num_outputs * client->buffer_size * sizeof(float)) {
        client->output_buffer_size = client->num_outputs * client->buffer_size * sizeof(float);
        client->output_buffer_0 = realloc(client->output_buffer_0, client->output_buffer_size);
        client->output_buffer_1 = realloc(client->output_buffer_1, client->output_buffer_size);
        //printf("Output buffer size %d bytes\n", output_buffer_size);
    }
    
    // set socket buffers to same size as snd/rcv buffers
    setsockopt(client->input_pipe[0], SOL_SOCKET, SO_RCVBUF, &client->input_buffer_size, sizeof(int));
    setsockopt(client->input_pipe[0], SOL_SOCKET, SO_SNDBUF, &client->input_buffer_size, sizeof(int));
    setsockopt(client->input_pipe[1], SOL_SOCKET, SO_RCVBUF, &client->input_buffer_size, sizeof(int));
    setsockopt(client->input_pipe[1], SOL_SOCKET, SO_SNDBUF, &client->input_buffer_size, sizeof(int));
    
    setsockopt(client->output_pipe[0], SOL_SOCKET, SO_RCVBUF, &client->output_buffer_size, sizeof(int));
    setsockopt(client->output_pipe[0], SOL_SOCKET, SO_SNDBUF, &client->output_buffer_size, sizeof(int));
    setsockopt(client->output_pipe[1], SOL_SOCKET, SO_RCVBUF, &client->output_buffer_size, sizeof(int));
    setsockopt(client->output_pipe[1], SOL_SOCKET, SO_SNDBUF, &client->output_buffer_size, sizeof(int));
}


// RT function called by jack
int pyjack_process(jack_nframes_t n, void* arg) {
    int i, r;
    
    // Send input data to python side (non-blocking!)
    for(i = 0; i < global_client.num_inputs; i++) {
        memcpy(
            &global_client.input_buffer_0[global_client.buffer_size * i], 
            jack_port_get_buffer(global_client.input_ports[i], n), 
            (global_client.buffer_size * sizeof(float))
        );
    }
    
    r = write(global_client.input_pipe[1], global_client.input_buffer_0, global_client.input_buffer_size);
    
    if(r < 0) {
        global_client.iosync = 0;
    } else if(r == global_client.input_buffer_size) {
        global_client.iosync = 1;
    }
    
    // Read data from python side (non-blocking!)
    r = read(global_client.output_pipe[0], global_client.output_buffer_0, global_client.output_buffer_size);

    if(r == global_client.buffer_size * sizeof(float) * global_client.num_outputs) {
        for(i = 0; i < global_client.num_outputs; i++) {
            memcpy(
                jack_port_get_buffer(global_client.output_ports[i], global_client.buffer_size), 
                global_client.output_buffer_0 + (global_client.buffer_size * i),
                global_client.buffer_size * sizeof(float)
            );
        }
    } else {
        //printf("not enough data; skipping output\n");
    }
    return 0;
}

// Event notification of sample rate change
// (Can this even happen??)
int pyjack_sample_rate_changed(jack_nframes_t n, void* arg) {
    global_client.event_sample_rate = 1;
    return 0;
}

// Event notification of graph connect/disconnection
int pyjack_graph_order(void* arg) {
    global_client.event_graph_ordering = 1;
    return 0;
}

// Event notification of port registration or drop
void pyjack_port_registration(jack_port_id_t pid, int action, void* arg) {
    global_client.event_port_registration = 1;
}

// Shutdown handler
void pyjack_shutdown() {
    global_client.event_shutdown = 1;
    global_client.pjc = NULL;    
}

// SIGHUP handler
void pyjack_hangup() {
    global_client.event_hangup = 1;
    global_client.pjc = NULL;
}

// ------------- Python module stuff ---------------------

// Module exception object
static PyObject* JackError;
static PyObject* JackNotConnectedError;
static PyObject* JackUsageError;
static PyObject* JackInputSyncError;
static PyObject* JackOutputSyncError;

// Jack flags
static PyObject* IsInput;
static PyObject* IsOutput;
static PyObject* IsTerminal;
static PyObject* IsPhysical;
static PyObject* CanMonitor;
// hacked by AkhIL begin
static PyObject* TransportStopped;
static PyObject* TransportRolling;
static PyObject* TransportStarting;
// hacked by AkhIL end

// Attempt to connect to the Jack server
static PyObject* attach(PyObject* self, PyObject* args)
{
    char* cname;

    if(global_client.pjc != NULL) {
        PyErr_SetString(JackUsageError, "A connection is already established.");
        return NULL;
    }
    
    if (! PyArg_ParseTuple(args, "s", &cname))
        return NULL;
        
    jack_status_t status;
    global_client.pjc = jack_client_open(cname, JackNoStartServer, &status);
    if(global_client.pjc == NULL) {
        //TODO check status
        PyErr_SetString(JackNotConnectedError, "Failed to connect to Jack audio server.");
        return NULL;
    }
    
    jack_on_shutdown(global_client.pjc, pyjack_shutdown, NULL);
    signal(SIGHUP, pyjack_hangup);
    
    if(jack_set_process_callback(global_client.pjc, pyjack_process, NULL) != 0) {
        PyErr_SetString(JackError, "Failed to set jack process callback.");
        return NULL;
    }

    if(jack_set_sample_rate_callback(global_client.pjc, pyjack_sample_rate_changed, NULL) != 0) {
        PyErr_SetString(JackError, "Failed to set jack process callback.");
        return NULL;
    }

    if(jack_set_graph_order_callback(global_client.pjc, pyjack_graph_order, NULL) != 0) {
        PyErr_SetString(JackError, "Failed to set jack process callback.");
        return NULL;
    }

    if(jack_set_port_registration_callback(global_client.pjc, pyjack_port_registration, NULL) != 0) {
        PyErr_SetString(JackError, "Failed to set jack process callback.");
        return NULL;
    }
    
    // Get buffer size
    global_client.buffer_size = jack_get_buffer_size(global_client.pjc);
    
    // Success!
    Py_INCREF(Py_None);
    return Py_None;
}

// Detach client from the jack server (also destroys all connections)
static PyObject* detach(PyObject* self, PyObject* args)
{
    if(global_client.pjc != NULL) {
        jack_client_close(global_client.pjc);
        pyjack_final();
    }

    Py_INCREF(Py_None);
    return Py_None;
}

// Create a new port for this client
// Unregistration of ports is not supported; you must disconnect, reconnect, re-reg all ports instead.
static PyObject* register_port(PyObject* self, PyObject* args)
{
    jack_port_t* jp;
    int flags;
    char* pname;

    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }

    if(global_client.active) {
        PyErr_SetString(JackUsageError, "Cannot add ports while client is active.");
        return NULL;
    }
    
    if(global_client.num_inputs >= PYJACK_MAX_PORTS) {
        PyErr_SetString(JackUsageError, "Cannot create more than 256 ports.  Sorry.");
        return NULL;
    }
    
    if (! PyArg_ParseTuple(args, "si", &pname, &flags))
        return NULL;
        
    jp = jack_port_register(global_client.pjc, pname, JACK_DEFAULT_AUDIO_TYPE, flags, 0);

    if(jp == NULL) {
        PyErr_SetString(JackError, "Failed to create port.");
        return NULL;
    }
    
    // Store pointer to this port and increment counter
    if(flags & JackPortIsInput) {
        global_client.input_ports[global_client.num_inputs] = jp;
        global_client.num_inputs++;
    }
    if(flags & JackPortIsOutput) {
        global_client.output_ports[global_client.num_outputs] = jp;
        global_client.num_outputs++;
    }
        
    init_pipe_buffers(&global_client);
    Py_INCREF(Py_None);
    return Py_None;
}

// Returns a list of all port names registered in the Jack system
static PyObject* get_ports(PyObject* self, PyObject* args)
{
    PyObject* plist;
    const char** jplist;
    int i;

    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }
    
    jplist = jack_get_ports(global_client.pjc, NULL, NULL, 0);
    
    i = 0;
    plist = PyList_New(0);
    if(jplist != NULL) {
        while(jplist[i] != NULL) {
            PyList_Append(plist, Py_BuildValue("s", jplist[i]));
            //free(jplist[i]);  // Memory leak or not??
            i++;
        }
    }

    Py_INCREF(plist);
    return plist;
}

// Return port flags (an integer)
static PyObject* get_port_flags(PyObject* self, PyObject* args)
{
    char* pname;
    jack_port_t* jp;
    int i;

    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }

    if (! PyArg_ParseTuple(args, "s", &pname))
        return NULL;

    jp = jack_port_by_name(global_client.pjc, pname);
    if(jp == NULL) {
        PyErr_SetString(JackError, "Bad port name.");
        return NULL;
    }

    i = jack_port_flags(jp);
    if(i < 0) {
        PyErr_SetString(JackError, "Error getting port flags.");
        return NULL;
    }
    
    return Py_BuildValue("i", i);
}

// Return a list of full port names connected to the named port
// Port does not need to be owned by this client.
static PyObject* get_connections(PyObject* self, PyObject* args)
{
    char* pname;
    const char** jplist;
    jack_port_t* jp;
    PyObject* plist;
    int i;

    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }
    
    if (! PyArg_ParseTuple(args, "s", &pname))
        return NULL;
        
    jp = jack_port_by_name(global_client.pjc, pname);
    if(jp == NULL) {
        PyErr_SetString(JackError, "Bad port name.");
        return NULL;
    }
        
    jplist = jack_port_get_all_connections(global_client.pjc, jp);
    
    i = 0;
    plist = PyList_New(0);
    if(jplist != NULL) {
        while(jplist[i] != NULL) {
            PyList_Append(plist, Py_BuildValue("s", jplist[i]));
            //free(jplist[i]);  // memory leak or not?
            i++;
        }
    }

    Py_INCREF(plist);
    return plist;
}


// connect_port
static PyObject* port_connect(PyObject* self, PyObject* args)
{
    char* src_name;
    char* dst_name;
    
    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }

    if (! PyArg_ParseTuple(args, "ss", &src_name, &dst_name))
        return NULL;

    jack_port_t * src = jack_port_by_name(global_client.pjc, src_name);
    if (!src) {
        PyErr_SetString(JackUsageError, "Non existing source port.");
        return NULL;
        }
    jack_port_t * dst = jack_port_by_name(global_client.pjc, dst_name);
    if (!dst) {
        PyErr_SetString(JackUsageError, "Non existing destination port.");
        return NULL;
        }
    if(! global_client.active) {
        if(jack_port_is_mine(global_client.pjc, src) || jack_port_is_mine(global_client.pjc, dst)) {
            PyErr_SetString(JackUsageError, "Jack client must be activated to connect own ports.");
            return NULL;
        }
    }
    int error = jack_connect(global_client.pjc, src_name, dst_name);
    if (error !=0 && error != EEXIST) {
        PyErr_SetString(JackError, "Failed to connect ports.");
        return NULL;
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

// disconnect_port
static PyObject* port_disconnect(PyObject* self, PyObject* args)
{
    char* src_name;
    char* dst_name;
    
    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }

    if (! PyArg_ParseTuple(args, "ss", &src_name, &dst_name))
        return NULL;
    
    jack_port_t * src = jack_port_by_name(global_client.pjc, src_name);
    if (!src) {
        PyErr_SetString(JackUsageError, "Non existing source port.");
        return NULL;
        }
    jack_port_t * dst = jack_port_by_name(global_client.pjc, dst_name);
    if (!dst) {
        PyErr_SetString(JackUsageError, "Non existing destination port.");
        return NULL;
        }

    if(jack_port_connected_to(src, dst_name)) {
        if(jack_disconnect(global_client.pjc, src_name, dst_name) != 0) {
            PyErr_SetString(JackError, "Failed to connect ports.");
            return NULL;
        }
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

// get_buffer_size
static PyObject* get_buffer_size(PyObject* self, PyObject* args)
{
    int bs;
    
    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }

    bs = jack_get_buffer_size(global_client.pjc);
    return Py_BuildValue("i", bs);
}

// get_sample_rate
static PyObject* get_sample_rate(PyObject* self, PyObject* args)
{
    int sr;

    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }
    
    sr = jack_get_sample_rate(global_client.pjc);
    return Py_BuildValue("i", sr);
}

// activate 
static PyObject* activate(PyObject* self, PyObject* args)
{
    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }

    if(global_client.active) {
        PyErr_SetString(JackUsageError, "Client is already active.");
        return NULL;
    }

    if(jack_activate(global_client.pjc) != 0) {
        PyErr_SetString(JackUsageError, "Could not activate client.");
        return NULL;
    }

    global_client.active = 1;
    Py_INCREF(Py_None);
    return Py_None;
}

// deactivate
static PyObject* deactivate(PyObject* self, PyObject* args)
{
    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }

    if(! global_client.active) {
        PyErr_SetString(JackUsageError, "Client is not active.");
        return NULL;
    }
    
    if(jack_deactivate(global_client.pjc) != 0) {
        PyErr_SetString(JackError, "Could not deactivate client.");
        return NULL;
    }
    
    global_client.active = 0;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject * get_client_name(PyObject* self, PyObject* args)
{
    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }
    return Py_BuildValue("s", jack_get_client_name(global_client.pjc));
}

/** Commit a chunk of audio for the outgoing stream, if any.
  * Return the next chunk of audio from the incoming stream, if any
  */
static PyObject* process(PyObject* self, PyObject *args)
{
    int j, c, r;
    PyArrayObject *input_array;
    PyArrayObject *output_array;
    
    if(! global_client.active) {
        PyErr_SetString(JackUsageError, "Client is not active.");
        return NULL;
    }
    
    // Import the first and only arg...
    if (! PyArg_ParseTuple(args, "O!O!", &PyArray_Type, &output_array, &PyArray_Type, &input_array))
        return NULL;
        
    if(input_array->descr->type_num != PyArray_FLOAT || output_array->descr->type_num != PyArray_FLOAT) {
        PyErr_SetString(PyExc_ValueError, "arrays must be of type float");
        return NULL;
    }
    if(input_array->nd != 2 || output_array->nd != 2) {
        printf("%d, %d\n", input_array->nd, output_array->nd);
        PyErr_SetString(PyExc_ValueError, "arrays must be two dimensional");
        return NULL;
    }
    if((global_client.num_inputs > 0 && input_array->dimensions[1] != global_client.buffer_size) || 
       (global_client.num_outputs > 0 && output_array->dimensions[1] != global_client.buffer_size)) {
        PyErr_SetString(PyExc_ValueError, "columns of arrays must match buffer size.");
        return NULL;
    }
    if(global_client.num_inputs > 0 && input_array->dimensions[0] != global_client.num_inputs) {
        PyErr_SetString(PyExc_ValueError, "rows for input array must match number of input ports");
        return NULL;
    }
    if(global_client.num_outputs > 0 && output_array->dimensions[0] != global_client.num_outputs) {
        PyErr_SetString(PyExc_ValueError, "rows for output array must match number of output ports");
        return NULL;
    }

    // Get input data
    // If we are out of sync, there might be bad data in the buffer
    // So we have to throw that away first...
    
    r = read(global_client.input_pipe[0], global_client.input_buffer_1, global_client.input_buffer_size);
    
    // Copy data into array...
    for(c = 0; c < global_client.num_inputs; c++) {
        for(j = 0; j < global_client.buffer_size; j++) {
            memcpy(
                input_array->data + (c*input_array->strides[0] + j*input_array->strides[1]), 
                global_client.input_buffer_1 + j + (c*global_client.buffer_size), 
                sizeof(float)
            );
        }
    }
    
    if(0 && ! global_client.iosync) {
        PyErr_SetString(JackInputSyncError, "Input data stream is not synchronized.");
        return NULL;
    }
    
    // Copy output data into output buffer...
    for(c = 0; c < global_client.num_outputs; c++) {
        for(j = 0; j < global_client.buffer_size; j++) {
            memcpy(&global_client.output_buffer_1[j + (c*global_client.buffer_size)],
                   output_array->data + c*output_array->strides[0] + j*output_array->strides[1],
                   sizeof(float)
            );
        }
    }

    // Send... raise an exception if the output data stream is full.
    r = write(global_client.output_pipe[1], global_client.output_buffer_1, global_client.output_buffer_size);
    if(r != global_client.output_buffer_size) {
        PyErr_SetString(JackOutputSyncError, "Failed to write output data.");
        return NULL;
    }

    // Okay...    
    Py_INCREF(Py_None);
    return Py_None;
}

// Return event status numbers...
static PyObject* check_events(PyObject* self, PyObject *args)
{
    PyObject* d;
    d = PyDict_New();
    if(d == NULL) {
        return NULL;
    }
    
    PyDict_SetItemString(d, "graph_ordering", Py_BuildValue("i", global_client.event_graph_ordering));
    PyDict_SetItemString(d, "port_registration", Py_BuildValue("i", global_client.event_port_registration));
    PyDict_SetItemString(d, "shutdown", Py_BuildValue("i", global_client.event_shutdown));
    PyDict_SetItemString(d, "hangup", Py_BuildValue("i", global_client.event_hangup));
    
    // Reset all
    global_client.event_graph_ordering = 0;
    global_client.event_port_registration = 0;
    global_client.event_shutdown = 0;
    global_client.event_hangup = 0;

    return d;
}

// hack by AkhIL begin
static PyObject* get_frame_time(PyObject* self, PyObject* args)
{
    int frt;

    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }
    
    frt = jack_frame_time(global_client.pjc);
    return Py_BuildValue("i", frt);
}
static PyObject* get_current_transport_frame(PyObject* self, PyObject* args)
{
    int ftr;

    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }
    
    ftr = jack_get_current_transport_frame(global_client.pjc);
    return Py_BuildValue("i", ftr);
}
static PyObject* transport_locate (PyObject* self, PyObject* args)
{
    //jack_position_t pos;
    //jack_transport_state_t transport_state;
    jack_nframes_t newfr;

    //int newfr;

    if (! PyArg_ParseTuple(args, "i", &newfr))
        return NULL;

    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }

    jack_transport_locate (global_client.pjc,newfr);
    //transport_state = jack_transport_query (global_client.pjc, &pos);
    //pos.frame = newfr;

    return Py_None;
}
static PyObject* get_transport_state (PyObject* self, PyObject* args)
{
    //int state;

    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }
    
    jack_transport_state_t transport_state;
    transport_state = jack_transport_query (global_client.pjc, NULL);

    return Py_BuildValue("i", transport_state);
}
static PyObject* transport_stop (PyObject* self, PyObject* args)
{
    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }

    jack_transport_stop (global_client.pjc);

    return Py_None;
}
static PyObject* transport_start (PyObject* self, PyObject* args)
{
    if(global_client.pjc == NULL) {
        PyErr_SetString(JackNotConnectedError, "Jack connection has not yet been established.");
        return NULL;
    }

    jack_transport_start (global_client.pjc);

    return Py_None;
}
// hack by AkhIL end




// Python Module definition ---------------------------------------------------

static PyMethodDef pyjack_methods[] = {
  {"attach",             attach,                  METH_VARARGS, "attach(name):\n  Attach client to the Jack server"},
  {"detach",             detach,                  METH_VARARGS, "detach():\n  Detach client from the Jack server"},
  {"activate",           activate,                METH_VARARGS, "activate():\n  Activate audio processing"},
  {"deactivate",         deactivate,              METH_VARARGS, "deactivate():\n  Deactivate audio processing"},
  {"connect",            port_connect,            METH_VARARGS, "connect(source, destination):\n  Connect two ports, given by name"},
  {"disconnect",         port_disconnect,         METH_VARARGS, "disconnect(source, destination):\n  Disconnect two ports, given by name"},
  {"process",            process,                 METH_VARARGS, "process(output_array, input_array):\n  Exchange I/O data with RT Jack thread"},
  {"get_client_name",    get_client_name,         METH_VARARGS, "client_name():\n  Returns the actual name of the client"},
  {"register_port",      register_port,           METH_VARARGS, "register_port(name, flags):\n  Register a new port for this client"},
  {"get_ports",          get_ports,               METH_VARARGS, "get_ports():\n  Get a list of all ports in the Jack graph"},
  {"get_port_flags",     get_port_flags,          METH_VARARGS, "get_port_flags():\n  Return flags of a port (flags are bits in an integer)"},
  {"get_connections",    get_connections,         METH_VARARGS, "get_connections():\n  Get a list of all ports connected to a port"},
  {"get_buffer_size",    get_buffer_size,         METH_VARARGS, "get_buffer_size():\n  Get the buffer size currently in use"},
  {"get_sample_rate",    get_sample_rate,         METH_VARARGS, "get_sample_rate():\n  Get the sample rate currently in use"},
  {"check_events",       check_events,            METH_VARARGS, "check_events():\n  Check for event notifications"},
// hack by AkhIL begin
  {"get_frame_time",	 get_frame_time,          METH_VARARGS, "get_frame_time():\n  Get current frame time"},
  {"get_current_transport_frame", get_current_transport_frame,  METH_VARARGS, "get_current_transport_frame():\n  Get current transport frame"},
  {"transport_locate",   transport_locate,	  METH_VARARGS, "transport_locate(frame):\n  Set current transport frame"},
  {"get_transport_state", get_transport_state,    METH_VARARGS, "get_transport_state():\n  Get current transport state"},
  {"transport_stop",	transport_stop,		  METH_VARARGS, "transport_stop():\n  Stopping transport"},
  {"transport_start",	transport_start,		  METH_VARARGS, "transport_start():\n  Starting transport"},
// hack by AkhIL end
  {NULL, NULL}
};
 
DL_EXPORT(void)
initjack(void)
{
  PyObject *m, *d;
  
  m = Py_InitModule3("jack", pyjack_methods,
	"This module provides bindings to manage clients for the Jack Audio Connection Kit architecture");
  if (m == NULL)
    goto fail;
  d = PyModule_GetDict(m);
  if (d == NULL)
    goto fail;
 
  JackError = PyErr_NewException("jack.Error", NULL, NULL);
  JackNotConnectedError = PyErr_NewException("jack.NotConnectedError", NULL, NULL);
  JackUsageError = PyErr_NewException("jack.UsageError", NULL, NULL);
  JackInputSyncError = PyErr_NewException("jack.InputSyncError", NULL, NULL);
  JackOutputSyncError = PyErr_NewException("jack.OutputSyncError", NULL, NULL);
  IsInput = Py_BuildValue("i", JackPortIsInput);
  IsOutput = Py_BuildValue("i", JackPortIsOutput);
  IsTerminal = Py_BuildValue("i", JackPortIsTerminal);
  IsPhysical = Py_BuildValue("i", JackPortIsPhysical);
  CanMonitor = Py_BuildValue("i", JackPortCanMonitor);
// hacked by AkhIL begin
  TransportStopped = Py_BuildValue("i", JackTransportStopped);
  TransportRolling = Py_BuildValue("i", JackTransportRolling);
  TransportStarting = Py_BuildValue("i", JackTransportStarting);
// hacked by AkhIL end

  
  PyDict_SetItemString(d, "Error", JackError);
  PyDict_SetItemString(d, "NotConnectedError", JackNotConnectedError);
  PyDict_SetItemString(d, "UsageError", JackUsageError);
  PyDict_SetItemString(d, "InputSyncError", JackInputSyncError);
  PyDict_SetItemString(d, "OutputSyncError", JackOutputSyncError);
  PyDict_SetItemString(d, "IsInput", IsInput);
  PyDict_SetItemString(d, "IsOutput", IsOutput);
  PyDict_SetItemString(d, "IsTerminal", IsTerminal);
  PyDict_SetItemString(d, "IsPhysical", IsPhysical);
  PyDict_SetItemString(d, "CanMonitor", CanMonitor);
// hacked by AkhIL begin
  PyDict_SetItemString(d, "TransportStopped", TransportStopped);
  PyDict_SetItemString(d, "TransportRolling", TransportRolling);
  PyDict_SetItemString(d, "TransportStarting", TransportStarting);
// hacked by AkhIL end
  
  // Enable Numeric module
  import_array();
 
  if (PyErr_Occurred())
    goto fail;

  // Init jack data structures
  pyjack_init();
 
  return;
 
fail:
  Py_FatalError("Failed to initialize module pyjack");
}
