Python bindings for OpenCog
---------------------------

== Requirements ==

* Python 2.7 - these bindings may work with earlier versions, but they have not been tested at all.
* Cython 0.14 or later. http://www.cython.org/
* Nosetests - for running unit tests.

Both Cython and Nosetests can be installed with easy_install:

 sudo easy_install cython nose

The bindings are written mostly using Cython, which allows writing
code that looks pythonic but gets compiled to C.  It also makes it
trivial to access both Python objects and C objects without using a
bunch of extra Python library API calls.

Currently the package structure looks like this:

 opencog.atomspace
 opencog.atomspace.types
 opencog.cogserver
 opencog.pymoses
 opencog.scheme_wrapper

Eventually, when other components of OpenCog are accessible, they'll
follow a similar pattern:

 opencog.rules

== Tutorial ==

This tutorial is a first look at the Python bindings. It assumes that
you've got a good grasp on the concept of the AtomSpace and the
CogServer. Oh, and it helps to know a bit of Python too!

=== Setting up ===

Go through the normal process of [[building OpenCog]]. Then ensure that
the OpenCog data directory is in your Python `sys.path`. By
default, the opencog python module lives at
`/usr/local/share/opencog/python` when you do
`make install`, and you can modify your `PYTHONPATH`
environment variable to ensure Python checks that location. If you
just want to use your build dir you can use something like:

 $ export PYTHONPATH=$PYTHONPATH:~/src/opencog/build/opencog/cython

=== AtomSpace API ===

