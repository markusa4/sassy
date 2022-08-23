#ifndef DEJAVU_COLORING_H
#define DEJAVU_COLORING_H


#include <vector>
#include <cassert>
#include "utility.h"

namespace sassy {
    template<class vertex_t>
    class coloring {
    public:
        vertex_t *bulk_alloc;
        vertex_t *bulk_pt;

        vertex_t *lab;
        vertex_t *ptn;

        int lab_sz;
        int ptn_sz;
        bool init = false;
        bool efficient_alloc = false;
        vertex_t *vertex_to_col;
        vertex_t *vertex_to_lab;

        int cells = 1;
        int smallest_cell_lower_bound = INT32_MAX;

        ~coloring() {
            if (init) {
                dealloc();
            }
        }

        void alloc(int sz) {
            if (!init) {
                lab = new vertex_t[sz];
                ptn = new vertex_t[sz];
                vertex_to_col = new vertex_t[sz];
                vertex_to_lab = new vertex_t[sz];
                efficient_alloc = false;
                init = true;

                lab_sz = sz;
                ptn_sz = sz;
            }
        }

        void dealloc() {
            delete[] ptn;
            delete[] lab;
            delete[] vertex_to_lab;
            delete[] vertex_to_col;
        };

        void copy_ptn(coloring<vertex_t> *c) {
            assert(init);
            assert(c->init);
            memcpy(ptn, c->ptn, c->ptn_sz * sizeof(vertex_t));
        }

        void copy(coloring<vertex_t> *c) {
            if (init) {
                if (lab_sz != c->lab_sz || ptn_sz != c->ptn_sz) {
                    dealloc();
                    init = false;
                } else {
                    cells = c->cells;
                    if (!efficient_alloc || !c->efficient_alloc) {
                        for (int i = 0; i < c->ptn_sz;) {
                            const vertex_t rd = c->ptn[i];
                            ptn[i] = rd;
                            i += rd + 1;
                        }
                        memcpy(vertex_to_col, c->vertex_to_col, c->ptn_sz * sizeof(vertex_t));
                    } else {
                        for (int i = 0; i < c->ptn_sz;) {
                            const vertex_t rd = c->ptn[i];
                            ptn[i] = rd;
                            i += rd + 1;
                        }
                        memcpy(vertex_to_col, c->vertex_to_col, c->ptn_sz * sizeof(vertex_t));
                    }
                    return;
                }
            }

            if (!init) {
                alloc(c->lab_sz);
            }

            if (c->cells > c->ptn_sz / 4) {
                memcpy(ptn, c->ptn, c->ptn_sz * sizeof(vertex_t));
            } else {
                for (int i = 0; i < c->ptn_sz;) {
                    const vertex_t rd = c->ptn[i];
                    ptn[i] = rd;
                    i += rd + 1;
                }
            }
            memcpy(lab, c->lab, c->lab_sz * sizeof(vertex_t));
            memcpy(vertex_to_col, c->vertex_to_col, c->lab_sz * sizeof(vertex_t));
            memcpy(vertex_to_lab, c->vertex_to_lab, c->lab_sz * sizeof(vertex_t));

            lab_sz = c->lab_sz;
            ptn_sz = c->ptn_sz;

            cells = c->cells;
            smallest_cell_lower_bound = c->smallest_cell_lower_bound;
            init = true;
        }

        void copy_force(coloring<vertex_t> *c) {
            if (init) {
                if (lab_sz != c->lab_sz || ptn_sz != c->ptn_sz) {
                    dealloc();
                    init = false;
                }
            }

            if (!init) {
                alloc(c->lab_sz);
            }

            if (c->cells > c->ptn_sz / 4) {
                memcpy(ptn, c->ptn, c->ptn_sz * sizeof(vertex_t));
            } else {
                for (int i = 0; i < c->ptn_sz;) {
                    const vertex_t rd = c->ptn[i];
                    ptn[i] = rd;
                    i += rd + 1;
                }
            }
            memcpy(lab, c->lab, c->lab_sz * sizeof(vertex_t));
            memcpy(vertex_to_col, c->vertex_to_col, c->lab_sz * sizeof(vertex_t));
            memcpy(vertex_to_lab, c->vertex_to_lab, c->lab_sz * sizeof(vertex_t));

            lab_sz = c->lab_sz;
            ptn_sz = c->ptn_sz;

            cells = c->cells;
            smallest_cell_lower_bound = c->smallest_cell_lower_bound;
            init = true;
        }

        void initialize(int domain_size) {
            alloc(domain_size);
        }

        bool check() {
            bool comp = true;

            for (int i = 0; i < lab_sz; ++i) {
                comp = comp && (lab[i] >= 0 && lab[i] < lab_sz);
                comp = comp && (lab[vertex_to_lab[i]] == i);
            }

            int last_col = -1;
            int counter = 1;
            for (int i = 0; i < ptn_sz; ++i) {
                --counter;
                if (counter == 0) {
                    counter = ptn[i] + 1;
                    last_col = i;
                    assert(ptn[i] >= 0 && ptn[i] < lab_sz);
                } else {
                    assert(vertex_to_col[lab[i]] == last_col);
                }
            }

            for (int i = 0; i < ptn_sz;) {
                assert(vertex_to_col[lab[i]] == i);
                i += ptn[i] + 1;
            }
            return comp;
        }
    };
}

#endif //DEJAVU_COLORING_H
