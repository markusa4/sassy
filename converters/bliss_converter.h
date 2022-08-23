#ifndef SASSY_BLISS_CONVERTER_H
#define SASSY_BLISS_CONVERTER_H

static void convert_sassy_to_bliss(sassy::sgraph* sassy_graph, int* sassy_col, bliss::Graph* bliss_graph) {
    std::vector<int> sassy_v_to_bliss_v;
    sassy_v_to_bliss_v.reserve(sassy_graph->v_size);
    for(int i = 0; i < sassy_graph->v_size; ++i) {
        sassy_v_to_bliss_v.push_back(bliss_graph->add_vertex(sassy_col[i]));
    }
    for(int i = 0; i < sassy_graph->v_size; ++i) {
        const int ept = sassy_graph->v[i];
        const int vd  = sassy_graph->d[i];
        for(int j = ept; j < ept + vd; ++j) {
            const int to = sassy_graph->e[j];
            if(i < to) {
                bliss_graph->add_edge(i, to);
            }
        }
    }
}

#endif //SASSY_BLISS_CONVERTER_H