These bindings let you interact and instantiate [[Atom]]s interactively.
The [http://ipython.scipy.org/ IPython] shell is recommended.

Here's how to add a node:

<source lang="python">
>>> from opencog.atomspace import AtomSpace, types

>>> a = AtomSpace()
>>> a.add_node(types.ConceptNode, "My first python created node")
<opencog.atomspace.Atom object at 0x203fa80>
</source>

To make things more succinct when referring to types, you can
alias stuff:

<source lang="python">
>>> t=types
>>> a.add_node(t.ConceptNode, "Ah, more concise")
>>> ConceptNode = t.ConceptNode
>>> a.add_node(ConceptNode, "Ah, a bit more concise")
</source>

You'll notice these return opencog.atomspace.Atom objects, which
internally store a Handle with a UUID and the AtomSpace it's
connected to:

<source lang="python">
>>> atom = a.add_node(ConceptNode, "handle bar")
>>> str(atom.h)
'<UUID:4>'
>>> atom.atomspace
<opencog.atomspace.AtomSpace object at 0x203220a>
</source>

Or you may get a different UUID. The atom will fetch and cache information
about itself behind the scenes.

<source lang="python">
>>> str(an_atom)
'node[ConceptNode:handle bar]'
>>> an_atom.long_string()
'node[ConceptNode:handle bar] av:(0,0) tv:([0.000000,0.000000=0.000000])'
>>> an_atom.name
'handle bar'
>>> str(an_atom.t) # can also use an_atom.type if you want to be verbose
'3'
>>> an_atom.type_name
'ConceptNode'
</source>

Note that on the second reference to an_atom.t, it won't recheck the
AtomSpace for the atom type because it's immutable and gets cached
internally. If you try to set an immutable property of an Atom, you
get an AttributeError exception:

<source lang="python">
>>> an_atom.name = 'change your name man, it sucks.'
AttributeError: attribute 'name' of 'opencog.atomspace.Atom' objects is not writable
</source>

I guess the 'handle bar' Atom is stuck with it's amusing but
unfortunate name.

Lets create our first link:

<source lang="python">
>>> n1 = a.add_node(t.ConceptNode, "I can refer to this atom now")
>>> n2 = a.add_node(t.ConceptNode, "this one too")
>>> l = a.add_link(t.SimilarityLink, [n1,n2])
>>> l.out
[Atom(Handle(5),<opencog.atomspace.AtomSpace object at 0x203220a>),
 Atom(Handle(6),<opencog.atomspace.AtomSpace object at 0x203220a>)]
</source>

Up until now we've ignored [[Truthvalues]]. The TruthValue
implementation is not entirely complete. Currently the bindings only
support "[[SimpleTruthValues]]", since these are the most frequently used.

<source lang="python">
>>> from opencog.atomspace import TruthValue
>>> alink.tv
'[0.000000,0.000000=0.000000]'
>>> alink.tv = TruthValue(0.5,0.1)
>>> alink
'[SimilarityLink <I can refer to this atom now,this one too> 0.500000 0.200000]'
</source>

Next up is queries to the AtomSpace. What if we want to iterate over
all the ConceptNodes we've added so far? Most of the AtomSpace
getHandleSet methods have been wrapped and also have more descriptive
names for each variant.

<source lang="python">
>>> my_nodes = a.get_atoms_by_type(t.ConceptNode)
>>> my_nodes
[Atom(Handle(6),<opencog.atomspace.AtomSpace object at 0x203220a>),
 Atom(Handle(5),<opencog.atomspace.AtomSpace object at 0x203220a>),
 Atom(Handle(4),<opencog.atomspace.AtomSpace object at 0x203220a>),
 Atom(Handle(3),<opencog.atomspace.AtomSpace object at 0x203220a>),
 Atom(Handle(2),<opencog.atomspace.AtomSpace object at 0x203220a>),
 Atom(Handle(1),<opencog.atomspace.AtomSpace object at 0x203220a>)]
</source>

By default, subtypes are also retrieved, but we can exclude subtypes
if we want to be specific.

<source lang="python">
>>> a.add_node(t.Node, "I am the one true Node")
>>> a.get_atoms_by_type(t.Node,subtype=False)
[Atom(Handle(8),<opencog.atomspace.AtomSpace object at 0x203220a>)]
>>> print my_specific_nodes[0]
node[Node:I am the one true Node]
</source>

There are other queries by type, outgoing set, name etc. See
`tests/opencog/cython/test_atomspace.py` for the complete picture.

Note: You can also now use the AtomSpace.add() method which automatically
determines and checks the required arguments for the type:

<source lang="python">
>>> a1 = a.add(t.Node, name="Easier this way")
>>> a2 = a.add(t.Node, name=".. much easier")
>>> a.add(t.Link, out=[a1,a2])
Atom(Handle(11),<opencog.atomspace.AtomSpace object at 0x203220a>)
</source>

==== The AtomSpace as a container ====

The AtomSpace supports the container methods Python expects for
a container-like object:

<source lang="python">
>>> alink in a    # is this atom in this atomspace
True
>>> h in a        # works for handles too
True
>>> len(a)        # how many atoms are in the atomspace?
8
>>> a[h]          # using [] notation, returns the Atom, or IndexError exception
                  # if handle not in AtomSpace
Atom(Handle(4),<opencog.atomspace.AtomSpace object at 0x14b2918>)
</source>

I'm very interested in making this Pythonic as possible, so if you notice
aberrations please [mailto:joel@opencog.org email me].

=== MindAgents in Python ===

MindAgents modify the AtomSpace autonomously. Adding and removing atoms,
updating TruthValues or anything else. The most important part for now is
the "run" method, which gets called with the CogServer AtomSpace as a parameter
(In the past, C++ MindAgents would be passed the CogServer itself, but it
wasn't obvious to me why this is necessary).

<source lang="python">
>>> import opencog.cogserver
>>> from opencog.atomspace import types
>>> class MyMindAgent(opencog.cogserver.MindAgent):
...    def run(self,atomspace):
...        atomspace.add_node(types.ConceptNode, "test")
</source>

This will try, every CogServer cycle, to add the ConceptNode called "test".

''Warning'': Note the opencog.cogserver.MindAgent is subclassed using the full
path. If you use:

<source lang="python">
>>> from opencog.cogserver import MindAgent # wrong
</source>

Then you'll get MindAgent showing up in the cogserver too. I'm not currently
sure why, but supporting this is another improvement to add eventually.

To actually allow the CogServer to use this MindAgent there are several things to check:

# Place the module containing the MindAgent within one of these place:
#* <your OpenCog data directory>/python
#* a directory specified in the configuration parameter PYTHON_EXTENSION_DIRS,
#* somewhere on your PYTHONPATH.
# Ensure you are loading the libPythonModule.so CogServer plugin.
# Either use the CogServer loadpy command to load the module (you should leave off the .py extension, e.g. "loadpy my_module") or place the module name in  the configuration parameter PYTHON_PRELOAD.

Note: Yet another thing to do is allow all modules in the OpenCog Python
directories to be loaded automatically and scanned for MindAgents.

=== CogServer Requests in Python ===

CogServer Requests are commands that can be issued in the shell or by other
modules in C++ code. If save the follow to a file myrequest.py:

<source lang="python">
import opencog.cogserver
from opencog.atomspace import types
class MyRequest(opencog.cogserver.Request):
    def run(self,args,atomspace):
        # args is a list of strings
        atomspace.add_node(types.ConceptNode, args[0])
</source>

And you telnet to a running CogServer, you can run the request:

 $ telnet localhost 17001
 opencog> loadpy myrequest
 opencog> myrequest.MyRequest blah

Then the request will have added a ConceptNode with the name "blah"

== Improving the bindings ==

There is more that can be added to the Python bindings.

See the Cython documentation, and then check out the source code in
SOURCE_ROOT/opencog/cython/opencog. Note the use of Cython .pxd
definition files which show how to make C++ classes and functions
available to cython code.

=== TODO ===

* Support add/remove signals (but depends on changing from boost::signals
  to ZeroMQ messaging first)
  (Why? ZMQ messgng is about 50x-200x slower than boost::signals ... )

* It'd also be nice to somehow import types using
  `from opencog.atomspace.types import ConceptNode`
  (or use a * import)
