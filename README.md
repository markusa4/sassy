# the sassy preprocessor for symmetry detection
The sassy preprocessor is designed to shrink large, sparse graphs. The idea is that before giving the graph to an off-the-shelf symmetry detection tool (such as bliss, dejavu, nauty, saucy, Traces), the graph is instead given to the preprocessor. The preprocessor shrinks the graph, in turn hopefully speeding up the subsequent solver.

Some technicalities apply, though: the graph format of sassy must be used as the input graph (`sgraph`). The graph format follows precisely the format of nauty / Traces, which is described in great detail [here](https://pallini.di.uniroma1.it/Guide.html). 

Also, a hook for automorphisms must be given to sassy (a `sassy_hook`), and automorphisms of the reduced graph must be translated back to the original graph. The preprocessor does the reverse translation, but a special hook must be given to the backend solver (see the example below for bliss).

At this point, the preprocessor comes in the form of a header-only library.

## The hook
A definition for a sassy_hook is given below:

    typedef void sassy_hook(int, const int *, int, const int *);

Note that the hook has four parameters, `int n`, `const int* p`, `int nsupp`, `const int* supp`. The meaning is as follows. The integer `n` gives the size of the domain of the symmetry, or in simple terms, the number of vertices of the graph. The array `p` is an array of length `n`. The described symmetry maps `i` to `p[i]`.

Crucially, `nsupp` and `supp` tell us which `i`'s are interesting at all: whenever `p[i] = i`, we do not iterate over `i`. To achieve this, `nsupp` first gives us the number of vertices where `p[i] != i`. Next, `supp[j]` for `0 <= j < nsupp` gives us the j-th vertex where `p[i] != i`. Iterating over `supp` yields all the points of the automorphism that are not the identity. Note that in many applications, not iterating over `p` in its entirety is crucial for decent performance.

An example is below:

    void hook(int n, const int *p, int nsupp, const int *supp) {
        for(int j = 0; j < nsupp; ++j) {
            const int i = supp[j];
            // do something with p[i]
        }
    }


## Example Usage with bliss

    #include "bliss/graph.hh"
    #include "sassy/preprocessor.h"
    #include "sassy/converters/bliss_converter.h"
    
    ...
    
    sassy::sgraph g;
    int* col = nullptr;
    
    // graph must be parsed into g here!
    
    // lets preprocess now...
    sassy::preprocessor p;
    // hook is a sassy_hook callback function
    p.reduce(&g, col, &hook);
    
    // ...and then we give the graph to bliss: first conver the graph
    bliss::Graph bliss_graph;
    convert_sassy_to_bliss(&g, col, &bliss_graph);
    
    // then call bliss
    bliss::Stats bliss_stat;
    bliss_graph.find_automorphisms(bliss_stat, sassy::preprocessor::bliss_hook, (void*) &p);
    
    // done!
    
## Work in progress
Note that this project is still being actively developed. I am happy to take suggestions, bug reports, ...
